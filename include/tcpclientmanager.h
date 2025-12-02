#ifndef TCPCLIENTMANAGER_H
#define TCPCLIENTMANAGER_H
#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include "mascota.h"
class TcpClientManager : public QObject
{
    Q_OBJECT
public:
    explicit TcpClientManager(QObject *parent = nullptr);
    void connectToServer(const QString& host,quint16 port);
    void sendJson(const QJsonObject& obj);
signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& error);
    //void jsonReceived(const QJsonObject& json);
    void allMascotasReceived(const QVector<Mascota>& lista);
    void insertMascotaResult(bool ok, int id);
    void updateMascotaResult(bool ok, int id);
    void deleteMascotaResult(bool ok);
    void mascotaByIdReceived(const Mascota& m);
    void mascotaByNameReceived(const QVector<Mascota>& lista);
    void MascotaByImagenReceived(const QByteArray& img);
private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket* socket;
    QByteArray buffer;
};

#endif // TCPCLIENTMANAGER_H
