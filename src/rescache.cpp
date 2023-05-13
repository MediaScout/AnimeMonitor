// Copyright (C) 2023 Zukaritasu
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "rescache.h"
#include "apijimov.h"
#include "curlconnection.h"
#include "provider.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDir>
#include <QThread>
#include <QApplication>
#include <QException>
#include <QMessageBox>
#include <QFileInfo>
#include <QBuffer>
#include <QDebug>
#include <QToolButton>

#include <iostream>

class QueryProvidersThread : public QThread
{
public:
    void run() override;
};

static QueryProvidersThread* query = nullptr;

ResourceCache* ResourceCache::_cache = nullptr;

void ResourceCache::saveFile(const QString& filename, const QByteArray& byteArray)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open \"" + filename + "\" file";
    } else {
        file.write(byteArray);
        file.close();
    }
}

void ResourceCache::loadJsonProviders()
{
    if (query != nullptr && !query->isFinished())
        return;
    if (query != nullptr)
        delete query;
    query = new QueryProvidersThread();
    QObject::connect(query, &QThread::finished, query, [&](){
        query->deleteLater();
        query = nullptr;
        auto array = getInstance()->_providers["data"];
        if (array.isArray())
            Provider::setProviders(array.toArray());
        _cache->notifyLoadedProviders();
    });
    query->start();
}

void ResourceCache::addComboBox(QComboBox *combobox)
{
    auto* cache = getInstance();
    cache->_event_mutex.lock();
    if (cache->_providers.length() != 0) {
        cache->addElementsComboBox(combobox);
    } else if (!cache->_comboboxs.contains(combobox)) {
       cache->_comboboxs.append(combobox);
    }
    cache->_event_mutex.unlock();
}

void ResourceCache::addToolBar(QToolBar *toolbar)
{
    auto* cache = getInstance();
    cache->_event_mutex.lock();
    if (cache->_providers.length() != 0) {
        cache->addButtonsToolBar(toolbar);
    } else if (!cache->_toolbars.contains(toolbar)) {
        cache->_toolbars.append(toolbar);
    }
    cache->_event_mutex.unlock();
}

QJsonObject ResourceCache::readJson(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        throw std::exception(QString("Failed to open " + filename + " file").toStdString().c_str());
    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    return object;
}

QImage ResourceCache::getImage(const Provider &prov)
{
    QFile fileicon(prov.getName().toLower());
    QDir relative_dir("./");
    auto list = relative_dir.entryList(QStringList() << QString(prov.getName().toLower()).append(".*"), QDir::Files);
    for (const auto &filename : list) {
        QString ext = QFileInfo(relative_dir, filename).suffix().toLower();
        if (QStringList({"png", "jpg", "jpeg", "bmp"}).contains(ext)) {
            return QImage(filename);
        }
    }
    return QImage("photo.png");
}

QIcon* ResourceCache::downloadImage(const QString &url, bool save, const QString& name)
{
    try {
        CurlConnection curl;
        curl.setURL(url).followLocation(true);
        QByteArray data;
        if (curl.readData(data)) {
            QFile outfile(name);
            if (save && !outfile.open(QIODevice::WriteOnly))
                qDebug() << "Failed to open " + name + " file";
            else {
                outfile.write(data);
                outfile.close();
            }
            return new QIcon(name);
        }
    }  catch (const std::exception& e) {
        qDebug() << e.what();
    }
    return nullptr;
}

bool ResourceCache::isLoadedProviders() noexcept
{
    return getInstance()->_providers.count() != 0;
}

void ResourceCache::notifyLoadedProviders()
{
    _event_mutex.lock();
    notifyComboBoxs();
    notifyToolBars();
    _event_mutex.unlock();
}

void ResourceCache::notifyComboBoxs()
{
    auto* cache = getInstance();
    for (int i = 0; i < cache->_comboboxs.size(); i++)
        addElementsComboBox(cache->_comboboxs[i]);
    cache->_comboboxs.clear();
}

void ResourceCache::notifyToolBars()
{
    auto* cache = getInstance();
    for (int i = 0; i < cache->_toolbars.size(); i++)
        addButtonsToolBar(cache->_toolbars[i]);
    cache->_toolbars.clear();
}

void ResourceCache::addElementsComboBox(QComboBox *combobox)
{
    auto array = getInstance()->_providers["data"];
    if (array.isArray()) {
        QJsonArray provs = array.toArray();
        for (int i = 0; i < provs.count(); i++) {
            combobox->addItem(provs[i].toObject()["name"].toString());
        }
    }
}

static QString getDefaultUrFavicon(const QJsonValueRef& ref)
{
    if (ref.isString()) {
        return ref.toString();
    } else if (ref.isArray()) {
        return ref.toArray().last().toString();
    }
    qDebug() << "invalid property favicon in JSON";
    return ""; // invalid JSON
}

QIcon* ResourceCache::getIconProvider(const QJsonValue& value, const QString& url_icon)
{   QString name = value.toObject()["name"].toString();
    QString filename = name.toLower() + "." + QFileInfo(QUrl(url_icon).path()).suffix();
    QIcon* image = QFile::exists(filename) ? new QIcon(filename) :
            ResourceCache::downloadImage(url_icon, true, filename);
    return image;
}

static void readAllIconProvidersCallback(const QJsonArray& providers,
                                         QList<QIcon*>* icons)
{
    for (int i = 0; i < providers.count(); i++) {
        const QString url_favicon = getDefaultUrFavicon(providers[i].toObject()["favicon"]);
        if (!url_favicon.isEmpty()) {
            icons->append(ResourceCache::getInstance()->getIconProvider(providers[i], url_favicon));
        }
    }
}

void ResourceCache::addButtonsToolBar(QToolBar *toolbar)
{
    auto array = getInstance()->_providers["data"];
    if (array.isArray()) {
        QJsonArray* providers = new QJsonArray(array.toArray());
        QList<QIcon*>* icons = new QList<QIcon*>();
        QThread* thread = QThread::create(::readAllIconProvidersCallback, *providers, icons);
        QObject::connect(thread, &QThread::finished, thread, [thread, providers, icons, toolbar](){
            thread->deleteLater();
            for (int j = 0; j < providers->count(); j++) {
                toolbar->addAction(*((*icons)[j]), (*providers)[j].toObject()["name"].toString());
            }
            delete providers;
            delete icons;
        });
        thread->start();
    }
}

ResourceCache *ResourceCache::getInstance()
{
    if (_cache == nullptr) _cache = new ResourceCache();
    return _cache;
}

void QueryProvidersThread::run()
{
    QJsonObject& json = ResourceCache::getInstance()->_providers;
    const QString fileJson = "providers.json";
    if (QFile::exists(fileJson)) {
        try {
            json = ResourceCache::readJson(fileJson);
            return;
        }  catch (const std::exception& e) {
            qDebug() << e.what();
        }
    }

    try {
        CurlConnection curl;
        curl.setURL(URL_JSON_PROVIDERS);
        QByteArray response;
        if (curl.readData(response)) {
            json = QJsonDocument::fromJson(response).object();
            ResourceCache::getInstance()->saveFile(fileJson, QJsonDocument(json).toJson());
        }
    }  catch (const std::exception& e) {
        qDebug() << e.what();
    }
}
