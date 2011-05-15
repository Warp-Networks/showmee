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


#include "pdfslideprovider.h"
#include <poppler/qt4/poppler-qt4.h>
#include <QDebug>
#include <QMutexLocker>


PdfSlideProvider::PdfSlideProvider() :
    QObject(0),
    QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    _renderThread = new RenderThread( _renderPageCache);

    QObject::connect(
        this,
        SIGNAL(renderPage(QString, int)),
        _renderThread,
        SLOT(preparePage(QString, int)),
        Qt::QueuedConnection
    );

    QObject::connect(
        _renderThread,
        SIGNAL(pageReady(QVariant, QVariant)),
        this,
        SIGNAL(pageReady(QVariant, QVariant)),
        Qt::QueuedConnection
    );

}

QImage PdfSlideProvider::requestImage(
    const QString &id,
    QSize *size,
    const QSize &requestedSize
)
{
    QStringList tokens = id.split("/page-");
    QString file = tokens[0];
    int page = 0;
    if (tokens.count() > 1) {
        page = tokens[1].toInt();
    }

    QMutexLocker(&_renderPageCache.mutex);
    if (_renderPageCache.pages.contains(page)) {
        return *_renderPageCache.pages[page];
    } else {
        return QImage();
    }
}

void PdfSlideProvider::preparePage(QString filePath, int page)
{
    renderPage(filePath, page);
}
