#include "negocio.h"
#define APP_NAME "FACG"






// devuelve siempre verdadero
// de momento no chekeamos nada
bool Negocio::DNIBueno(const QString& dni){

    // mirar longitud
    //if ((dni == null) || (dni.Length != 9) || (!int.TryParse(dni.Substring(0,8), out n)))
    bool ok = true;
    QString numeros = dni.left(8);

    if ( dni.isEmpty() || dni.length()!=9 || dni.length()!=11 ){
        //numero caracteres incorrecto o vacio
        return ok;
    }
    else {
        // mirar formatos
        // 8 numeros + 1 letra
        // 1 letra + 7 numeros + 1 letra
        //      X -> se desprecia la X
        //      Y -> se cambia por un 1 -> y mismo calculo
        //      Z -> se cambia por un 2 -> y mismo calculo



    }
    // mirar si es dni o nie

    return ok;
}

// dado el numero de nie o dni devuelve la letra
QChar letraDNI(int dni)
{
  return correspondencia[dni%23];
}
////////////////////////////////////////////////////////////////////////////////////
//////////////// DEL PROPIO NEGOCIO ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
Negocio::Negocio() {
    // por defecto ponemos nombre archivo negocio.ini
    nombreArchivo = _archivoNegocio;
}
Negocio::Negocio(const QString arch){
    nombreArchivo = arch;
}

QMap<QString, QString> Negocio::getMapaNegocio() { return mapaNegocio; }


// guardar datos en el archivo ini del negociok
bool Negocio::saveNegocio(){
    bool ok = false;

    if (!mapaNegocio.isEmpty()) {
        QFile file(nombreArchivo);
        QTextStream out(&file);
        if(!file.open(QIODevice::WriteOnly))
        {
                qDebug() << "No se puede abrir " << nombreArchivo;
                ok = false;
        }
        else {
            QMap<QString, QString>::iterator i;
            for (i = mapaNegocio.begin(); i != mapaNegocio.end(); ++i){
                //qDebug() << i.key() << " : " << i.value();
                QString linea = i.key() + "=" + i.value() + "\n";
                out << linea;

            }
            file.flush();
            ok = true;
        }
        file.close();
    }
    else {
        qDebug() << "Mapa negocio vacio!!";
        ok=false;
    }

    return ok;
}

bool Negocio::saveToFile(const QString arch){
    nombreArchivo = arch;
    return saveNegocio();
}

// cargar datos del archivo y meterlos en el qmap
bool Negocio::loadNegocio(){
    bool ok = false;

    QFile file(nombreArchivo);

    if(!file.open(QIODevice::ReadOnly))
    {
            qDebug() << "Could not open " << nombreArchivo;
            ok=false;
    }

    while (!file.atEnd()) {
        QString lineaOriginal = file.readLine();
        lineaOriginal.remove("\r\n");
        QString lineaTest = lineaOriginal;
        // si la linea empieza por ; es un comentario

        // quitar espacios
        lineaTest.remove(" ");
        if (lineaTest.left(1) != ";") // es comentario saltamos
        {    // en la linea tiene que hacer un "="
                int nigual = lineaTest.count("=");
                if (nigual == 1) { // un solo = -> si mas lo descartamos

                    // split por el =
                    QStringList keyvalues = lineaOriginal.split("=");

                    mapaNegocio.insert(keyvalues[0],  keyvalues[1]);
                    //qDebug() << "Key:" << keyvalues[0] << " Valor:" << keyvalues[1];

                }
        }

    }
    file.close();

    if (mapaNegocio.isEmpty()) ok = false;
    else ok = true;
    return ok;

}

// cargamos el ini del negocio desde un archivos especifico
// ojo que ya no trabajaremos desde el archivo por defecto
// sino desde el que yo especifique aqui
bool Negocio::loadFromFile(const QString arch){
    nombreArchivo = arch;
    return loadNegocio();

}


