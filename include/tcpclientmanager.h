#ifndef TCPCLIENTMANAGER_H
#define TCPCLIENTMANAGER_H
#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
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
    void jsonReceived(const QJsonObject& json);
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
