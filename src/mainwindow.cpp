#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QLabel>

#include "controlwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlWidget = new ControlWidget(this);
    setCentralWidget(controlWidget);
    selectPuerto();
}
int MainWindow::selectPuerto(){
    bool ok;
    int puerto = QInputDialog::getInt(
        this, "Configuracion del Servidor",
        "Ingrese el puerto",
        8080,1,65535,1,&ok);
    if(!ok) puerto = 5050;
    QLabel* labelPuerto = new QLabel(QString("Puerto: %1").arg(puerto));
    statusBar()->addPermanentWidget(labelPuerto);

    return puerto;
}
MainWindow::~MainWindow()
{
    delete ui;
}
