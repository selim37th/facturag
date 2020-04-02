#include "fusuarios.h"
#include "ui_fusuarios.h"



fUsuarios::fUsuarios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fUsuarios)
{
    // contructor nuevo usuario
    ui->setupUi(this);
    idcliente = 0; // idcliente a 0 -> nuevo

    // tunear algun boton y cosas
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");
    setWindowTitle("Nuevo Cliente");

    ui->txtFechaAlta->setDate(QDate::currentDate());

    ui->tabWidget->setTabEnabled(1,false); // al crear usuario deshabilitamos tab documentacion

    // crear signal and slot para cuando cierren este dialogo actualizar TW
    // añadir signal a todos los txt por si hay cambios
    cargarSignalOnChange();

}

fUsuarios::fUsuarios(QWidget *parent, unsigned int id) :
    QDialog(parent),
    ui(new Ui::fUsuarios)
{
    // constructor editar usuario
    ui->setupUi(this);
    idcliente = id;

    // Mostrar los datos del cliente
    cargarDatosCliente();

    // añadir signal y slots a todos los txt por si hay cambios
    cargarSignalOnChange();

    // tunear algun boton
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");
}

fUsuarios::~fUsuarios()
{
    delete ui;
}

void fUsuarios::setIdCliente(const unsigned int id){
    idcliente = id;
}
unsigned int fUsuarios::getIdCliente(){
    return idcliente;
}


/*void fUsuarios::closeEvent(QCloseEvent *){
    qDebug() << "Cerrando dialog";
}
*/
void fUsuarios::reject(){ // tambien para close - CERRAR -

    if (haycambios) {
        QMessageBox::StandardButton resBtn;
        //qDebug() << "hago reject";
        resBtn = QMessageBox::question( this, "FACG",
                                        tr("Se han producido cambios, antes de salir\ndesea GUARDARLOS?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            // mirar campos, guardar y salir
            if (checkCampos()) {
                guardarDatosCliente();
                emit cerrandoFicha(idcliente);
                QDialog::reject();
            }
        }

        if (resBtn == QMessageBox::No) {
            //  salir
            emit cerrandoFicha(idcliente);
            QDialog::reject();
        }

        if (resBtn == QMessageBox::Cancel) {
            //  No salir

        }
    }
    else {
        // no hay cambios salimos
        emit cerrandoFicha(idcliente);
        QDialog::reject();
    }

}

void fUsuarios::accept(){ // tambien para SAVE - GUARDAR -

     // si cambios
     if (haycambios) {

        QMessageBox::StandardButton resBtn;
         //check campos
        if (checkCampos()) {
             if (idcliente!=0)
                resBtn = QMessageBox::question( this, "FACG",
                                             tr("Se han producido cambios, desea GUARDARLOS?\n"),
                                             QMessageBox::No | QMessageBox::Yes);
             else
                 resBtn = QMessageBox::question( this, "FACG",
                                              tr("Desea crear este cliente?\n"),
                                              QMessageBox::No | QMessageBox::Yes);

             if (resBtn == QMessageBox::Yes) {
                // guardar cambios
                guardarDatosCliente();
             }
        }
     }
}

/* Cargar datos del formulario */
void fUsuarios::cargarDatosCliente(){

    // cargar datos de la DB
    QSqlQuery qselect;
    qselect.prepare("select * from usuarios where id=:id");
    qselect.bindValue(":id", idcliente);
    qselect.exec();

    if (qselect.next()){
        // QDate f = QDate::fromString(query.value(0).toString(),"yyyy-MM-dd");
        setWindowTitle(qselect.value(6).toString() + " " + qselect.value(7).toString() + ", " + qselect.value(5).toString() + ". " + qselect.value(4).toString());

        ui->txtDni->setText(qselect.value(4).toString());
        ui->txtApellido1->setText(qselect.value(6).toString());
        ui->txtApellido2->setText(qselect.value(7).toString());
        ui->txtNombre->setText(qselect.value(5).toString());
        ui->txtNombre->setText(qselect.value(5).toString());

        ui->txtTelefono->setText(qselect.value(12).toString());
        ui->txtEmail->setText(qselect.value(14).toString());
        ui->txtWeb->setText(qselect.value(15).toString());
        ui->txtMovil->setText(qselect.value(13).toString());

        //QString strFecha = qselect.value(8).toString();
        QDate fn = QDate::fromString(qselect.value(8).toString(),"yyyy-MM-dd");
        ui->txtFechaNacimiento->setDate(fn);

        ui->txtLugarNacimiento->setText(qselect.value(9).toString());
        ui->txtNacionalidad->setText(qselect.value(11).toString());

        int sex = qselect.value(10).toInt(); // 1 mujer, 0 hombre
        ui->txtSexo->setCurrentIndex(sex);

        //strFecha = qselect.value(32).toString();
        fn = QDate::fromString(qselect.value(32).toString(),"yyyy-MM-dd");
        ui->txtFechaAlta->setDate(fn);

        ui->txtCodigoPostal->setText(qselect.value(20).toString());
        ui->txtLocalidad->setText(qselect.value(17).toString());
        ui->txtMunicipio->setText(qselect.value(18).toString());
        ui->txtProvincia->setText(qselect.value(19).toString());
        ui->txtTipoVia->setText(qselect.value(21).toString());
        ui->txtNombreVia->setText(qselect.value(22).toString());
        ui->txtNumero->setText(qselect.value(23).toString());
        ui->txtPlanta->setText(qselect.value(26).toString());
        ui->txtPuerta->setText(qselect.value(27).toString());
        ui->txtBloque->setText(qselect.value(24).toString());
        ui->txtPortal->setText(qselect.value(25).toString());
        ui->txtEscalera->setText(qselect.value(28).toString());
        ui->txtKm->setText(qselect.value(29).toString());
        ui->txtObservaciones->setPlainText(qselect.value(33).toString());

    }
    else // error en la select
    {
        //Dialogo grandote para mostrar el error
    }

}

// conecta todos los campos editable del formulario
// con el slot cambio en ficha
void fUsuarios::cargarSignalOnChange(){

    // conectar todos los QLineEdit
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>()) {
        connect(le, SIGNAL(textChanged(const QString &)), this, SLOT(CambiosEnFichaCliente(const QString &)));
    }

    // conectar los campos de fecha
    connect(ui->txtFechaAlta, SIGNAL(dateChanged(const QDate &)), this, SLOT(CambiosEnFichaCliente(const QDate &)));
    connect(ui->txtFechaNacimiento, SIGNAL(dateChanged(const QDate &)), this, SLOT(CambiosEnFichaCliente(const QDate &)));

    // conectar combo del sexo
    connect(ui->txtSexo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(cambioTxtSexo(const QString &)));

    // conectar el plaintText Edit de observaciones
    connect(ui->txtObservaciones, SIGNAL(textChanged()), this, SLOT(CambiosEnFichaCliente()));

}

