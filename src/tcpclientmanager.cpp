#include "tcpclientmanager.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include "mascota.h"
TcpClientManager::TcpClientManager(QObject *parent)
    : QObject(parent),
    socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead,
            this, &TcpClientManager::onReadyRead);

    connect(socket, &QTcpSocket::connected,
            this, &TcpClientManager::onConnected);

    connect(socket, &QTcpSocket::disconnected,
            this, &TcpClientManager::onDisconnected);

    connect(socket, &QTcpSocket::errorOccurred,
            this, &TcpClientManager::onErrorOccurred);
}

void TcpClientManager::connectToServer(const QString& host, quint16 port)
{
    socket->connectToHost(host, port);
}

void TcpClientManager::sendJson(const QJsonObject& obj)
{
    if (!socket->isOpen()) {
        qWarning() << "Socket no está abierto";
        return;
    }

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    socket->write(data);
}

void TcpClientManager::onConnected()
{
    emit connected();
}

void TcpClientManager::onDisconnected()
{
    emit disconnected();
}

void TcpClientManager::onErrorOccurred(QAbstractSocket::SocketError)
{
    emit errorOccurred(socket->errorString());
}
void TcpClientManager::onReadyRead()
{
    buffer.append(socket->readAll());

    while (true) {
        int index = buffer.indexOf('\n');
        if (index < 0)
            break;

        QByteArray message = buffer.left(index);
        buffer.remove(0, index + 1);

        // Parsear JSON
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(message, &err);

        if (err.error != QJsonParseError::NoError) {
            qWarning() << "JSON cliente inválido:" << err.errorString();
            continue;
        }

        if (!doc.isObject()) {
            qWarning() << "Mensaje recibido no es objeto JSON";
            continue;
        }

        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();

        // === CLASIFICACIÓN POR TYPE ===
        if (type == "all_table") {
            const QJsonArray arr = obj["data"].toArray();
            QVector<Mascota> lista;
            lista.reserve(arr.size());
            for (auto v : arr)
                lista.append(mascotaFromJson(v.toObject()));

            emit allMascotasReceived(lista);
        }
        else if (type == "insert_ok" || type == "insert_error") {
            bool ok = (type == "insert_ok");
            int id = obj.value("id").toInt();
            emit insertMascotaResult(ok, id);
        }
        else if (type == "update_ok" || type == "update_error") {
            bool ok = (type == "update_ok");
            int id = obj.value("id").toInt();
            emit updateMascotaResult(ok, id);
        }
        else if (type == "delete_ok" || type == "delete_error") {
            bool ok = (type == "delete_ok");
            emit deleteMascotaResult(ok);
        }
        else if (type == "view_imagen_result"){
            QByteArray img = QByteArray::fromBase64(obj.value("foto").toString().toLatin1());
            emit MascotaByImagenReceived(img);
        }
        else if (type == "id_research") {
            Mascota m = mascotaFromJson(obj["data"].toObject());
            emit mascotaByIdReceived(m);
        }
        else if (type == "name_research") {
            const QJsonArray arr = obj["data"].toArray();
            QVector<Mascota> lista;
            lista.reserve(arr.size());
            for (auto v : arr)
                lista.append(mascotaFromJson(v.toObject()));

            emit mascotaByNameReceived(lista);
        }
        else {
            qWarning() << "Tipo de mensaje desconocido en cliente:" << type;
        }
    }
}
