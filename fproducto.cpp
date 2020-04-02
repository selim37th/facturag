#include "fproducto.h"
#include "ui_fproducto.h"
#include <QLocale>

fproducto::fproducto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fproducto)
{

    ui->setupUi(this);



    //tunear boton save
     ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonNormal);



    // cargar signal/slots
    cargarSignalOnChange();

    idproducto = 0;



}

fproducto::fproducto(QWidget *parent, Producto *p):
    QDialog(parent),
    ui(new Ui::fproducto)
{

    ui->setupUi(this);



    //tunear boton save
     ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");

    idproducto = p->getId();
    ui->txtDescripcion->setText(p->getDescripcion());

    ui->txtImporte->setValue(p->getImporte());
    //ui->txtIvaPor->setText(QString("%L0").arg(p->getIva(), 0, 'f', 0));
    ui->txtIvaPor->setValue(p->getIva());
    ui->txtObservaciones->setPlainText(p->getObservaciones());
    calculaImportes();


    cargarSignalOnChange(); // cargamos signals despues de cargar los datos
                            // porque asi no se modifica ni el formato del boton
                            // ni el hay cambios

    haycambios = false;


}

fproducto::~fproducto()
{
    delete ui;
    haycambios = false;


}


void fproducto::cargarSignalOnChange(){
    // conectar todos los QLineEdit
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>()) {
        connect(le, SIGNAL(textChanged(const QString &)), this, SLOT(CambiosEnFichaProducto(const QString &)));
    }
    foreach(QDoubleSpinBox* spb, this->findChildren<QDoubleSpinBox*>()) {
        connect(spb, SIGNAL(textChanged(const QString &)), this, SLOT(CambiosEnFichaProducto(const QString &)));
    }

    // conectar el plaintText Edit de observaciones
    connect(ui->txtObservaciones, SIGNAL(textChanged()), this, SLOT(CambiosEnFichaProducto()));

}


void fproducto::CambiosEnFichaProducto(const QString &){
    QObject *object = sender();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(object);
    if (lineEdit) {
        lineEdit->setStyleSheet("background-color: orange");
    }

    QDoubleSpinBox *dspb = static_cast<QDoubleSpinBox*>(object);
    if (dspb) {
        dspb->setStyleSheet("background-color: orange");
        calculaImportes();
    }
    haycambios = true;
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonAlerta);
}


void fproducto::CambiosEnFichaProducto(){
    QObject *object = sender();
    QPlainTextEdit *ptext = static_cast<QPlainTextEdit*>(object);
    if (ptext) {
        ptext->setStyleSheet("background-color: orange");
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonAlerta);
    haycambios = true;
}


void fproducto::reject(){ //////////////////////// CLOSE
    if (haycambios) {
        QMessageBox::StandardButton resBtn;

        resBtn = QMessageBox::question( this, "FACG",
                                        tr("Se han producido cambios, antes de salir\ndesea GUARDARLOS?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            // mirar campos, guardar y salir
            if (checkCampos()) {
                guardarProducto();
                QDialog::reject();
            }
        }

        if (resBtn == QMessageBox::No) {
            //  salir

            QDialog::reject();
        }

        if (resBtn == QMessageBox::Cancel) {
            //  No salir

        }
    }
    else {
        // no hay cambios salimos

        QDialog::reject();
    }

}

void fproducto::accept(){ ///////////////////////// SAVE
    // si cambios
    if (haycambios) {

       QMessageBox::StandardButton resBtn;
        //check campos
       if (checkCampos()) {
            if (idproducto!=0)
               resBtn = QMessageBox::question( this, "FACG",
                                            tr("Se han producido cambios, desea GUARDARLOS?\n"),
                                            QMessageBox::No | QMessageBox::Yes);
            else
                resBtn = QMessageBox::question( this, "FACG",
                                             tr("Desea guardar este producto?\n"),
                                             QMessageBox::No | QMessageBox::Yes);

            if (resBtn == QMessageBox::Yes) {
               // guardar cambios
               guardarProducto();
            }
       }
    }
}


void fproducto::guardarProducto(){

     if (checkCampos()) {
        QSqlQuery query;

        // insertando nuevo producto
        if (idproducto == 0) { // es un alta de producto
            query.prepare("INSERT INTO productos (descripcion, precio, impuesto, observaciones) values ("
                          ":des, :pre, :imp, :obser)");
        }
        else {
            // es una actualización
            query.prepare("UPDATE productos set "
                          "descripcion = :des, "
                          "precio = :pre, "
                          "impuesto = :imp,"
                          "observaciones = :obser where "
                          "id = :id");
            query.bindValue(":id", idproducto);
        }
            query.bindValue(":des", ui->txtDescripcion->text());
            query.bindValue(":pre", importe);
            query.bindValue(":imp", ivapor);
            query.bindValue(":obser", ui->txtObservaciones->toPlainText());
            query.exec();
            QSqlDatabase::database().commit();

            query.clear();
            // si era un alta recuperamos el ultimo id insertado

            if (idproducto == 0) {
                query.prepare("SELECT id FROM productos ORDER BY id DESC LIMIT 1");
                query.exec();
                if (query.last()) {
                    idproducto = query.value(0).toInt();

                }
            }
        }

         haycambios = false;
         // devolver color a los campos
         devolverColorCampos();

}

bool fproducto::checkCampos(){
    bool ok = true;

    if (ui->txtDescripcion->text().isEmpty()) {
         QMessageBox::critical(0,"Error","No debe dejar la DESCRIPCIÓN DEL PRODUCTO en blanco");
         ok = false;
    }

    return ok;
}


void fproducto::devolverColorCampos() {
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>()) {
        le->setStyleSheet(QString());
    }
    ui->txtObservaciones->setStyleSheet(QString());

    // azul boton guardar
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet(::estiloBotonNormal);
}

// calcula el importe total segun campos
void fproducto::calculaImportes(){
    // bloquea signals
    ui->txtIvaImp->blockSignals(true);
    ui->txtPrecio->blockSignals(true);
    if (ui->txtImporte->value() != 0.0) {
        importe = ui->txtImporte->value();
        if (ui->txtIvaPor->value() != 0.0 ) {
            ivapor = ui->txtIvaPor->value();
            ivaimpor = (importe * ivapor)/100;
            ui->txtIvaImp->setValue(ivaimpor);
            total = ivaimpor + importe;
            ui->txtPrecio->setValue(total);
        }
    }
    else {
        ui->txtPrecio->setValue(0.0);
        ui->txtIvaImp->setValue(0.0);
    }
    //desbloquea señales
    ui->txtIvaImp->blockSignals(false);
    ui->txtPrecio->blockSignals(false);
}