// slots cuando se detecta aalgun cambio en la ficha del cliente, en algun cambio
// estaria chulo que cambie el color del campo el que que hayan cambios ---------------------------------------------> MIRAR
void fUsuarios::CambiosEnFichaCliente(const QString &nuevoValor){
    /* intentamos dejar cambiado el fondo */
    QObject *object = sender();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(object);
    if (lineEdit) {
        lineEdit->setStyleSheet("background-color: orange");
    }
    haycambios = true;
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: red; color: white");
}

void fUsuarios::CambiosEnFichaCliente(const QDate &nuevFecha){
    QObject *object = sender();
    QDateEdit *date = static_cast<QDateEdit*>(object);
    if (date) {
        date->setStyleSheet("background-color: orange");
    }
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: red; color: white");
    haycambios = true;
}

//para el text plain
void fUsuarios::CambiosEnFichaCliente(){
    QObject *object = sender();
    QPlainTextEdit *ptext = static_cast<QPlainTextEdit*>(object);
    if (ptext) {
        ptext->setStyleSheet("background-color: orange");
    }

    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: red; color: white");
    haycambios = true;

}

// Para el combo sexo slot
void fUsuarios::cambioTxtSexo(const QString &arg1){
    ui->txtSexo->setStyleSheet("background-color: orange");
    ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: red; color: white");
    haycambios = true;
}

