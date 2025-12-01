#include "controlwidget.h"
#include "ui_controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);

}
void ControlWidget::allMascotasReceived(const QVector<Mascota>& lista){

}
void ControlWidget::insertMascotaResult(bool ok, int id){

}
void ControlWidget::mascotaByIdReceived(const Mascota& m){

}
void ControlWidget::mascotaByNameReceived(const QVector<Mascota>& lista){

}
ControlWidget::~ControlWidget()
{
    delete ui;
}
