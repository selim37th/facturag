#include "fdatosempresa.h"
#include "ui_fdatosempresa.h"

fDatosEmpresa::fDatosEmpresa(QWidget *parent, Negocio *n) :
    QDialog(parent),
    ui(new Ui::fDatosEmpresa)
{
    ui->setupUi(this);
    N=n;

    //qDebug() << N->getMapaNegocio.value("fiscalRazonSocial");

    // cargar datos del negocio en el formulario
    if (!N->mapaNegocio.isEmpty()) {
        ui->txtFiscalesNombreComercial->setText(N->mapaNegocio["fiscalNombreComercial"]);
        ui->txtFiscalesRazonSocial->setText(N->mapaNegocio["fiscalRazonSocial"]);
        ui->txtFiscalesNIF->setText(N->mapaNegocio["fiscalNIF"]);
        ui->txtFiscalesDomicilio->setText(N->mapaNegocio["fiscalDomicilio"]);
        ui->txtFiscalesCodigoPostal->setText(N->mapaNegocio["fiscalCodigoPostal"]);
        ui->txtFiscalesPoblacion->setText(N->mapaNegocio["fiscalPoblacion"]);
        ui->txtFiscalesProvincia->setText(N->mapaNegocio["fiscalProvincia"]);
        ui->txtFiscalesTelefono->setText(N->mapaNegocio["fiscalTelefono"]);
        ui->txtFiscalesFax->setText(N->mapaNegocio["fiscalFax"]);

        ui->txtAutoNombreComercial->setText(N->mapaNegocio["autoescuelaNombreComercial"]);
        ui->txtAutoCodigoProvincial->setText(N->mapaNegocio["autoescuelaCodigoProvincial"]);
        ui->txtAutoSeccion->setText(N->mapaNegocio["autoescuelaSeccion"]);
        ui->txtAutoDigitoControl->setText(N->mapaNegocio["autoescuelaDigitoControl"]);
        ui->txtAutoRazonSocial->setText(N->mapaNegocio["autoescuelaRazonSocial"]);
        ui->txtAutoNIF->setText(N->mapaNegocio["autoescuelaNIF"]);
        ui->txtAutoDomicilio->setText(N->mapaNegocio["autoescuelaDomicilio"]);
        ui->txtAutoCodigoPostal->setText(N->mapaNegocio["autoescuelaCodigoPostal"]);
        ui->txtAutoPoblacion->setText(N->mapaNegocio["autoescuelaPoblacion"]);
        ui->txtAutoProvincia->setText(N->mapaNegocio["autoescuelaProvincia"]);
        ui->txtAutoTelefono->setText(N->mapaNegocio["autoescuelaTelefono"]);
        ui->txtAutoEmail->setText(N->mapaNegocio["autoescuelaEmail"]);
        ui->txtAutoMovil->setText(N->mapaNegocio["autoescuelaMovil"]);
        ui->txtAutoNombreTitular->setText(N->mapaNegocio["autoescuelaNombreTitular"]);
        ui->txtAutoDirector->setText(N->mapaNegocio["autoescuelaNombreDirector"]);
        ui->txtAutoDirectorNIF->setText(N->mapaNegocio["autoescuelaNIFDirector"]);
        ui->txtAutoLocalidadExamen->setText(N->mapaNegocio["autoescuelaLocalidadExamen"]);
    }

    // tunear algunas cosillas
    cargarSignalOnChange();
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");
}

fDatosEmpresa::~fDatosEmpresa()
{
    delete ui;
}


void fDatosEmpresa::accept() { // SAVE
    // save
    if (haycambios){

         //check campos
        if (checkCampos()) {
            QMessageBox::StandardButton resBtn;
            resBtn = QMessageBox::question( this, "FACG",
                                         tr("Se han producido cambios, desea GUARDARLOS?\n"),
                                         QMessageBox::No | QMessageBox::Yes);
            if (resBtn == QMessageBox::Yes) {
               // guardar cambios
               guardarDatosEmpresa();
               quitarColor();
            }
        }
    }
}
void fDatosEmpresa::reject() { // CANCEL
    if (haycambios) {
        QMessageBox::StandardButton resBtn;
        resBtn = QMessageBox::question( this, "FACG",
                                     tr("Se han producido cambios,\nantes de salir desea GUARDARLOS ?\n"),
                                     QMessageBox::No | QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
           // guardar cambios
           guardarDatosEmpresa();

        }
    }
    QDialog::reject();
}


