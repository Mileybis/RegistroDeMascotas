#include "controlwidget.h"
#include "ui_controlwidget.h"
#include <QMessageBox>
#include <QBuffer>
#include <QFileDialog>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTableWidgetItem>
ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    connect(ui->PushButtonBuscar,&QPushButton::clicked,
        this,&ControlWidget::onRequestResearchMascota);
    connect(ui->PushButtonAgregar,&QPushButton::clicked,
            this,&ControlWidget::onRequestAddMascota);
    connect(ui->PushButtonEliminar,&QPushButton::clicked,
            this,&ControlWidget::onRequestDeleteMascota);
    connect(ui->PushButtonEditar,&QPushButton::clicked,
            this,&ControlWidget::onRequestUpdateMascota);
    connect(ui->PushButtonSubirFoto,&QPushButton::clicked,
            this,&ControlWidget::subirFoto);
    connect(ui->PushButtonImprimir,&QPushButton::clicked,
            this,&ControlWidget::imprimirDato);
    connect(ui->tableWidget,&QTableWidget::cellClicked,
            this,&ControlWidget::onRequestImagenMascota);
}
void ControlWidget::allMascotasReceived(const QVector<Mascota>& lista)
{
    auto tabla = ui->tableWidget;

    tabla->clear();
    tabla->setRowCount(0);

    QStringList headers = {
        "ID","Nombre","Raza","Genero","Peso kg","Edad","Especie"
    };

    tabla->setColumnCount(headers.size());
    tabla->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const Mascota &m : lista)
    {
        tabla->insertRow(row);

        tabla->setItem(row, 0, new QTableWidgetItem(QString::number(m.id)));
        tabla->setItem(row, 1, new QTableWidgetItem(m.nombre));
        tabla->setItem(row, 2, new QTableWidgetItem(m.raza));
        tabla->setItem(row, 3, new QTableWidgetItem(m.sexo));
        tabla->setItem(row, 4, new QTableWidgetItem(QString::number(m.peso)));
        tabla->setItem(row, 5, new QTableWidgetItem(QString::number(m.edad)));
        tabla->setItem(row, 6, new QTableWidgetItem(m.especie));

        row++;
    }

    tabla->resizeColumnsToContents();
}
void ControlWidget::ImagenMascota(const QByteArray& img)
{
    QPixmap pix;

    // Cargar el pixmap desde los bytes
    if (!pix.loadFromData(img)) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen.");
        return;
    }

    // Mostrar en el label, escalado correctamente
    ui->labelImagen->setPixmap(
        pix.scaled(ui->labelImagen->size(),
                   Qt::KeepAspectRatio,
                   Qt::SmoothTransformation)
        );
}
void ControlWidget::insertMascotaResult(bool ok, int id){
    if(ok) QMessageBox::information(this,"Informacion",QString("Se Agrego Con exito: %1").arg(id));
    else QMessageBox::information(this,"Informacion",QString("Hubo un erro agregando: %1").arg(id));
}
void ControlWidget::mascotaByIdReceived(const Mascota& m)
{
    ui->spinBoxId->setValue(m.id);
    ui->LineEditNombre->setText(m.nombre);
    ui->LineEditRaza->setText(m.raza);
    ui->LineEditSexo->setText(m.sexo);
    ui->doubleSpinBoxPeso->setValue(m.peso);
    ui->spinBoxEdad->setValue(m.edad);
    ui->ComboBoxTipo->setCurrentText(m.especie);
    QPixmap foto;
    foto.loadFromData(m.foto);
    // mostrar imagen
    if (!m.foto.isNull()) {
        ui->labelImagen->setPixmap(
            foto.scaled(ui->labelImagen->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation));
    } else {
        ui->labelImagen->clear();
        ui->labelImagen->setText("Sin Imagen");
    }
}
void ControlWidget::mascotaByNameReceived(const QVector<Mascota>& lista)
{
    if (lista.isEmpty()) {
        QMessageBox::information(this, "Resultado", "No se encontraron coincidencias");
        return;
    }

    allMascotasReceived(lista);
}
void ControlWidget::onRequestResearchMascota(){
    int id = ui->spinBoxId->value();
    QString name = ui->LineEditNombreBuscar->text();
    if(id >= 0){
        emit requestResearchIdMascota(id);
    }else if(!name.trimmed().isEmpty()){
        emit requestResearchNameMascota(name);
    }else{
        emit requestAllMascota();
    }


}
void ControlWidget::onRequestAddMascota(){
    Mascota m;
    m.nombre = ui->LineEditNombre->text();
    if(m.nombre.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Nombre Invalido"));
        return;
    }
    m.raza = ui->LineEditRaza->text();
    if(m.raza.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Raza Invalido"));
        return;
    }
    m.edad = ui->spinBoxEdad->value();
    m.especie = ui->ComboBoxTipo->currentText();
    if(m.especie.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Especie Invalido"));
        return;
    }
    m.peso = ui->doubleSpinBoxPeso->value();
    m.sexo = ui->LineEditSexo->text();
    if(m.sexo.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Sexo Invalido"));
        return;
    }
    QPixmap pix = ui->labelImagen->pixmap(Qt::ReturnByValue);

    if (!pix.isNull()) {
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "PNG");  // o "JPG"
        m.foto = data;
    }
    if(m.foto.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Imagen Invalido"));
        return;
    }
    emit requestAddMascota(m);
}
void ControlWidget::onRequestDeleteMascota(){
    int id = ui->spinBoxId->value();
    if(id < 0){
        QMessageBox::warning(this,"Advertencia",QString("id Invalido"));
        return;
    }
    emit requestDeleteMascota(id);
}
void ControlWidget::onRequestUpdateMascota(){
    Mascota m;
    m.id = ui->spinBoxId->value();
    if(m.id < 0){
        QMessageBox::warning(this,"Advertencia",QString("id Invalido"));
        return;
    }
    m.nombre = ui->LineEditNombre->text();
    if(m.nombre.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Nombre Invalido"));
        return;
    }
    m.raza = ui->LineEditRaza->text();
    if(m.raza.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Raza Invalido"));
        return;
    }
    m.edad = ui->spinBoxEdad->value();
    m.especie = ui->ComboBoxTipo->currentText();
    if(m.especie.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Especie Invalido"));
        return;
    }
    m.peso = ui->doubleSpinBoxPeso->value();
    m.sexo = ui->LineEditSexo->text();
    if(m.sexo.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Sexo Invalido"));
        return;
    }
    QPixmap pix = ui->labelImagen->pixmap(Qt::ReturnByValue);

    if (!pix.isNull()) {
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "PNG");  // o "JPG"
        m.foto = data;
    }
    if(m.foto.isEmpty()){
        QMessageBox::warning(this,"Advertencia",QString("Imagen Invalido"));
        return;
    }

    emit requestUpdateMascota(m);
}
void ControlWidget::subirFoto()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Seleccionar imagen",
        QString(),
        "Imágenes (*.png *.jpg *.jpeg *.bmp)"
        );

    if (fileName.isEmpty())
        return; // usuario canceló

    QPixmap pix;
    if (!pix.load(fileName)) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen seleccionada.");
        return;
    }

    // Escalar suavemente para el label
    QPixmap scaled = pix.scaled(
        ui->labelImagen->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    ui->labelImagen->setPixmap(scaled);
}
void ControlWidget::imprimirDato()
{
    Mascota m;
    m.id = ui->spinBoxId->value();
    m.nombre = ui->LineEditNombre->text();
    m.raza = ui->LineEditRaza->text();
    m.sexo = ui->LineEditSexo->text();
    m.edad = ui->spinBoxEdad->value();
    m.peso = ui->doubleSpinBoxPeso->value();
    m.especie = ui->ComboBoxTipo->currentText();

    QPixmap pix = ui->labelImagen->pixmap(Qt::ReturnByValue);

    if (m.nombre.isEmpty()) {
        QMessageBox::warning(this, "Imprimir", "No hay datos válidos para imprimir.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);

    // Usuario elige impresora o PDF
    if (dialog.exec() != QDialog::Accepted)
        return;

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Error", "No se pudo iniciar la impresión.");
        return;
    }

    int y = 200;
    int x = 200;

    QFont titleFont("Arial", 16, QFont::Bold);
    QFont textFont("Arial", 12);
    painter.setFont(titleFont);
    painter.drawText(x, y, "Ficha de Mascota");
    y += 300;

    painter.setFont(textFont);
    painter.drawText(x, y, QString("ID: %1").arg(m.id)); y += 200;
    painter.drawText(x, y, QString("Nombre: %1").arg(m.nombre)); y += 200;
    painter.drawText(x, y, QString("Raza: %1").arg(m.raza)); y += 200;
    painter.drawText(x, y, QString("Sexo: %1").arg(m.sexo)); y += 200;
    painter.drawText(x, y, QString("Edad: %1 años").arg(m.edad)); y += 200;
    painter.drawText(x, y, QString("Peso: %1 kg").arg(m.peso)); y += 200;
    painter.drawText(x, y, QString("Especie: %1").arg(m.especie)); y += 300;

    if (!pix.isNull()) {
        int imgW = 600;
        int imgH = (pix.height() * imgW) / pix.width();
        painter.drawPixmap(x, y, imgW, imgH, pix);
    }

    painter.end();

    QMessageBox::information(this, "Impreso", "El documento se imprimió correctamente.");
}
void ControlWidget::onRequestImagenMascota(int row,int column){
    Q_UNUSED(column)
    int id = ui->tableWidget->item(row,0)->text().toInt();
    if(id < 0){
        QMessageBox::warning(this,"Advertencia",QString("id Invalido"));
        return;
    }
    emit requestImagenMascota(id);
}
ControlWidget::~ControlWidget()
{
    delete ui;
}
