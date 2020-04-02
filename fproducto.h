#ifndef FPRODUCTO_H
#define FPRODUCTO_H

#include <QDialog>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDoubleSpinBox>

#include "negocio.h"
#include "producto.h"

namespace Ui {
class fproducto;
}

class fproducto : public QDialog
{
    Q_OBJECT

public:
    explicit fproducto(QWidget *parent = nullptr);
    explicit fproducto(QWidget *parent, Producto *p);
    ~fproducto();

private:
    Ui::fproducto *ui;

    void cargarSignalOnChange();
    void guardarProducto();
    bool checkCampos();
    void devolverColorCampos();
    void accept() override;
    void reject() override;
    void calculaImportes();

    unsigned int idproducto = 0;
    bool haycambios;
    // Para conversion entre numeros y calculos
    bool conversionOK;
    double importe=0.0;
    double ivapor=0.0;
    double ivaimpor=0.0;
    double total=0.0;



private slots:
    void CambiosEnFichaProducto(const QString &); // para los QLineEdit
    void CambiosEnFichaProducto();
};

#endif // FPRODUCTO_H
