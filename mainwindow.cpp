#include "mainwindow.h"
#include "conexion.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QtUiTools>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,  ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 // cargar datos del negocio
    cargarDatosNegocio();
    /* Tunear un poco los controles */
    /* tunear calendario*/
    formatoParaEvento.setBackground(Qt::red);
    formatoNoEvento.setBackground(Qt::white);


    QTextCharFormat format;
    format.setForeground(QColor(Qt::black));
    ui->calendario->setWeekdayTextFormat(Qt::Monday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Tuesday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Wednesday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Thursday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Friday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Saturday, format);
    ui->calendario->setWeekdayTextFormat(Qt::Sunday, format);

    // tunear algun boton  que otro
    ui->btnGuardarCambiosClientes->setStyleSheet(::estiloBotonNormal);

    // tunear el label de busqueda
    ui->label->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 0px;");

    // en calendario  poner fecha de hoy
    ui->calendario->setSelectedDate(hoy);


    // meter datos en el qtableview
    QSqlDatabase* DB = createConnection();
    if (DB != NULL) {
        modelo = new QSqlTableModel(this, *DB);
        modelo->setTable("usuarios");
        modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
        modelo->select();
        proxyModelo = new QSortFilterProxyModel(this);
        proxyModelo->setSourceModel(modelo);

        ui->tw->setModel(proxyModelo);

        ui->tw->resizeColumnsToContents();

        // esconder algunas columnas
        ui->tw->setColumnHidden(0, true);
        ui->tw->setColumnHidden(1, true);
        ui->tw->setColumnHidden(2, true);
        ui->tw->setColumnHidden(3, true);

        // tunear un poco la tabla de clientes
        ui->tw->setStyleSheet("QHeaderView::section { border: 1px solid black; font: bold}");
        log("Conectado a : " + QString(_DB));

        // Conectar signals y slots
        connect(ui->btnGuardarCambiosClientes, &QPushButton::clicked, this, &MainWindow::guardaDatosTW);
        connect(ui->btnDeshacerCambios, &QPushButton::clicked,  this, &MainWindow::deshacerCambiosEnClientes);
        //connect(ui->btnDeshacerCambios, &QPushButton::clicked,  modelo, &QSqlTableModel::revertAll);
        connect(ui->tw->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(cambiosEnClientes(const QModelIndex&, const QModelIndex&)));
        //connect(tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
        connect(ui->tw, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClickTablaClientes(const QModelIndex &)));

        // cargar lista eventos
        cargarListaEventos();
        // lanzamos esto para poner el evento de hoy si lo hay

        //on_fechaSeleccionada_dateChanged(hoy);
         ui->fechaSeleccionada->setDate(hoy); // a ver si emite el onchange en fecha Seleccionada

         // boton guardar cambios eventos en azul. si esto lo pongo antes de cargar los eventos
         // se pondrá en rojo cuando aun no he hecho nada or el hecho de si hay
         // algun evento hoy
         ui->btnGuardarDetalleEvento->setStyleSheet(estiloBotonNormal);

    }
    else log("No he podido conectar a la DB se recomienda salir de la aplicación!!");

} // FIN CONSTRUCT MAINWINDOW

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calendario_selectionChanged()
{
    ui->fechaSeleccionada->setDate(ui->calendario->selectedDate());
}


/* Mis func*/
void MainWindow::log(const QString msn ){
    ui->statusbar->showMessage(msn);
}

void MainWindow::guardaDatosTW() {
    modelo->database().transaction();
    if (modelo->submitAll()) {
        modelo->database().commit();
        ui->btnGuardarCambiosClientes->setStyleSheet(estiloBotonNormal);
        haycambios=false;
    } else {
        modelo->database().rollback();
        QMessageBox::warning(this, tr("Tabla usuarios corrompida"),
                             tr("Error: %1")
                             .arg(modelo->lastError().text()));
    }
}


/* Click btn en borrar busqueda */
void MainWindow::on_btnBorrarTxtBuscar_clicked()
{
    ui->txtBuscarPorDNI->setText("");
    ui->txtBuscarPorNombre->setText("");
    ui->txtBuscarPorApellido1->setText("");
    ui->txtBuscarPorApellido2->setText("");
    ui->txtBuscarPorDNI->setFocus();
}

