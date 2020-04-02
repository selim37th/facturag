#include "faddabonocargo.h"
#include "ui_faddabonocargo.h"

// de momento esto como hemos visto hacer el pantallazo del widget
// no continuamos desarrollando

fAddAbonoCargo::fAddAbonoCargo(QWidget *parent, QString *op,  unsigned int ID) :
    QDialog(parent),
    ui(new Ui::fAddAbonoCargo)
{
    ui->setupUi(this);
    idUsuario = ID;
    ui->lblOperacion->setText(ui->lblOperacion->text() + " " + op);
    emit on_rbPropio_clicked();

    enablePropios(true);
    ui->sbIvaPorPropio->setValue(::IVADEFAULT);
    enableManual(false);
    ui->sbIvaPorManual->setValue(::IVADEFAULT);
    enableSoloDetalle(false);
    ui->rbPropio->setChecked(true);

    operacion = *op;
    ui->deFecha->setDate(QDate::currentDate());

    ui->sbCantidadPropio->setValue(1);
    ui->sbCantidadManual->setValue(1);

    cargaDatosProductos();


}

fAddAbonoCargo::~fAddAbonoCargo()
{
    delete ui;
}

void fAddAbonoCargo::accept(){ /////////////////////// GUARDAR ABONO CARGO ////////////////////
    // no pedimos confirmacion pa ir mas rapido
    // porque luego se puede eliminar rapido la linea

    // check campos
    if (checkCampos()) {

        // ver tipo de operacion: cargo o abono
        guardarLinea();

        QDialog::accept();
    }
}

void fAddAbonoCargo::guardarLinea(){
    QSqlQuery q;
    q.prepare("INSERT INTO abonoscargos (fecha, idcliente, idproducto, detalle, importe, cantidad, ivapor, debe, haber) VALUES ("
              ":fecha, :idusu, :idpro, :deta, :imp, :cant, :ivap, :de, :ha)");
    q.bindValue(":fecha", ui->deFecha->date().toString("yyyy-MM-dd"));
    q.bindValue(":idusu", idUsuario);

    switch (accion) {
    case 1: //

        q.bindValue(":deta", ui->txtConceptoPropio->text());
        q.bindValue(":imp", ui->sbImportePropio->value());
        q.bindValue(":cant", ui->sbCantidadPropio->value());
        q.bindValue(":ivap", ui->sbIvaPorPropio->value());

        if (operacion == "CARGO") {
            q.bindValue(":de", ui->sbTotalPropio->value());
            q.bindValue(":ha", 0.0);
        }

        if (operacion == "ABONO") {
            q.bindValue(":de", 0.0);
            q.bindValue(":ha", ui->sbTotalPropio->value());

        }
        break;
    case 2:
        q.bindValue(":deta", ui->txtConceptoManual->text());
        q.bindValue(":imp", ui->sbImporteManual->value());
        q.bindValue(":cant", ui->sbCantidadManual->value());
        q.bindValue(":ivap", ui->sbIvaPorManual->value());

        if (operacion == "CARGO") {
            q.bindValue(":de", ui->sbTotalManual->value());
            q.bindValue(":ha", 0.0);
        }

        if (operacion == "ABONO") {
            q.bindValue(":de", 0.0);
            q.bindValue(":ha", ui->sbTotalManual->value());

        }
        break;
    case 3:
        q.bindValue(":deta", ui->txtConceptoPelao->text());
        q.bindValue(":imp", 0.0);
        q.bindValue(":cant", 0);
        q.bindValue(":ivap", 0.0);
        q.bindValue(":de", 0.0);
        q.bindValue(":ha", 0.0);
        break;
    }

    q.bindValue(":idpro", idproducto); // si es manual tendra valor 0
    q.exec();

    QSqlError err = q.lastError();
    if ( err.isValid() ) qDebug() << err.text();
    QSqlDatabase::database().commit();

}

void fAddAbonoCargo::reject() {   /////////////////// CANCELAR ABONO/CARGO /////////////////////
    QDialog::reject();
}

void fAddAbonoCargo::on_rbPropio_clicked() /////////////// CLICK EN PRODUCTO PROPIO
{
    accion = 1;
    enablePropios(true);
    enableManual(false);
    enableSoloDetalle(false);
}

