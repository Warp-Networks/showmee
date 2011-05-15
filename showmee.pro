#-------------------------------------------------
#
# Project created by QtCreator 2011-02-07T16:56:21
#
#-------------------------------------------------

QT       += declarative

TARGET = showmee
CONFIG += meegotouch
TEMPLATE = app


SOURCES += main.cpp \
    pdfpreviewprovider.cpp \
    pdfslideprovider.cpp \
    pdfpreviewmodel.cpp \
    mainwindow.cpp \
    renderthread.cpp

HEADERS += \
    pdfpreviewprovider.h \
    pdfslideprovider.h \
    pdfpreviewmodel.h \
    mainwindow.h \
    renderthread.h \
    renderpagecache.h

OTHER_FILES += \
    PdfPreview.qml

INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -L/usr/lib -lpoppler-qt4

RESOURCES += \
    showmee.qrc

# iadp: Intel AppUp
CONFIG(iadp) {
   target.path = /opt/es.warp.showmee
   #icon.files = showmee.png
   #icon.path = $$[install_prefix]/share/icons
   desktop.files = showmee.desktop
   desktop.path = /usr/share/applications
} else {
   target.path = $$[install_prefix]/bin
   #icon.files = showmee.png
   #icon.path = $$[install_prefix]/share/icons
   desktop.files = showmee.desktop
   desktop.path = $$[install_prefix]/share/applications
}

INSTALLS += target desktop
