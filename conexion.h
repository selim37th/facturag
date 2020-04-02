#ifndef CONEXION_H
#define CONEXION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <fstream>
#include "negocio.h"

static QSqlDatabase db;

static QSqlDatabase* createConnection()
{
    std::ifstream infile(_DB);
    if (infile.good()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(_DB);
        if (!db.open()) {
            QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Cancelar para salir."), QMessageBox::Cancel);
            return NULL;
        }
        return &db;
    }

    QMessageBox::critical(nullptr, QObject::tr("No existe el fichero de la base de datos"),
            QObject::tr("No existe el fichero de la base de datos.\n"
                    "Ponganse en contacto con yo\n\n"
                    "Click Cancel to exit."), QMessageBox::Cancel);
    return NULL;
}

#endif // CONEXION_H
