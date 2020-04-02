#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QDate>
#include <QList>
#include <QMap>
#include <QtUiTools>
#include <QLocale>

#include "conexion.h"
#include "fusuarios.h"
#include "fproducto.h"
#include "flistadoproductos.h"
#include "Evento.h"
#include "fcuentas.h"
#include "negocio.h"
#include "fdatosempresa.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* Siempre fecha de hoy */
static QDate hoy = QDate::currentDate();


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /* lista de id de clientes con la ficha abierta */
    QList<unsigned int> clientesAbiertos;
    QList<unsigned int> ctasAbiertas;

    /* unsigned int del ID cliente que tengo seleccionado en la lista de clientes */
    unsigned int ID=0;

    /* Para el día seleccionado en el calendario hay o no hay evento */
    bool hayEvento = false;

public slots:
    /* Cuando se cierra el Qdialog de la ficha cliente se saca de clientesAbiertos su id
    para poder volver a aabrirla */
    void sacaIdCliente(unsigned int id);
    void sacaIdFicha(unsigned int id);

private slots:

    void guardaDatosTW();
    void deshacerCambiosEnClientes();
    void cambiosEnClientes(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void on_calendario_selectionChanged();

    void on_btnBorrarTxtBuscar_clicked();

    void on_txtBuscarPorDNI_textChanged(const QString &arg1);

    void on_txtBuscarPorNombre_textChanged(const QString &arg1);

    void on_txtBuscarPorApellido1_textChanged(const QString &arg1);

    void on_txtBuscarPorApellido2_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_btnFichaCliente_clicked();

    /* click en tabla de cliente */
    void onClickTablaClientes(const QModelIndex &index);

    void on_fechaSeleccionada_dateChanged(const QDate &date);

    void on_txtEventos_textChanged();

    void on_btnGuardarDetalleEvento_clicked();

    void on_btnIrAhoy_clicked();

    void on_btnEliminarEvento_clicked();

    void on_actionSalir_triggered();

    void on_actionNuevo_triggered();

    void on_btnBorrarCliente_clicked();

    void on_actionCrearProducto_triggered();

    void on_actionListarProductos_triggered();

    void on_btnPagosFacturas_clicked();

    void on_actionDatos_Empresa_triggered();

private:
    Ui::MainWindow *ui;
    Negocio *N;
    QSqlTableModel *modelo;
    QSortFilterProxyModel *proxyModelo;
    QMap<QDate, QString> *mapaEventos;
    QMap<QString, QString> *mapaDatosEmpresa;
    QTextCharFormat formatoParaEvento;
    QTextCharFormat formatoNoEvento;
    bool haycambios = false;

    void log(QString const);
    void cargarListaEventos();

    void cargarDatosNegocio();


};
#endif // MAINWINDOW_H
