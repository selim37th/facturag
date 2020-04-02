#include "fcuentas.h"
#include "ui_fcuentas.h"
#include <QTextCharFormat>

fCuentas::fCuentas(QWidget *parent, unsigned int id) :
    QDialog(parent),
    ui(new Ui::fCuentas)
{
    ui->setupUi(this);
    IDusuario = id;
    cargarDatosCliente();
    cargarCuentas();
    tunearTabla();

    // conecta click soble la tabla
    connect(ui->TWCuentas, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClickTablaDetalles(const QModelIndex &)));
}

fCuentas::~fCuentas()
{
    delete ui;
}


void fCuentas::reject(){
    emit cerrandoFichaCuenta(IDusuario);
    QDialog::reject();
}

void fCuentas::accept(){
    emit cerrandoFichaCuenta(IDusuario);
    QDialog::accept();
}

void fCuentas::cargarDatosCliente(){
    // cargar datos de la DB
    QSqlQuery qselect;
    qselect.prepare("select * from usuarios where id=:id");
    qselect.bindValue(":id", IDusuario);
    qselect.exec();

    if (qselect.next()){
        QString nombreToMayusculas = qselect.value(6).toString() + " " + qselect.value(7).toString() + ", " + qselect.value(5).toString() + ". " + qselect.value(4).toString();
        QString tit = "Cargos/Abonos: " + nombreToMayusculas.toUpper();
        nombreClienteTuneado = "ctas" + qselect.value(6).toString().toUpper() + qselect.value(7).toString().toUpper() +  qselect.value(5).toString().toUpper();
        nombreClienteTuneado.replace(" ", "");
        this->setWindowTitle(tit);
        ui->lblNombre->setText(nombreToMayusculas.toUpper());
    }
}

void fCuentas::cargarCuentas() {
    sumaDebe = 0.0;
    sumaHaber = 0.0;
    QSqlDatabase::database().database();
    modeloCuentas = new QSqlQueryModel();
    //para fecha formato spanish strftime('%d-%m-%Y', fecha)
    QString q = QString("select id, strftime('%d-%m-%Y', fecha), idcliente, idproducto, detalle, importe, cantidad, ivapor, debe, haber from abonoscargos where idcliente=%1 order by fecha ASC, id ASC").arg(IDusuario);
    modeloCuentas->setQuery(q);
    modeloCuentas->setHeaderData(1, Qt::Horizontal, "FECHA");
    modeloCuentas->setHeaderData(4, Qt::Horizontal, "DETALLE");
    modeloCuentas->setHeaderData(5, Qt::Horizontal, "IMPORTE/u");
    modeloCuentas->setHeaderData(6, Qt::Horizontal, "UNIDADES");
    modeloCuentas->setHeaderData(7, Qt::Horizontal, "IVA");
    modeloCuentas->setHeaderData(8, Qt::Horizontal, "DEBE");
    modeloCuentas->setHeaderData(9, Qt::Horizontal, "HABER");

    ui->TWCuentas->setModel(modeloCuentas);

    // calcular importe del saldo
    int nfilas = modeloCuentas->rowCount();
    if (nfilas>0) {
        QModelIndex puntero;
        for ( int f = 0; f < nfilas; f++ ) {

            puntero = modeloCuentas->index(f,8);
            QString strdebe = puntero.data().toString().replace(",", ".");
            sumaDebe = sumaDebe + strdebe.toDouble();

            puntero = modeloCuentas->index(f,9);
            QString strHaber = puntero.data().toString().replace(",", ".");
            sumaHaber = sumaHaber+ strHaber.toDouble();


        }

        saldo = sumaDebe-sumaHaber;
        ui->txtTotalDebe->setValue(sumaDebe);
        ui->txtTotalHaber->setValue(sumaHaber);
        ui->txtSaldo->setValue(saldo);

        if (saldo < 0 ) ui->txtSaldo->setStyleSheet("color: black");
        else ui->txtSaldo->setStyleSheet("color: red");
    }



}

