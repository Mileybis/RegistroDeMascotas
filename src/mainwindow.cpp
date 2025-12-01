#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QLabel>
#include "tcpclientmanager.h"
#include "controlwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlWidget = new ControlWidget(this);
    tcpClientManager = new TcpClientManager(this);
    setCentralWidget(controlWidget);
    connect(tcpClientManager,&TcpClientManager::connected,
        this,&MainWindow::onClientConnected);
    connect(tcpClientManager,&TcpClientManager::disconnected,
            this,&MainWindow::onClientDisconnected);
    connect(tcpClientManager,&TcpClientManager::errorOccurred,
            this,&MainWindow::onClientError);
    connect(tcpClientManager,&TcpClientManager::jsonReceived,
            this,&MainWindow::onJsonReceived);

    tcpClientManager->connectToServer(selectHost(),selectPuerto());
}
int MainWindow::selectPuerto(){
    bool ok;
    int puerto = QInputDialog::getInt(
        this, "Configuracion del Cliente",
        "Ingrese el puerto del servidor",
        8080,1,65535,1,&ok);
    if(!ok) puerto = 5050;
    QLabel* labelPuerto = new QLabel(QString("Puerto: %1").arg(puerto));
    statusBar()->addPermanentWidget(labelPuerto);

    return puerto;
}
QString MainWindow::selectHost(){
    bool ok;
    QString host = QInputDialog::getText(
        this, "Configuracion del Cliente",
        "Ingrese el Host del Servidor",
        QLineEdit::Normal,"192.168.0.10",&ok);
    if(!ok || host.trimmed().isEmpty()) host = "192.168.0.10";
    QLabel* labelHost = new QLabel(QString("Host: %1").arg(host));
    statusBar()->addPermanentWidget(labelHost);

    return host;
}
void MainWindow::onClientConnected()
{
    statusBar()->showMessage("Conectado al servidor", 3000);
}

void MainWindow::onClientDisconnected()
{
    statusBar()->showMessage("Desconectado del servidor", 3000);
}

void MainWindow::onClientError(const QString &msg)
{
    statusBar()->showMessage("Error: " + msg, 5000);
}

void MainWindow::onJsonReceived(const QJsonObject &json)
{
    qDebug() << "JSON recibido en MainWindow:" << json;

    // Aquí se procesará la data real
    // Ejemplo:
    // controlWidget->cargarTabla(json["data"].toArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}