void fAddAbonoCargo::on_rbManual_clicked() /////////////// CLICK EN PRODUCTO MANUAL
{
    accion = 2;
    idproducto = 0;
    enablePropios(false);
    enableManual(true);
    enableSoloDetalle(false);
}

void fAddAbonoCargo::on_rbSinImportes_clicked() /////////////// CLICK EN LINEA PELA
{
    accion = 3;
    enablePropios(false);
    enableManual(false);
    enableSoloDetalle(true);
}

// cargamos los datos de los productos disponible
void fAddAbonoCargo::cargaDatosProductos(){
    // Selecciono todos los productos
    QSqlQuery query;
    QSqlDatabase::database().database();
    modeloPro = new QSqlTableModel(this, QSqlDatabase::database().database());
    modeloPro->setTable("productos");
    modeloPro->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloPro->select();

    // recorro registros y añado al combo
    ui->cbCodigoPropio->addItem("Cod. | Descripción");
    int nfilas = modeloPro->rowCount();
    if (nfilas>0) {
        for ( int f = 0; f < nfilas; f++ ) {
            QString strcb = modeloPro->index(f,0).data().toString() + " | " + modeloPro->index(f,1).data().toString();
            ui->cbCodigoPropio->addItem(strcb);
        }
    }
}

void fAddAbonoCargo::on_cbCodigoPropio_currentIndexChanged(int index)
{
    // restamos 1 al index para que coincida con el modeloPro
    if (index > 0) {
        //qDebug() << modeloPro->index(index-1, 1).data().toString();
        idproducto = modeloPro->index(index-1, 0).data().toUInt();
        ui->txtConceptoPropio->setText(modeloPro->index(index-1, 1).data().toString());
        ui->sbImportePropio->setValue(modeloPro->index(index-1, 2).data().toDouble());
        ui->sbIvaPorPropio->setValue(modeloPro->index(index-1, 3).data().toDouble());
        ui->sbCantidadPropio->setValue(1);
        calculaTotales();
    }
}


////////////////// calculamos totales segun datos
void fAddAbonoCargo::calculaTotales(){
    // segun linea que estemos
    //bloquear todas las señales
    // para no hacer referencias cruzadas OJO

    switch (accion) {
        case 1: // linea propio producto
        {
                blockSignalsCamposPropias(true);
                double impIva = 0;
                impIva = ((ui->sbImportePropio->value() * ui->sbCantidadPropio->value()) * ui->sbIvaPorPropio->value()) / 100;
                ui->sbImporteTotalPropio->setValue(ui->sbImportePropio->value() * ui->sbCantidadPropio->value());
                ui->sbIvaImportePropio->setValue(impIva);
                ui->sbTotalPropio->setValue((ui->sbImportePropio->value() * ui->sbCantidadPropio->value()) + impIva );
                blockSignalsCamposPropias(false);
        }
        break;
        case 2: // linea manual
        {
            blockSignalsCamposManual(true);
            double impIva = 0;
            impIva = ((ui->sbImporteManual->value() * ui->sbCantidadManual->value()) * ui->sbIvaPorManual->value()) / 100;
            ui->sbImporteTotalManual->setValue(ui->sbImporteManual->value() * ui->sbCantidadManual->value());
            ui->sbIvaImporteManual->setValue(impIva);
            ui->sbTotalManual->setValue((ui->sbImporteManual->value() * ui->sbCantidadManual->value()) + impIva );
            blockSignalsCamposManual(false);
        }
        break;

        case 3: // linea sin importes

                break;

    }

}

/************************    BLOQUEAR / DESBLOQUEAR SEÑALES para cuando hacemos numeros ****************/
void fAddAbonoCargo::blockSignalsCamposPropias(bool enable) {
    //ui->cbCodigoPropio->blockSignals(enable);
    //ui->txtConceptoPropio->blockSignals(enable);
    ui->sbImportePropio->blockSignals(enable);
    ui->sbCantidadPropio->blockSignals(enable);
    ui->sbImporteTotalPropio->blockSignals(enable);
    ui->sbIvaPorPropio->blockSignals(enable);
    ui->sbIvaImportePropio->blockSignals(enable);
    ui->sbTotalPropio->blockSignals(enable);
}

