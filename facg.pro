QT       += core gui sql uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    Evento.cpp \
    faddabonocargo.cpp \
    fcuentas.cpp \
    fdatosempresa.cpp \
    fexportimagecuentascliente.cpp \
    flistadoproductos.cpp \
    fproducto.cpp \
    fusuarios.cpp \
    negocio.cpp \
    main.cpp \
    mainwindow.cpp \
    producto.cpp

HEADERS += \
    Evento.h \
    conexion.h \
    faddabonocargo.h \
    fcuentas.h \
    fdatosempresa.h \
    fexportimagecuentascliente.h \
    flistadoproductos.h \
    fproducto.h \
    fusuarios.h \
    negocio.h \
    mainwindow.h \
    producto.h

FORMS += \
    faddabonocargo.ui \
    fcuentas.ui \
    fdatosempresa.ui \
    fexportimagecuentascliente.ui \
    flistadoproductos.ui \
    formUsuarios.ui \
    fproducto.ui \
    fusuarios.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    forms.qrc



