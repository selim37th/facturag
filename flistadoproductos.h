#ifndef FLISTADOPRODUCTOS_H
#define FLISTADOPRODUCTOS_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QMessageBox>


#include "negocio.h"
#include "producto.h"

namespace Ui {
class fListadoProductos;
}

class fListadoProductos : public QDialog
{
    Q_OBJECT

public:
    explicit fListadoProductos(QWidget *parent = nullptr);
    ~fListadoProductos();

private:
    Ui::fListadoProductos *ui;
    QSqlTableModel *modeloPro;
    QSortFilterProxyModel *proxyModelo;
    bool haycambios = false;
    unsigned int ID = 0;
    void cargarDatosProductos();
    void guardaDatosTW();
    void eliminaProducto();

    void accept() override;// boton guardar
    void reject() override; // boton salir


private slots:
    void cambiosEnProducto(const QModelIndex&, const QModelIndex&);
    void onClickTablaProductos(const QModelIndex &);
    void deshacerCambiosEnProducto();
    void on_txtBuscarDescripcion_textChanged(const QString &arg1);
    void on_btnBorrarBusqueda_clicked();
    void on_btnEliminarProducto_clicked();
    void on_btnFichaProducto_clicked();
};

#endif // FLISTADOPRODUCTOS_H
