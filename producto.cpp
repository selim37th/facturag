#include "producto.h"

Producto::Producto()
{
}
Producto::~Producto() {

}
Producto::Producto(unsigned int idpro, QString des, double pre, double iv, QString obs):
    id(idpro), descripcion(des), precio(pre), iva(iv), observaciones(obs) {}


void Producto::setId(unsigned int i){
    id = i;
}
void Producto::setDescripcion(const QString des){
    descripcion = des;
}
void Producto::setImporte(const double imp){
    precio = imp;
}
void Producto::setIva(const double iv){
    iva=iv;
}
void Producto::setObservaciones(const QString obs) {
    observaciones = obs;
}

QString Producto::toString()
{
    QString str = QString("%1 - %2 - %3 - %4 - %5\n").arg(QString::number(id), descripcion, QString::number(precio), QString::number(iva), observaciones);
    return str;
}
