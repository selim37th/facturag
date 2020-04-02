#include "flistadoproductos.h"
#include "ui_flistadoproductos.h"
#include "fproducto.h"
#include <QDebug>

fListadoProductos::fListadoProductos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fListadoProductos)
{
    ui->setupUi(this);
    cargarDatosProductos();

    //conectar signals/slots
    connect(ui->TWProductos->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(cambiosEnProducto(const QModelIndex&, const QModelIndex&)));
    connect(ui->TWProductos, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClickTablaProductos(const QModelIndex &)));


    // Tunear boton guardar
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonNormal);
}

fListadoProductos::~fListadoProductos()
{
    delete ui;
}

void fListadoProductos::cargarDatosProductos(){
    QSqlQuery query;
    QSqlDatabase::database().database();
    modeloPro = new QSqlTableModel(this, QSqlDatabase::database().database());
    modeloPro->setTable("productos");
    modeloPro->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloPro->select();
    proxyModelo = new QSortFilterProxyModel(this);
    proxyModelo->setSourceModel(modeloPro);
    ui->TWProductos->setModel(proxyModelo);
    ui->TWProductos->resizeColumnsToContents();
    ui->TWProductos->setStyleSheet("QHeaderView::section { border: 1px solid black; font: bold}");
}

//Slot si se modifica algun dato de la tabla de productos
void fListadoProductos::cambiosEnProducto(const QModelIndex& topLeft, const QModelIndex& bottomRight){
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonAlerta);
    haycambios = true;
}

void fListadoProductos::deshacerCambiosEnProducto(){
    //qDebug() << "DeshacerCambios en Producto";
    modeloPro->revertAll();
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonNormal);
    haycambios=false;
}

void fListadoProductos::guardaDatosTW() {
    modeloPro->database().transaction();
    if (modeloPro->submitAll()) {
        modeloPro->database().commit();

        haycambios=false;
    } else {
        modeloPro->database().rollback();
        QMessageBox::warning(this, tr("Tabla usuarios corrompida"),
                             tr("Error: %1")
                             .arg(modeloPro->lastError().text()));
    }
}

//Slot hacen click en una linea de producto
void fListadoProductos::onClickTablaProductos(const QModelIndex & indexx){
    if (indexx.isValid()) {
           /* Da valor en ID de la celda clickada */
           unsigned int y = indexx.row();
           QModelIndex pos = ui->TWProductos->model()->index(y,0);
           QString cellText = pos.data().toString();
           //qDebug() << "Onclick id = " << cellText;
           ID = cellText.toInt();
       }
}


void fListadoProductos::accept(){
    guardaDatosTW();
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonNormal);
}

void fListadoProductos::reject() {
    if (haycambios){
         QMessageBox::StandardButton resBtn;
         resBtn = QMessageBox::question( this, "FACG",
                                      tr("Se han producido cambios, desea GUARDARLOS?\n"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
         if (resBtn == QMessageBox::Yes){
             guardaDatosTW();
             QDialog::reject();
         }
         if (resBtn == QMessageBox::No) {
             QDialog::reject();
         }

    }
    else
        QDialog::reject();
}
void fListadoProductos::on_txtBuscarDescripcion_textChanged(const QString &arg1)
{
    proxyModelo->setFilterKeyColumn(1); // por descripcion
    proxyModelo->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString ));
}

void fListadoProductos::on_btnBorrarBusqueda_clicked()
{
    ui->txtBuscarDescripcion->setText(QString());
    ui->txtBuscarDescripcion->setFocus();
}

void fListadoProductos::on_btnEliminarProducto_clicked()
{
    if (ID > 0 ) {
        QMessageBox::StandardButton resBtn;
        resBtn = QMessageBox::question( this, "FACG",
                                     tr("Desea borrar el producto seleccionado?\n"),
                                     QMessageBox::Yes | QMessageBox::No);
        if (resBtn == QMessageBox::Yes){
            //Eliminar
            eliminaProducto();
            modeloPro->select();
        }
        if (resBtn == QMessageBox::No) {
            //No hacer nada
        }
    }

}
// Elimina el producto seleccionado
void fListadoProductos::eliminaProducto(){

    QSqlQuery query;
    query.prepare("DELETE FROM productos WHERE id=:id");
    query.bindValue(":id", ID);
    query.exec();
    QSqlDatabase::database().commit();

}

//slot ver ficha del producto seleccionado
void fListadoProductos::on_btnFichaProducto_clicked()
{
    // primero primero ver si hay selección valida en la tablaview
    if (ID){

        Producto *mipro;
        mipro = new Producto();
        mipro->setId(ID);

        int seleccion = ui->TWProductos->selectionModel()->currentIndex().row();

        QModelIndex row = ui->TWProductos->model()->index(seleccion,1); // Descripcion
        //QString des = row.data().toString();
        mipro->setDescripcion(row.data().toString());

        row = ui->TWProductos->model()->index(seleccion,2); // precio
        //double pre = row.data().toDouble();
        mipro->setImporte(row.data().toDouble());

        row = ui->TWProductos->model()->index(seleccion,3); // IVA
        //double iva = row.data().toDouble();
        mipro->setIva(row.data().toDouble());

        row = ui->TWProductos->model()->index(seleccion,4); // observaciones
        //QString obs = row.data().toString();
        mipro->setObservaciones(row.data().toString());

        //qDebug() << mipro->toString();

        fproducto *fp = new fproducto(this, mipro);
        fp->setModal(true);
        fp->exec();

        delete (mipro);

        modeloPro->select();

    }




}
