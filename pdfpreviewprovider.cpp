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


#include "pdfpreviewprovider.h"
#include <poppler/qt4/poppler-qt4.h>

PdfPreviewProvider::PdfPreviewProvider() :
    QObject(0),
    QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
}

QImage PdfPreviewProvider::requestImage(
    const QString &id,
    QSize *size,
    const QSize &requestedSize
)
{
    Poppler::Document* doc = Poppler::Document::load(id);
    if (!doc || doc->isLocked()) {
        // TODO: handle errors
        delete doc;
        return QImage();
    }

    doc->setRenderHint(Poppler::Document::Antialiasing);
    doc->setRenderHint(Poppler::Document::TextAntialiasing);

    QImage image = doc->page(0)->renderToImage(120, 120);

    if (image.size() != requestedSize) {
        return image.scaled(
            requestedSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
    } else {
        return image;
    }
}
