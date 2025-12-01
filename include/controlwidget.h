#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "mascota.h"
#include "tcpclientmanager.h"
namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();
    void allMascotasReceived(const QVector<Mascota>& lista);
    void insertMascotaResult(bool ok, int id);
    //void updateMascotaResult(bool ok, int id);
    //void deleteMascotaResult(bool ok);
    void mascotaByIdReceived(const Mascota& m);
    void mascotaByNameReceived(const QVector<Mascota>& lista);
    void ImagenMascota(const QByteArray& img);
signals:
    void requestAllMascota();
    void requestAddMascota(const Mascota &m);
    void requestDeleteMascota(int id);
    void requestUpdateMascota(const Mascota &m);
    void requestResearchIdMascota(int id);
    void requestImagenMascota(int id);
    void requestResearchNameMascota(const QString name);
private slots:
    void onRequestResearchMascota();
    void onRequestAddMascota();
    void onRequestDeleteMascota();
    void onRequestUpdateMascota();
    void onRequestImagenMascota(int row,int column);
    void subirFoto();
    void imprimirDato();
private:
    Ui::ControlWidget *ui;
    TcpClientManager *client;
};

#endif // CONTROLWIDGET_H
