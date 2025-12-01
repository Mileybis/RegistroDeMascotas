#include "tcpclientmanager.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
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

    // Procesar mensajes línea por línea (terminan en '\n')
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

        emit jsonReceived(doc.object());
    }
}
