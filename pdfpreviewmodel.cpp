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


#include <QDir>
#include <poppler/qt4/poppler-qt4.h>
#include <pdfpreviewmodel.h>
#include <qdebug.h>

PdfPreviewModel::PdfPreviewModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[TITLE] = "title";
    roles[PAGES] = "pages";
    roles[IMAGE] = "image";
    roles[IS_FILE] = "file";
    roles[PATH] = "path";
    setRoleNames(roles);

    _currentDir = QDir::homePath();
}

void PdfPreviewModel::addDirToSearch(QString &dir)
{
}

void PdfPreviewModel::removeDirToSearch(QString &dir)
{
}

void PdfPreviewModel::changeCurrentDir(int index)
{
    if (index >= _dirs.count()) {
        return;
    }
    // XXX: The "Go" thing is ugly is hell. Store paths differently
    if (index == 0 && _dirs[index].startsWith("Go")) {
        _currentDir += "/..";
    } else {
        _currentDir += "/" +  _dirs[index];
    }
    searchPdfFiles();
    reset();
}

void PdfPreviewModel::searchPdfFiles()
{
    _pdfFiles.clear();
    _dirs.clear();

    QDir directory = QDir(_currentDir);
    directory.setFilter(QDir::Files);
    foreach (QString file, directory.entryList(QStringList("*pdf"))) {
        _pdfFiles.append(directory.absoluteFilePath(file));
    }

    directory.setFilter(QDir::Dirs);
    foreach (QString file, directory.entryList()) {
        QString dirToAdd;
        QString absPath = directory.absoluteFilePath(file);
        if (file == "." || !QDir(absPath).isReadable())
            continue;
        if (file == "..") {
            QDir dir = QDir(absPath);
            // XXX Ugly as hell. Do not transform paths here
            if (!dir.isRoot()) {
                dirToAdd =
                    "Go Back To '" + QDir(dir.canonicalPath()).dirName() + "'";
            } else {
                dirToAdd = "Go Back To /";
            }
        } else {
            dirToAdd = QDir(absPath).dirName();
        }
        _dirs.append(dirToAdd);
    }
}


int PdfPreviewModel::rowCount(const QModelIndex &parent) const
{
    return (_dirs.count() + _pdfFiles.count());
}

QVariant PdfPreviewModel::data(const QModelIndex &index, int role) const
{

    int dirRow =  index.row();

    bool isFile = false;
    if (dirRow >=  _dirs.count()) {
        isFile = true;
    }

    if (isFile) {
        int fileRow = index.row() - _dirs.count();
        switch (role) {
            case TITLE:
                return QDir(_pdfFiles[fileRow]).dirName();
                break;
            case IMAGE:
               return (QString(
                    "image://pdf_preview_provider/" +
                    _pdfFiles[fileRow]
                ));
                break;
            case PAGES:
                Poppler::Document* document;
                document = Poppler::Document::load(_pdfFiles[fileRow]);
                if (document)
                    return document->numPages();
                else
                    return 0;
                break;
            case IS_FILE:
                return 1;
                break;
            case PATH:
                return _pdfFiles[fileRow];
                break;
       }
    } else {
        switch (role) {
            case TITLE:
                return _dirs[dirRow];
                break;
            case IMAGE:
                if (dirRow == 0 && _dirs[dirRow].startsWith("Go")) {
                    return QString("images/back_dir.png");
                } else {
                    return QString("images/enter_dir.png");
                }
                break;
            case PAGES:
                return 0;
                break;
            case IS_FILE:
                return 0;
                break;
            case PATH:
                return _dirs[dirRow];
                break;
        }
    }

    return QVariant();
}