/* slot para cuando se detectan cambios en la qtableview
    avisar de guardar cambios Y cambio color boton GUARDAR
*/
void MainWindow::cambiosEnClientes(const QModelIndex& topLeft, const QModelIndex& bottomRight){
    ui->btnGuardarCambiosClientes->setStyleSheet(::estiloBotonAlerta);
    haycambios = true;
    //qDebug() << "Esta celda ha cambiado: " << topLeft << ", " << bottomRight;
}

void MainWindow::deshacerCambiosEnClientes(){
     //&QSqlTableModel::revertAll;
    modelo->revertAll();
     ui->btnGuardarCambiosClientes->setStyleSheet(::estiloBotonNormal);
     haycambios=false;
}

void MainWindow::on_txtBuscarPorDNI_textChanged(const QString &arg1)
{
    proxyModelo->setFilterKeyColumn(4); // por DNI
    proxyModelo->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString ));
}

void MainWindow::on_txtBuscarPorNombre_textChanged(const QString &arg1)
{
    proxyModelo->setFilterKeyColumn(5); // por nombre
    proxyModelo->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString ));
}

void MainWindow::on_txtBuscarPorApellido1_textChanged(const QString &arg1)
{
    proxyModelo->setFilterKeyColumn(6); // por apellido1
    proxyModelo->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString ));
}

void MainWindow::on_txtBuscarPorApellido2_textChanged(const QString &arg1)
{
    proxyModelo->setFilterKeyColumn(7); // por apellido2
    proxyModelo->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString ));
}

void MainWindow::on_pushButton_5_clicked() // BOTON DE EJEMPLO PARA CARGAR UNA ui
{

    /*  opciones o cargo el UI y hago toda la logica del usuario aqui
    o mejor creo su propia clase .h .cpp y ahí hago toda la logica
    */
    QFile file("formUsuarios.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    QWidget *fusu = loader.load(&file, this);
    file.close();
    fusu->show();
}

void MainWindow::on_btnFichaCliente_clicked()
{
    /* aqui podemos no hacer modal el dialog asi permitir editar varios clientes */
    /* pero metemos el id del cliente en una lista para no poder volver a abrirlo */
    /* hasta que se cierre su qdialog, al sacarlo de la lista clientesAbiertos */

    // si no hay seleccion de cliente no hacemos nada //
    //QItemSelectionModel *select = ui->tw->selectionModel();
    if (ID) {
        // si no tiene ficha abierta abrimos su formulario
        bool esta = false;
        for (unsigned int i: clientesAbiertos){
            if (i == ID) {
                esta = true;
                break; // esta -> salimos
            }
        }
        // cogemos el id del cliente para merterlo en la lista como que ya tiene su
        // ficha abierta su qdialog
        if (!esta) {
            clientesAbiertos.append(ID);

            // dialogo
            fUsuarios *usu = new fUsuarios(this, ID); // al constructor mandamos ID cliente
            usu->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint  | Qt::WindowCloseButtonHint);
            //| Qt::WindowMaximizeButtonHint

            // conectar señal, el qdialog emite cerrandoFicha y pasara el ID del cliente, aqui el slot recibe ID para sacar de la lista
            connect(usu, SIGNAL(cerrandoFicha(unsigned int)),this, SLOT(sacaIdCliente(unsigned int)));
            usu->show();
        }

    }
}

/* slot para cuando cierran el dialogo de cliente sacar de la lista el id
 y asi poder volver a abrir su ficha, el qdialog */
void MainWindow::sacaIdCliente(unsigned int id){
    //qDebug() << "actualizando tabla de clientes por cliente: " << id;
    for (int idx = 0; idx < clientesAbiertos.size(); idx++){
        if (clientesAbiertos[idx] == id){
            clientesAbiertos.removeAt(idx);
            //  saca el id de la lista --- pero mirar si hay cambios para actualizar la TW
            break;
        }
    }
    // actualizamos TW siempre no solo por modificacion de cliente
    // sino tambien por alta de nuevo cliente

    // al salir de ficha cliente actualizamos siembre TW
    modelo->select();
}

void MainWindow::onClickTablaClientes(const QModelIndex &indexx){
    if (indexx.isValid()) {
           /* Da valor en ID de la celda clickada */
           unsigned int y = indexx.row();
           QModelIndex pos = ui->tw->model()->index(y,0);
           QString cellText = pos.data().toString();
           ID = cellText.toInt();
       }
}

