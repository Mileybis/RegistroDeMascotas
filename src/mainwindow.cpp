#include "mainwindow.h"
#include "mascota.h"
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
    //repuesta del servidor al cliente
    connect(tcpClientManager,&TcpClientManager::allMascotasReceived,
            this,&MainWindow::onAllMascotasReceived);
    connect(tcpClientManager,&TcpClientManager::insertMascotaResult,
            this,&MainWindow::onInsertMascotaResult);
    connect(tcpClientManager,&TcpClientManager::mascotaByIdReceived,
            this,&MainWindow::onMascotaByIdReceived);
    connect(tcpClientManager,&TcpClientManager::mascotaByNameReceived,
            this,&MainWindow::onMascotaByNameReceived);
    connect(tcpClientManager,&TcpClientManager::MascotaByImagenReceived,
            this,&MainWindow::onMascotaByImagenReceived);
    tcpClientManager->connectToServer(selectHost(),selectPuerto());
    //conexiones del controlwidget al main para comunicarse al server
    connect(controlWidget,&ControlWidget::requestAllMascota,
            this,&MainWindow::requestAllMascota);
    connect(controlWidget,&ControlWidget::requestAddMascota,
            this,&MainWindow::requestAddMascota);
    connect(controlWidget,&ControlWidget::requestDeleteMascota,
            this,&MainWindow::requestDeleteMascota);
    connect(controlWidget,&ControlWidget::requestImagenMascota,
            this,&MainWindow::requestImagenMascota);
    connect(controlWidget,&ControlWidget::requestUpdateMascota,
            this,&MainWindow::requestUpdateMascota);
    connect(controlWidget,&ControlWidget::requestResearchIdMascota,
            this,&MainWindow::requestResearchIdMascota);
    connect(controlWidget,&ControlWidget::requestResearchNameMascota,
            this,&MainWindow::requestResearchNameMascota);
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
void MainWindow::requestAllMascota(){
    QJsonObject req;
    req["type"]="request_all";
    tcpClientManager->sendJson(req);
}
void MainWindow::requestAddMascota(const Mascota &m){
    QJsonObject req;
    req["type"]="add";
    req["data"]=mascotaToJson(m);
    tcpClientManager->sendJson(req);
}
void MainWindow::requestDeleteMascota(int id){
    QJsonObject req;
    req["type"]="delete";
    req["id"]=id;
    tcpClientManager->sendJson(req);
}
void MainWindow::requestUpdateMascota(const Mascota &m){
    QJsonObject req;
    req["type"]="update";
    req["data"]=mascotaToJson(m);
    tcpClientManager->sendJson(req);
}
void MainWindow::requestResearchIdMascota(int id){
    QJsonObject req;
    req["type"]="research_id";
    req["id"]=id;
    tcpClientManager->sendJson(req);
}
void MainWindow::requestImagenMascota(int id){
    QJsonObject req;
    req["type"]="view_imagen_id";
    req["id"]=id;
    tcpClientManager->sendJson(req);
}
void MainWindow::requestResearchNameMascota(const QString name){
    QJsonObject req;
    req["type"]="research_name";
    req["nombre"]=name;
    tcpClientManager->sendJson(req);
}
void MainWindow::onAllMascotasReceived(const QVector<Mascota>& lista){
    controlWidget->allMascotasReceived(lista);
}
void MainWindow::onInsertMascotaResult(bool ok, int id){
    controlWidget->insertMascotaResult(ok,id);
}
//void MainWindow::updateMascotaResult(bool ok, int id);
//void deleteMascotaResult(bool ok);
void MainWindow::onMascotaByIdReceived(const Mascota& m){
    controlWidget->mascotaByIdReceived(m);
}
void MainWindow::onMascotaByNameReceived(const QVector<Mascota>& lista){
    controlWidget->mascotaByNameReceived(lista);
}
void MainWindow::onMascotaByImagenReceived(const QByteArray& img){
    controlWidget->ImagenMascota(img);
}
MainWindow::~MainWindow()
{
    delete ui;
}