void fCuentas::tunearTabla(){
    //Esconder algunas columnas
    ui->TWCuentas->setColumnHidden(0, true);
    ui->TWCuentas->setColumnHidden(2, true);
    ui->TWCuentas->setColumnHidden(3, true);

    // tunear la tabla
    ui->TWCuentas->setColumnWidth(1, 105); // fecha
    ui->TWCuentas->setColumnWidth(4, 170); // detalle
    ui->TWCuentas->setColumnWidth(5, 105);  //importe/u
    ui->TWCuentas->setColumnWidth(6, 86);  //cantidad
    ui->TWCuentas->setColumnWidth(7, 60);  //IVA
    ui->TWCuentas->setColumnWidth(8, 80);  // debe
    ui->TWCuentas->setColumnWidth(9, 80);  // haber
    ui->TWCuentas->setStyleSheet("QHeaderView::section {border: 1px solid black; font: bold}");

    /* ********************** PRUEBAS PARA ALIEAR A LA DERECHA **********************
    QTableWidgetItem  itemdetabla;
    QStandardItem *item;
    QModelIndex indice;
    for (int z =0; z< ui->TWCuentas->model()->rowCount(); ++z) {
        for (int y =0; y< ui->TWCuentas->model()->columnCount(); ++y){
            item= new QStandardItem();

            indice = ui->TWCuentas->model()->index(z,y);





            qDebug() << indice.data().toString();
            //item->setText(modeloCuentas->index(z,y).data().toString());
            //item->setTextAlignment(Qt::AlignRight);
            //item->
            //modd->setItem(z,y,item);
        }
    }

    */
     ui->TWCuentas->show();

     // tuneo algunas cosas mas
     ui->btnAnnadeAbono->setToolTip("Añadir anotación al HABER");
     ui->btnAnnadeCargo->setToolTip("Añadir anotación al DEBE");
}


void fCuentas::onClickTablaDetalles(const QModelIndex & fila){

    if (fila.isValid()) {
           /* Da valor en ID de la celda clickada */
           unsigned int y = fila.row();
           QModelIndex pos = ui->TWCuentas->model()->index(y,0);
           QString cellText = pos.data().toString();
           //qDebug() << "Onclick id = " << cellText;
           IDdetalle = cellText.toInt();
           //qDebug() << "idDetalle: " << IDdetalle;
       }
}




// acciones con los Botones

// elimina linea de detalle seleccionada
void fCuentas::on_btnEliminaDetalle_clicked()
{
    if (IDdetalle){

            QMessageBox::StandardButton resBtn;
            resBtn = QMessageBox::question( this, "FACG",
                                         tr("Desea borrar la linea seleccionada?\n"),
                                         QMessageBox::Yes | QMessageBox::No);
            if (resBtn == QMessageBox::Yes){
                //Eliminar
                eliminaDetalle();
                cargarCuentas();
            }
    }
}

void fCuentas::eliminaDetalle() {
    QSqlQuery query;
    query.prepare("DELETE FROM abonoscargos WHERE id=:id");
    query.bindValue(":id", IDdetalle);
    query.exec();
    QSqlError err = query.lastError();
    if ( err.isValid() ) qDebug() << err.text();

    QSqlDatabase::database().commit();
}

// añadir cargo
void fCuentas::on_btnAnnadeCargo_clicked()
{
    if (IDusuario){
        //qDebug() << "Añade cargo idDetalle: " << IDusuario;
        QString operacion = "CARGO";
        fAddAbonoCargo *fadd = new fAddAbonoCargo(this, &operacion, IDusuario);
        fadd->setModal(true);
        fadd->exec();
        cargarCuentas();

    }
}

// añadir abono
void fCuentas::on_btnAnnadeAbono_clicked()
{
    if (IDusuario){
        //qDebug() << "Añade Abono idDetalle: " << IDusuario;
        QString operacion = "ABONO";
        fAddAbonoCargo *fadd = new fAddAbonoCargo(this, &operacion, IDusuario);
        fadd->setModal(true);
        fadd->exec();
        cargarCuentas();
    }
}

void fCuentas::on_btnExportar_clicked()
{
    //fExportImageCuentasCliente *fexp  = new fExportImageCuentasCliente(this, modeloCuentas);
    //fexp->setModal(true);
    //fexp->show();

    // a ver si se puede hacer directamente de la ventana
    // QPixmap QWidget::grab(const QRect &rectangle = QRect(QPoint(0, 0), QSize(-1, -1)))


    // buaaa como molaaaa
    QPixmap pantallazo(QWidget::grab());
    //QPainter painter(&img);
    //mywidget.render(&painter);

    QString archivo = QFileDialog::getSaveFileName(this,
            "Guardar imagen", nombreClienteTuneado + ".png",
             "Alumno (*.png);;All Files (*)");

    if (!archivo.isNull()) pantallazo.save(archivo);

}
