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


#include "renderthread.h"
#include <QMutexLocker>
#include <qdebug.h>

const int RenderThread::MAX_CACHE_PAGES = 4;

RenderThread::RenderThread(RenderPageCache& renderPageCache) :
    _renderPageCache(&renderPageCache),
    _document(0)
{
    start();
    QObject::moveToThread(this);
}

void RenderThread::preparePage(QString filePath, int page)
{
    if (filePath != _currentFilePath) {
        _currentFilePath = filePath;
        if (_document)
            delete _document;
        _document = Poppler::Document::load(filePath);
        if (!_document || _document->isLocked()) {
            delete _document;
            _numPages = 0;
            return;
        }
        _document->setRenderHint(Poppler::Document::Antialiasing);
        _document->setRenderHint(Poppler::Document::TextAntialiasing);
        _numPages = _document->numPages();
        _currentPage = 0;
        initialRender();
    } else {
        if (page >= _numPages)
            return;
        bool signalSend = false;
        if (page >= _renderPageCache->minPage && page <= _renderPageCache->maxPage) {
            pageReady(filePath, page);
            signalSend = true;
        }
        int halfMax = MAX_CACHE_PAGES / 2;
        int minPage = page - halfMax;
        int maxPage = page + halfMax;
        if (page > _currentPage && minPage > 0 && (maxPage < _numPages )) {
            QImage*  image = renderPage(maxPage);
            QMutexLocker locker(&_renderPageCache->mutex);
           _renderPageCache->pages.remove(minPage - 1);
           _renderPageCache->pages[maxPage] = image;
           _renderPageCache->minPage = minPage;
           _renderPageCache->maxPage = maxPage;
        } else if (page < _currentPage && ((maxPage) <  _numPages -1) && minPage >= 0) {
            QImage*  image = renderPage(minPage);
            QMutexLocker locker(&_renderPageCache->mutex);
           _renderPageCache->pages.remove(maxPage + 1);
           _renderPageCache->pages[minPage] = image;
           _renderPageCache->minPage = minPage;
           _renderPageCache->maxPage = maxPage;
        }
        _currentPage = page;
        if (!signalSend)
            pageReady(filePath, page);
    }
}

void RenderThread::run()
{
    exec();
}

QImage* RenderThread::renderPage(int page)
{
    return (new QImage(
        _document->page(page)->renderToImage(120, 120)
    ));
}


/*
 * Render the first MAX_CACHE_PAGES + 1 pages
 */
void RenderThread::initialRender()
{

    foreach (QImage* value, _renderPageCache->pages)
        if (value)
            delete value;
    _renderPageCache->pages.clear();

    int maxPage = std::min((MAX_CACHE_PAGES), _numPages -1);
    for (int i = 0; i <= maxPage; i++) {
        _renderPageCache->pages[i] = renderPage(i);
        if (i == 0) {
            pageReady(_currentFilePath, 0);
            _renderPageCache->minPage = 0;
            _renderPageCache->maxPage = 0;
        }
    }
    _renderPageCache->minPage = 0;
    _renderPageCache->maxPage = maxPage;
}

RenderThread::~RenderThread()
{
    if (_document)
        delete _document;
}

