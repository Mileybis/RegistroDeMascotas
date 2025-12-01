#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class ControlWidget;
class TcpClientManager;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onClientError(const QString &msg);
    void onJsonReceived(const QJsonObject &json);
private:
    Ui::MainWindow *ui;
    ControlWidget *controlWidget;
    TcpClientManager *tcpClientManager;
    int selectPuerto();
    QString selectHost();
};
#endif // MAINWINDOW_H
