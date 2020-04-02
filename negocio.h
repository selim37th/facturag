#ifndef NEGOCIO_H
#define NEGOCIO_H
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QString>
#include <QMap>

#define _SGDB "sqlite3"
#define _DB "autoescuela.db"
#define _tusuarios "usuarios"
#define _tproductos "productos"
#define _teventos "eventos"
#define _tfacturas "facturas"
#define _tdetallesFactura "detallesFactura"
#define _archivoNegocio "negocio.ini"

// constantes para la interfaz
const QString estiloBotonAlerta = "color: white; background-color: red";
const QString estiloBotonNormal = "color: white; background-color: blue";

static double IVADEFAULT = 21.00;

// constantes para el negocio
const QString correspondencia = "TRWAGMYFPDXBNJZSQVHLCKE";

/* Clase negocio :
no defino miembros de clase solo hay un map key->value
con las variables que se definen en un fichero
de pruebas tenemos un negocio.ini
*/
class Negocio
{
public:
    Negocio(); // Constructor por defecto
    Negocio(QString nombreArchivo);

    QMap <QString, QString> mapaNegocio;
    QString nombreArchivo;
    QMap<QString, QString> getMapaNegocio();




    bool saveNegocio();
    bool saveToFile(const QString);

    bool loadNegocio();
    bool loadFromFile(const QString);




    // funciones auxiliares
    static bool DNIBueno( const QString& );



private:


};




#endif // NEGOCIO_H