void fDatosEmpresa::cargarSignalOnChange(){
    // conectar todos los QLineEdit
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>()) {
        connect(le, SIGNAL(textChanged(const QString &)), this, SLOT(CambiosEnDatosEmpresa(const QString &)));
    }

}

void fDatosEmpresa::CambiosEnDatosEmpresa(const QString &cadena){
    /* intentamos dejar cambiado el fondo */
    QObject *object = sender();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(object);
    if (lineEdit) {
        lineEdit->setStyleSheet("background-color: orange");
    }
    haycambios = true;
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: red; color: white");
}


void fDatosEmpresa::quitarColor() {
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
            le->setStyleSheet(QString());
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");
}


bool fDatosEmpresa::checkCampos(){
    bool ok = true;
    if (ui->txtFiscalesRazonSocial->text().isEmpty()) {
        ok=false;
        QMessageBox::critical(0,"Error","No debe dejar el campo Fiscal:\nRazón Social en blanco");
        ui->txtFiscalesRazonSocial->setFocus();
    }

    if (ui->txtFiscalesNIF->text().isEmpty()) {
        ok=false;
        QMessageBox::critical(0,"Error","No debe dejar el campo Fiscal:\nN.I.F. en blanco");
        ui->txtFiscalesNIF->setFocus();
    }

    if (ui->txtFiscalesCodigoPostal->text().isEmpty()) {
        ok=false;
        QMessageBox::critical(0,"Error","No debe dejar el campo Fiscal:\nCódigo Postal en blanco");
        ui->txtFiscalesCodigoPostal->setFocus();
    }

    return ok;
}

void fDatosEmpresa::guardarDatosEmpresa(){

    N->mapaNegocio["fiscalNombreComercial"] = ui->txtFiscalesNombreComercial->text();
    N->mapaNegocio["fiscalRazonSocial"] = ui->txtFiscalesRazonSocial->text();
    N->mapaNegocio["fiscalNIF"] = ui->txtFiscalesNIF->text();
    N->mapaNegocio["fiscalDomicilio"] = ui->txtFiscalesDomicilio->text();
    N->mapaNegocio["fiscalCodigoPostal"] = ui->txtFiscalesCodigoPostal->text();
    N->mapaNegocio["fiscalPoblacion"] = ui->txtFiscalesPoblacion->text();
    N->mapaNegocio["fiscalProvincia"] = ui->txtFiscalesProvincia->text();
    N->mapaNegocio["fiscalTelefono"] = ui->txtFiscalesTelefono->text();
    N->mapaNegocio["fiscalFax"] = ui->txtFiscalesFax->text();

    N->mapaNegocio["autoescuelaNombreComercial"] = ui->txtAutoNombreComercial->text();
    N->mapaNegocio["autoescuelaCodigoProvincial"] = ui->txtAutoCodigoProvincial->text();
    N->mapaNegocio["autoescuelaSeccion"] = ui->txtAutoSeccion->text();
    N->mapaNegocio["autoescuelaDigitoControl"] = ui->txtAutoDigitoControl->text();
    N->mapaNegocio["autoescuelaRazonSocial"] = ui->txtAutoRazonSocial->text();
    N->mapaNegocio["autoescuelaNIF"] = ui->txtAutoNIF->text();
    N->mapaNegocio["autoescuelaDomicilio"] = ui->txtAutoDomicilio->text();
    N->mapaNegocio["autoescuelaCodigoPostal"] = ui->txtAutoCodigoPostal->text();
    N->mapaNegocio["autoescuelaPoblacion"] = ui->txtAutoPoblacion->text();
    N->mapaNegocio["autoescuelaProvincia"] = ui->txtAutoProvincia->text();
    N->mapaNegocio["autoescuelaTelefono"] = ui->txtAutoTelefono->text();
    N->mapaNegocio["autoescuelaEmail"] = ui->txtAutoEmail->text();
    N->mapaNegocio["autoescuelaMovil"] = ui->txtAutoMovil->text();
    N->mapaNegocio["autoescuelaNombreTitular"] = ui->txtAutoNombreTitular->text();
    N->mapaNegocio["autoescuelaNombreDirector"] = ui->txtAutoDirector->text();
    N->mapaNegocio["autoescuelaNIFDirector"] = ui->txtAutoDirectorNIF->text();
    N->mapaNegocio["autoescuelaLocalidadExamen"] = ui->txtAutoLocalidadExamen->text();

    N->saveNegocio();

    haycambios = false;

}
