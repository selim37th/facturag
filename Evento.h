#ifndef EVENTO_H
#define EVENTO_H

#include <QDateTime>
#include <QString>

class Evento
{

public:
    Evento( const QDate f, const QString de );
    ~Evento();

    inline QDate fecha() const {return _fecha;};
    void setFecha(const QDate &fechaini);

    inline QString detalle() const {return _detalle;};
    void setDetalle(const QString &detalle);

private:
    QDate _fecha;
    QString _detalle;

};

#endif // EVENTO_H