void fAddAbonoCargo::blockSignalsCamposManual(bool enable){
    ui->txtConceptoManual->blockSignals(enable);
    ui->sbImporteManual->blockSignals(enable);
    ui->sbCantidadManual->blockSignals(enable);
    ui->sbImporteTotalManual->blockSignals(enable);
    ui->sbIvaPorManual->blockSignals(enable);
    ui->sbIvaImporteManual->blockSignals(enable);
    ui->sbTotalManual->blockSignals(enable);
}
/*********************************************************************************************************/


////////////////////// HABILITAR DESHABILITAR LINEAS /////////////////////////
void fAddAbonoCargo::enablePropios(bool enable){
    ui->cbCodigoPropio->setEnabled(enable);
    ui->txtConceptoPropio->setEnabled(enable);
    ui->sbImportePropio->setEnabled(enable);
    ui->sbCantidadPropio->setEnabled(enable);
    ui->sbImporteTotalPropio->setEnabled(enable);
    ui->sbIvaPorPropio->setEnabled(enable);
    ui->sbIvaImportePropio->setEnabled(enable);
    ui->sbTotalPropio->setEnabled(enable);
}

void fAddAbonoCargo::enableManual(bool enable){
    ui->txtConceptoManual->setEnabled(enable);
    ui->sbImporteManual->setEnabled(enable);
    ui->sbCantidadManual->setEnabled(enable);
    ui->sbImporteTotalManual->setEnabled(enable);
    ui->sbIvaPorManual->setEnabled(enable);
    ui->sbIvaImporteManual->setEnabled(enable);
    ui->sbTotalManual->setEnabled(enable);
}

void fAddAbonoCargo::enableSoloDetalle(bool enable){
    ui->txtConceptoPelao->setEnabled(enable);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* al modificar los spinbox de producto propio */
void fAddAbonoCargo::on_sbCantidadPropio_valueChanged(int arg1)
{
    calculaTotales();
}

void fAddAbonoCargo::on_sbImportePropio_valueChanged(double arg1)
{
    calculaTotales();
}

void fAddAbonoCargo::on_sbIvaPorPropio_valueChanged(double arg1)
{
    calculaTotales();
}

// caso especial: ponen directamente precio final de la linea


void fAddAbonoCargo::on_sbTotalPropio_editingFinished()
{
    //cambiamos el importe por producto y luego calculatotales() ojo bloquea signals
    double iv = (ui->sbIvaPorPropio->value()/100) + 1.0;
    double imp = ui->sbTotalPropio->value() / iv;
    ui->sbImportePropio->blockSignals(true);
    ui->sbImportePropio->setValue(imp);
    ui->sbImportePropio->blockSignals(false);
    calculaTotales();
}

void fAddAbonoCargo::on_sbTotalManual_editingFinished()
{
    //cambiamos el importe por producto y luego calculatotales() ojo bloquea signals
    double iv = (ui->sbIvaPorManual->value()/100) + 1.0;
    double imp = ui->sbTotalManual->value() / iv;
    ui->sbImporteManual->blockSignals(true);
    ui->sbImporteManual->setValue(imp);
    ui->sbImporteManual->blockSignals(false);
    calculaTotales();
}

bool fAddAbonoCargo::checkCampos() {
    bool ok = true;

    // ver que tipo de linea estamos
    switch (accion) {
    case 1:
        if (idproducto <= 0 || ui->sbTotalPropio->value() == 0.0 || ui->txtConceptoPropio->text().isEmpty()) ok = false;
        break;
    case 2:
        if (ui->txtConceptoManual->text().isEmpty() || ui->sbTotalManual->value() == 0.0) ok = false;
        break;
    case 3:
        if (ui->txtConceptoPelao->text().isEmpty()) ok = false;
        break;
    }

    return ok;
}

void fAddAbonoCargo::on_sbImporteManual_valueChanged(double arg1)
{
    calculaTotales();
}

void fAddAbonoCargo::on_sbCantidadManual_valueChanged(int arg1)
{
    calculaTotales();
}

void fAddAbonoCargo::on_sbIvaPorManual_valueChanged(double arg1)
{
    calculaTotales();
}
