#ifndef FEXPORTIMAGECUENTASCLIENTE_H
#define FEXPORTIMAGECUENTASCLIENTE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDate>

#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QFileDialog>

namespace Ui {
class fExportImageCuentasCliente;
}

class fExportImageCuentasCliente : public QDialog
{
    Q_OBJECT

public:
    explicit fExportImageCuentasCliente(QWidget *parent = nullptr, QSqlQueryModel *mcta = nullptr);
    ~fExportImageCuentasCliente();

private slots:
    void on_pushButton_clicked();

private:
    Ui::fExportImageCuentasCliente *ui;


    QSqlQueryModel *modeloCuentas;

    // para el dibujo
    QPicture pi;
    QImage *imagen;
    //QPainter p;


};

#endif // FEXPORTIMAGECUENTASCLIENTE_H
