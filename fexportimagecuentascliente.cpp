#include "fexportimagecuentascliente.h"
#include "ui_fexportimagecuentascliente.h"

fExportImageCuentasCliente::fExportImageCuentasCliente(QWidget *parent, QSqlQueryModel *mctas) :
    QDialog(parent),
    ui(new Ui::fExportImageCuentasCliente)
{
    ui->setupUi(this);
    modeloCuentas = mctas;
    //qDebug() << "Filas del modelo " << modeloCuentas->rowCount();


    // pintar los datos ------ uffff que toston --- pero practico para
    // luego enviar por whatsapp a los clientes


    //QPainter p(&pi);

    int ancho = ui->lblDibujo->width();
    int alto = ui->lblDibujo->height();
    qDebug() << "Ancho:" << ancho;
    qDebug() << "alto:" << alto;

    imagen = new QImage(ancho,alto,  QImage::Format_RGB32);
    QPainter p(imagen);

    /* Cuadrito de fondo */
    p.setPen(QColor("#d4d4d4"));    // color del borde
    p.setBrush(QBrush("#ffffff"));  // color del fondo
    p.drawRect(1, 1, ancho-3, alto-3);  // cuadro

    /* Letras */
    QFont font = p.font(); // creamos fuente
    font.setPixelSize(16); // ponemos tamaño
    p.setPen(QColor("#000000")); // color de la fuente
    p.setFont(font); // al painter le asignamos la fuente
    QRect areaLetras = QRect(3, 3, 300, 100); // area de para las letras
    p.drawText(areaLetras, 0, "Fecha emisión: " + QDate::currentDate().toString("dd-MM-yyyy")); // añadimos letras

    // dibujar lineas
    QPen penLineaFina(Qt::black, 2, Qt::SolidLine);
    p.setPen(penLineaFina);
    p.drawLine(1, 26, ancho-3, 26);

    //QRect rect(1, 1, ancho-1, alto-1);
    //p.setRenderHint(QPainter::Antialiasing, true);

    //QFont font = p.font();
    //p.setRenderHint(QPainter::Antialiasing);
    //font.setPixelSize(12);
    //p.setFont(font);

    //QRect rectangle = QRect(0, 0, 100, 100);

    //p.drawText(rectangle, 1, tr("Hello"));



    //p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    //p.drawRect(1,1,100,100);
    //p.drawText(rect, Qt::AlignCenter, tr("Qt by\nThe Qt Company"));

    p.end();


    /*
    p.begin(&pi); el qpainter no hace falta iniciarlo
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(0, 0, 200, 200);
    p.drawRect(0,0,100,100);
    p.end(); // Don't forget this line!
    ui->lblDibujo->setPicture(pi);
*/


    ui->lblDibujo->setPixmap(QPixmap::fromImage(*imagen));

}

fExportImageCuentasCliente::~fExportImageCuentasCliente()
{
    delete ui;
}

void fExportImageCuentasCliente::on_pushButton_clicked()
{

    /*
     * QFile file("ctas.jpg");
    file.open(QIODevice::WriteOnly);

    pi.save(&file, "PNG");
    pi.save("ctas.jpg", "jpg");
    */


    QString archivo = QFileDialog::getSaveFileName(this,
            "Guardar imagen", "",
            "Alumno (*.jpg);;All Files (*)");

    if (!archivo.isNull()) imagen->save(archivo);

}
