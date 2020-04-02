#include "Evento.h"

Evento::Evento( QDate fecha, QString detalle )
{
    Evento::_fecha = fecha;
    Evento::_detalle = detalle;
}

Evento::~Evento() { }
void Evento::setFecha(const QDate &fechaini){ _fecha = fechaini; }
void Evento::setDetalle(const QString &detalle) { _detalle = detalle; }
