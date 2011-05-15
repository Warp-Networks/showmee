/*
 *
 *  ShowMee - Presentation software with touch interface
 *
 *  Copyright (C) 2011   Warp Networks, S.L. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#include "mainwindow.h"
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <pdfpreviewprovider.h>
#include <pdfslideprovider.h>
#include <pdfpreviewmodel.h>
#include <qdebug.h>


MainWindow::MainWindow(QDesktopWidget *parent) :
  QMainWindow(parent)
{
    this->ui = new QDeclarativeView;
    QDeclarativeEngine* engine = this->ui->engine();

    PdfPreviewProvider* pdfPreviewProvider = new PdfPreviewProvider();
    engine->addImageProvider(
        QLatin1String("pdf_preview_provider"),
        pdfPreviewProvider
    );

    PdfSlideProvider* pdfSlideProvider = new PdfSlideProvider();
    engine->addImageProvider(
        QLatin1String("pdf_slide_provider"),
        pdfSlideProvider
    );

    PdfPreviewModel* model = new PdfPreviewModel();
    model->searchPdfFiles();
    engine->rootContext()->setContextProperty("pdfPreviewModel", model);

    this->ui->setSource(QUrl("qrc:/ShowMee.qml"));
    this->ui->show();

    QObject::connect(
        (QObject*)this->ui->rootObject(),
        SIGNAL(fullScreen(bool)),
        this,
        SLOT(fullScreen(bool))
    );

    QObject::connect(
        (QObject*)this->ui->rootObject(),
        SIGNAL(changeDirectory(int)),
        model,
        SLOT(changeCurrentDir(int))
    );

    QObject::connect(
        (QObject*)this->ui->rootObject(),
        SIGNAL(preparePage(QString, int)),
        pdfSlideProvider,
        SLOT(preparePage(QString, int))
    );

    QObject::connect(
        pdfSlideProvider,
        SIGNAL(pageReady(QVariant, QVariant)),
        (QObject*)this->ui->rootObject(),
        SLOT(pageReady(QVariant, QVariant))
   );

    setCentralWidget(this->ui);
}

void MainWindow::fullScreen(bool full)
{
    if (full) {
        showFullScreen();
    } else {
        showNormal();
    }
}

MainWindow::~MainWindow()
{
  delete this->ui;
}