void fUsuarios::guardarDatosCliente()
{
        QSqlQuery query;
        if (idcliente!=0) {
            // actualizar datos cliente
            query.prepare("update usuarios set nombre=:nombre,"
                          "dni=:dni, "
                          "apellido1=:apellido1, "
                          "apellido2=:apellido2, "
                          "poblacionNacimiento=:pon, "
                          "fechaNacimiento=:fn, "
                          "sexo=:sex,"
                          "nacionalidad=:nac,"
                          "fechaAlta=:fa,"
                          "telefono=:tel,"
                          "movil=:mov,"
                          "mail=:mail,"
                          "web=:web,"
                          "codigoPostal=:cp,"
                          "localidad=:loc,"
                          "municipio=:mun,"
                          "provincia=:pro,"
                          "tipoVia=:tipov,"
                          "nombreVia=:nombrev,"
                          "numero=:numero,"
                          "planta=:planta,"
                          "puerta=:puerta,"
                          "bloque=:bloque,"
                          "portal=:portal,"
                          "escalera=:esc,"
                          "km=:km,"
                          "observaciones=:obs "
                          " where id=:id");

                        query.bindValue(":id", idcliente);
        }
        else
        {
            // alta nuevo cliente
            query.prepare("INSERT INTO usuarios("
                          "nombre, dni, apellido1, apellido2, poblacionNacimiento, fechaNacimiento, sexo, nacionalidad, "
                          "fechaAlta, telefono, movil, mail, web, codigoPostal, localidad, municipio, provincia, tipoVia, "
                          "nombreVia, numero, planta, puerta, bloque, portal, escalera, km, observaciones"
                          ") VALUES (:nombre, :dni, :apellido1, :apellido2, :pon, :fn, :sex, :nac, :fa, :tel, :mov, :mail,"
                          " :web, :cp, :loc, :mun, :pro, :tipov, :nombrev, :numero, :planta, :puerta, :bloque, :portal, "
                          ":esc, :km, :obs)");

        }
        query.bindValue(":nombre", ui->txtNombre->text());
        query.bindValue(":dni", ui->txtDni->text());

        query.bindValue(":apellido1", ui->txtApellido1->text());
        query.bindValue(":apellido2", ui->txtApellido2->text());

        QString fn = ui->txtFechaNacimiento->date().toString("yyyy-MM-dd");
        query.bindValue(":fn", fn);
        query.bindValue(":pon", ui->txtLugarNacimiento->text());

        query.bindValue(":sex", ui->txtSexo->currentIndex());
        query.bindValue(":nac", ui->txtNacionalidad->text());

        QString fa = ui->txtFechaAlta->date().toString(("yyyy-MM-dd"));
        query.bindValue(":fa", fa);

        query.bindValue(":tel", ui->txtTelefono->text());
        query.bindValue(":mov", ui->txtMovil->text());
        query.bindValue(":mail", ui->txtEmail->text());
        query.bindValue(":web", ui->txtWeb->text());

        query.bindValue(":cp", ui->txtCodigoPostal->text());
        query.bindValue(":loc", ui->txtLocalidad->text());
        query.bindValue(":mun", ui->txtMunicipio->text());
        query.bindValue(":pro", ui->txtProvincia->text());
        query.bindValue(":tipov", ui->txtTipoVia->text());
        query.bindValue(":nombrev", ui->txtNombreVia->text());
        query.bindValue(":numero", ui->txtNumero->text());
        query.bindValue(":planta", ui->txtPlanta->text());
        query.bindValue(":puerta", ui->txtPuerta->text());
        query.bindValue(":bloque", ui->txtBloque->text());
        query.bindValue(":portal", ui->txtPortal->text());
        query.bindValue(":esc", ui->txtEscalera->text());
        query.bindValue(":km", ui->txtKm->text());
        query.bindValue(":obs", ui->txtObservaciones->toPlainText());

        query.exec();
        QSqlDatabase::database().commit();

        //qDebug()<< query.lastQuery();

        // por tema de poder meter ya documentacion sin cerrar la ficha del cliente
        // metemos ya el idcliente si es un alta
        if (idcliente == 0) {
            query.clear();
            query.prepare("SELECT id FROM usuarios ORDER BY id DESC LIMIT 1");
            query.exec();
            if (query.last()) {
                idcliente = query.value(0).toInt();

                // habilitamos tab de documentacion
                ui->tabWidget->setTabEnabled(1,true);
            }
        }
        QSqlDatabase::database().commit();

       // cambiamos boton guardar de color, ya no hay cambios y devolvemos color a los camps
        ui->buttonBox->button(QDialogButtonBox::Save)->setStyleSheet("background-color: blue; color: white");
        haycambios=false;
        devolverColorClampos();
}

// devuelve el color normal a los campos que han sido modificados en la ficha del cliente
// lo hacemos contodos por no complicarnos
void fUsuarios::devolverColorClampos() {
    foreach(QLineEdit* le, this->findChildren<QLineEdit*>()) {
        le->setStyleSheet(QString());
    }
    ui->txtFechaAlta->setStyleSheet(QString());
    ui->txtFechaNacimiento->setStyleSheet(QString());
    ui->txtSexo->setStyleSheet(QString());
    ui->txtObservaciones->setStyleSheet(QString());

}

/* check de campos criticos */
bool fUsuarios::checkCampos(){
    bool todook = true;

    if (ui->txtDni->text().isEmpty()) {
        todook = false;
        QMessageBox::critical(0,"Error","No debe dejar el DNI en blanco");
    }

    if (!Negocio::DNIBueno(ui->txtDni->text())){
        todook = false;
        QMessageBox::critical(0,"Error","El DNI/NIF es incorrecto");
    }

    if (ui->txtNombre->text().isEmpty()) {
        todook=false;
        QMessageBox::critical(0,"Error","No debe dejar el NOMBRE en blanco");
    }

    if (ui->txtApellido1->text().isEmpty()) {
        todook=false;
        QMessageBox::critical(0,"Error","No debe dejar el PRIMER APELLIDO en blanco");
    }

    return todook;
}
