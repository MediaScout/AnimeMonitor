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

#ifndef RESCACHE_H
#define RESCACHE_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QMutex>
#include <QComboBox>
#include <QToolBar>
#include <QImage>

#define URL_JSON_PROVIDERS "https://jimov.herokuapp.com/providers"

class ResourceCache: public QObject
{
    Q_OBJECT
public:
    static void loadJsonProviders();
    static void addComboBox(QComboBox* combobox);
    static void addToolBar(QToolBar* toolbar);
    static QJsonObject readJson(const QString& filename);
    void saveFile(const QString& filename, const QByteArray& byteArray);
    static QIcon* downloadImage(const QString& url, bool save = true, const QString& name = nullptr);
    static const QJsonObject& getProviders() { return getInstance()->_providers; }
    static bool isLoadedProviders() noexcept;

private:
    static ResourceCache* _cache;
    QJsonObject _providers;
    QVector<QComboBox*> _comboboxs;
    QVector<QToolBar*> _toolbars;
    QMutex _event_mutex;

    friend class QueryProvidersThread;
    friend class MainWindow;
    friend void readAllIconProvidersCallback(const QJsonArray&, QToolBar*, QList<QIcon*>*);

    ResourceCache(QObject* parent = nullptr) : QObject(parent) {}

    void notifyLoadedProviders();
    void notifyComboBoxs();
    void notifyToolBars();
    void addElementsComboBox(QComboBox* combobox);
    void addButtonsToolBar(QToolBar* toolbar);
    QIcon* getIconProvider(const QJsonValue& value, const QString& url_icon);

    static ResourceCache *getInstance();

signals:
};

#endif // RESCACHE_H
