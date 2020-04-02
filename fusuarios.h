#ifndef FUSUARIOS_H
#define FUSUARIOS_H

#include <QDialog>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDate>

#include <negocio.h>

namespace Ui {
class fUsuarios;
}

class fUsuarios : public QDialog
{
    Q_OBJECT
    unsigned int idcliente;
    bool haycambios = false;

public:
    explicit fUsuarios(QWidget *parent = nullptr);
    explicit fUsuarios(QWidget *parent, unsigned int ID);
    ~fUsuarios();
    void setIdCliente(const unsigned int);
    unsigned int getIdCliente();

signals:
    void cerrandoFicha(unsigned int);


private:
    Ui::fUsuarios *ui;
    void reject() override;
    void accept() override;
    //void closeEvent(QCloseEvent *) override;

    void cargarDatosCliente();
    void guardarDatosCliente();
    void cargarSignalOnChange();
    void devolverColorClampos();

    bool checkCampos();


private slots:
     void CambiosEnFichaCliente(const QString &);
     void CambiosEnFichaCliente(const QDate &);
     void CambiosEnFichaCliente();
     void cambioTxtSexo(const QString &arg1);
};

#endif // FUSUARIOS_H
