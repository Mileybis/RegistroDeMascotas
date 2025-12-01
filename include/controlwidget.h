#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "mascota.h"
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
signals:
    void RequestAllMascota();
    void requestAddMascota(const Mascota &m);
    void requestDeleteMascota(int id);
    void requestUpdateMascota(const Mascota &m);
    void requestResearchIdMascota(int id);
    void requestResearchNameMascota(const QString name);
private slots:
    void onRequestAllMascota();
    void onRequestAddMascota(const Mascota &m);
    void onRequestDeleteMascota(int id);
    void onRequestUpdateMascota(const Mascota &m);
    void onRequestResearchIdMascota(int id);
    void onRequestResearchNameMascota(const QString name);
private:
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H
