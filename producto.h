#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QObject>
#include <QString>

class Producto
{
public:
    Producto();
    Producto(unsigned int, QString, double, double, QString);
    ~Producto();

    inline unsigned int getId() { return id; }
    inline QString      getDescripcion() { return descripcion; }
    inline double       getImporte() { return precio; }
    inline double       getIva() { return iva; }
    inline QString      getObservaciones() { return observaciones; }

    void setId(unsigned int);
    void setDescripcion(const QString);
    void setImporte(const double);
    void setIva(const double);
    void setObservaciones(const QString);

    QString toString();

private:
    unsigned int id;
    QString descripcion;
    double precio;
    double iva;
    QString observaciones;



};

#endif // PRODUCTO_H
