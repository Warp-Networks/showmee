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


#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QImage>
#include <poppler/qt4/poppler-qt4.h>
#include "renderpagecache.h"


class RenderThread : public QThread
{
     Q_OBJECT

public:
    RenderThread(RenderPageCache& renderPageCache);
    ~RenderThread();
    void run();
signals:
    void pageReady(QVariant, QVariant);

public slots:
    void preparePage(QString, int);

private:
    void initialRender();
    QImage* renderPage(int page);

    QString _currentFilePath;
    RenderPageCache* _renderPageCache;
    Poppler::Document* _document;
    int _numPages;
    int _currentPage;

    static const int MAX_CACHE_PAGES;

};

#endif // RENDERTHREAD_H