void MainWindow::cargarListaEventos() {
    /* Consulta a db para cargar la lista de eventos y meterla en listaEventos */
    QSqlQuery query;

    // podriamos listar todos los eventos o de 8 meses antes y despues por ahorrar vamos
    // ya vemos
    if (query.exec("SELECT * from eventos")) {

        mapaEventos = new QMap<QDate, QString>;

        while (query.next()) {
            QDate f = QDate::fromString(query.value(0).toString(),"yyyy-MM-dd"); // fecha
            mapaEventos->insert( f, query.value(1).toString() );

            // conforme leo los evento tb puedo ir tuneardo el qcalendarwidget -> calendario
            ui->calendario->setDateTextFormat(f, formatoParaEvento);
        }
    }
    query.clear();
}

/*cuando cambia fechaSeleccionada se muestra el texto del evento en txtEventos */
void MainWindow::on_fechaSeleccionada_dateChanged(const QDate &date)
{
    // bloqueo la señal de txtEventos_textChange para que no me cambie el color del boton GUARDAR
    // al cambiar de fecha
    ui->txtEventos->blockSignals(true);

    ui->calendario->setSelectedDate(date);

    if (!mapaEventos->empty()){
        ui->txtEventos->setPlainText(mapaEventos->value(date));

    }

     ui->txtEventos->blockSignals(false);
}

/* Botones para eventos */
void MainWindow::on_txtEventos_textChanged()
{
    ui->btnGuardarDetalleEvento->setStyleSheet(::estiloBotonAlerta);
}

void MainWindow::on_btnGuardarDetalleEvento_clicked()
{

    // si txt evento esta vacio na hacer nada
    if (!ui->txtEventos->toPlainText().isEmpty()) {

        // añado al mapa de eventos. mola por que si ya existe la key no añade :) mas que la lista que teniamos antes
        mapaEventos->insert(ui->fechaSeleccionada->date(), ui->txtEventos->toPlainText());

        // marcar en calendario en rojo
        ui->calendario->setDateTextFormat(ui->fechaSeleccionada->date(), formatoParaEvento);

        // insertar evento en DB
        QSqlQuery query;
        QString fechatxt = ui->fechaSeleccionada->date().toString("yyyy-MM-dd");
        // mirar si existe por si es insert o update
        query.prepare("select * from eventos where fecha=:fecha");
        query.bindValue(":fecha", fechatxt);
        query.exec();
        //int y = query.size(); // joder en sqlite eto no rula no esta soportado!!!!
        // int y;
        // sacar numero rows
        /* if(query.last())
        {
            y =  query.at() + 1;
            query.first();
            query.previous();
        }
        */ // esto me dira el numero de rows

        //if (y<1) {
        if (!query.last()) { // si no hay ultimo insertamos
            query.prepare("INSERT INTO eventos ( fecha , detalle) "
                          "VALUES (:fecha, :detalle)");
        }
        else { // si hay es una actualizacion
            query.prepare("update eventos set detalle=:detalle where fecha=:fecha");
        }

        query.bindValue(":fecha", fechatxt);
        query.bindValue(":detalle", ui->txtEventos->toPlainText());
        query.exec();
        query.clear();
        QSqlDatabase::database().commit();

        // cambio color boton como que se ha guardado
        ui->btnGuardarDetalleEvento->setStyleSheet(::estiloBotonNormal);
    }
}

void MainWindow::on_btnIrAhoy_clicked()
{
    ui->calendario->setSelectedDate(hoy);
}

void MainWindow::on_btnEliminarEvento_clicked()
{
    /* Si hay evento lo eliminamos */
    if (!ui->txtEventos->toPlainText().isEmpty()) {

        // confirmar que quieren eliminar evento
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "FACG", "Desea eliminar las notas de esta fecha?",
                                       QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            // bloqueamos señal para que no cambie de color el boton guardar
           // al detectar cambios en el txtEvento
            ui->txtEventos->blockSignals(true);
           //cambiar la celda de color del calendario
           ui->calendario->setDateTextFormat(ui->fechaSeleccionada->date(), formatoNoEvento);

           // vaciar txxEvento
           ui->txtEventos->setPlainText("");

           // sacar del mapaEventos
           mapaEventos->remove(ui->fechaSeleccionada->date());

           // borrar de la db
           QSqlQuery query;
           QString fechatxt = ui->fechaSeleccionada->date().toString("yyyy-MM-dd");
           query.prepare("DELETE FROM eventos WHERE fecha=:fecha");
           query.bindValue(":fecha", fechatxt);
           query.exec();
           query.clear();
           QSqlDatabase::database().commit();
           ui->txtEventos->blockSignals(false);
         }


    }
}


