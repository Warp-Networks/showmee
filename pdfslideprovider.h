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


#ifndef PDFSLIDEPROVIDER_H
#define PDFSLIDEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QObject>
#include <renderpagecache.h>
#include <renderthread.h>

class PdfSlideProvider : public QObject, public QDeclarativeImageProvider
{
    Q_OBJECT

public:
    explicit PdfSlideProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    RenderPageCache _renderPageCache;

signals:
    void pageReady(QVariant, QVariant);
    void renderPage(QString, int);

public slots:
    void preparePage(QString, int);

private:
    RenderThread* _renderThread;
    QString _currentFile;


};

#endif // PDFPREVIEWPROVIDER_H
