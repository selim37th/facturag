#ifndef FADDABONOCARGO_H
#define FADDABONOCARGO_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDate>

#include "negocio.h"

namespace Ui {
class fAddAbonoCargo;
}

class fAddAbonoCargo : public QDialog
{
    Q_OBJECT

public:
    explicit fAddAbonoCargo(QWidget *parent = nullptr, QString *operacion=nullptr, unsigned int ID=0);
    ~fAddAbonoCargo();

    QString operacion = QString();

private slots:
    void on_rbPropio_clicked();

    void on_rbManual_clicked();

    void on_rbSinImportes_clicked();

    void on_cbCodigoPropio_currentIndexChanged(int index);

    void on_sbCantidadPropio_valueChanged(int arg1);

    void on_sbImportePropio_valueChanged(double arg1);

    void on_sbIvaPorPropio_valueChanged(double arg1);



    void on_sbTotalPropio_editingFinished();

    void on_sbTotalManual_editingFinished();

    void on_sbImporteManual_valueChanged(double arg1);

    void on_sbCantidadManual_valueChanged(int arg1);

    void on_sbIvaPorManual_valueChanged(double arg1);

private:
    Ui::fAddAbonoCargo *ui;

    void enablePropios(bool);
    void enableManual(bool);
    void enableSoloDetalle(bool);
    void cargaDatosProductos();
    void calculaTotales();
    void blockSignalsCamposPropias(bool);
    void blockSignalsCamposManual(bool);
    bool checkCampos();
    void guardarLinea();
    void accept() override;
    void reject() override;

    QSqlTableModel *modeloPro;
    unsigned int idUsuario=0;
    unsigned int idproducto=0;
    int accion=0; /*1-> linea de propios
                    2-> linea manual
                    3-> linea sin importes*/

};

#endif // FADDABONOCARGO_H
