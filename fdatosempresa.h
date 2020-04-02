#ifndef FDATOSEMPRESA_H
#define FDATOSEMPRESA_H

#include "negocio.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class fDatosEmpresa;
}

class fDatosEmpresa : public QDialog
{
    Q_OBJECT

public:
    explicit fDatosEmpresa(QWidget *parent = nullptr, Negocio *n = nullptr);
    ~fDatosEmpresa();
    void accept() override;
    void reject() override;

private:
    Ui::fDatosEmpresa *ui;
    Negocio *N;
    Negocio *NPrueba;
    void cargarSignalOnChange();
    void quitarColor();
    bool haycambios = false;
    bool checkCampos();
    void guardarDatosEmpresa();


private slots:
    void CambiosEnDatosEmpresa(const QString &);
};

#endif // FDATOSEMPRESA_H
