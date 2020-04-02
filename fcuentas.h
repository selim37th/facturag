#ifndef FCUENTAS_H
#define FCUENTAS_H
#include <QDebug>
#include <QDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include "faddabonocargo.h"
#include <QFileDialog>


#include <QPixmap>

namespace Ui {
class fCuentas;
}

class fCuentas : public QDialog
{
    Q_OBJECT

public:
    explicit fCuentas(QWidget *parent = nullptr, unsigned int id = 0);
    ~fCuentas();

signals:
    void cerrandoFichaCuenta(unsigned int);





private:
    unsigned int IDusuario=0; // ide de usuario;
    unsigned int IDdetalle=0; // ide para el detalle seleccionado en la tabla
    void reject() override;
    void accept() override;
    void cargarDatosCliente();
    void cargarCuentas();
    void tunearTabla();
    void eliminaDetalle();

    Ui::fCuentas *ui;
    QSqlQueryModel *modeloCuentas;
    double saldo=0.0;
    double sumaDebe=0.0;
    double sumaHaber=0.0;
    bool haycambios = false;
    QString nombreClienteTuneado;

private slots:
    void onClickTablaDetalles(const QModelIndex &);

    void on_btnEliminaDetalle_clicked();
    void on_btnAnnadeCargo_clicked();
    void on_btnAnnadeAbono_clicked();
    void on_btnExportar_clicked();
};

#endif // FCUENTAS_H