void MainWindow::on_actionSalir_triggered()
{
    if (haycambios) {
        QMessageBox::StandardButton resBtn = QMessageBox::Yes;

        resBtn = QMessageBox::question( this, "FACG",
                                        tr("Hay cambios en el listado de cliente.\n"
                                           "Desea salir sin guardar?\n"),
                                        QMessageBox::No | QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            QApplication::quit();
        }
    }
    else QApplication::quit();

}

// dialogo nuevo cliente
void MainWindow::on_actionNuevo_triggered()
{
    // dialogo
    fUsuarios *nusu = new fUsuarios(this);
    nusu->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint  | Qt::WindowCloseButtonHint);
    nusu->setModal(true);
    connect(nusu, SIGNAL(cerrandoFicha(unsigned int)),this, SLOT(sacaIdCliente(unsigned int)));
    //| Qt::WindowMaximizeButtonHint
    nusu->show();

}

// eliminar cliente
void MainWindow::on_btnBorrarCliente_clicked()
{
    // ojito con todas las comprobaciones que hay que hacer

    // que no tenga facturas

    // que no tenga pagos

    // examenes, documentacion, etc ...

    // de momento lo borramos y listo .. ya veremos
    if (ID) {
        QMessageBox::StandardButton resBtn = QMessageBox::Yes;
        resBtn = QMessageBox::question( this, "FACG",
                                        tr("¿Desea eliminar este usuario?\n"),
                                        QMessageBox::No | QMessageBox::Yes);
        if (resBtn ==QMessageBox::Yes ) {
            // elimina cliente
            QSqlQuery delc;
            delc.prepare("DELETE FROM usuarios WHERE id=:id");
            delc.bindValue(":id", ID);
            delc.exec();
            QSqlDatabase::database().commit();

            // actualiza TW
            modelo->select();
        }
    }
}

// lanza formulario de creacion de producto
void MainWindow::on_actionCrearProducto_triggered()
{
    fproducto *fp = new fproducto(this);
    fp->setModal(true);
    fp->show();
}

// lanza formulario listado de productos
void MainWindow::on_actionListarProductos_triggered()
{
    fListadoProductos *fpro = new fListadoProductos(this);
    fpro->setModal(true);
    fpro->exec();
    delete(fpro);
}

// Abrir ficha de cuentas del cliente
void MainWindow::on_btnPagosFacturas_clicked()
{
    if (ID) {

        // mirar si el formulario de ctas de ese cliente está abierto ya
        bool estaabierto = false;
        for (unsigned int i: ctasAbiertas){
            if (i == ID) {
                estaabierto = true;
                break; // esta -> salimos
            }
        }

        if (!estaabierto) {
            ctasAbiertas.append(ID);

            // dialogo
            fCuentas *fc = new fCuentas(this, ID);
            // conectar señal, el qdialog emite cerrandoFicha y pasara el ID del cliente, aqui el slot recibe ID para sacar de la lista
            connect(fc, SIGNAL(cerrandoFichaCuenta(unsigned int)),this, SLOT(sacaIdFicha(unsigned int)));
            fc->show();
        }
    }
}

// sacamos el id de la qlist de ficha del cliente para poder volver a abrir
void MainWindow::sacaIdFicha(unsigned int id) {
    for (int idx = 0; idx < ctasAbiertas.size(); idx++){
        if (ctasAbiertas[idx] == id){
            ctasAbiertas.removeAt(idx);
            break;
        }
    }
}

void MainWindow::on_actionDatos_Empresa_triggered()
{
    fDatosEmpresa *fde = new fDatosEmpresa(this, N);
    fde->setModal(true);
    fde->exec();


}

/*
void MainWindow::saveNegocio() {


}
*/
void MainWindow::cargarDatosNegocio(){

    N = new Negocio();
    if (N->loadNegocio()) {
        qDebug()<< "Datos del negocio: " << N->nombreArchivo << " Cargados";
    }
    else {
        log("No se ha podido cargar el archivo de configuración " + N->nombreArchivo);
        QMessageBox msn;
        msn.setText("Error al cargar archivo ini.\nConsulte con soporte.");
        msn.setModal(true);
        msn.exec();
    }
}




