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

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDir>
#include <QThread>
#include <QApplication>
#include <QException>
#include <QMessageBox>

#include <curl/curl.h>
#include <iostream>

/**
 * @brief The QueryProvidersThread class is used with the only function
 * of querying the API to obtain as a result the JSON with the providers.
 */
class QueryProvidersThread : public QThread
{
public:
    void run() override;
};

static QueryProvidersThread* query = nullptr;
QJsonObject ResourceCache::providers;

/**
 * @brief SaveJSONProviders write the providers in a JSON file
 * @param object
 */
static void SaveJSONProviders(const QJsonObject& object)
{
    QFile file("providers.json");
    if (!file.open(QIODevice::WriteOnly))
        throw std::exception("Failed to open providers.json file");
    file.write(QJsonDocument(object).toJson());
    file.close();
}

void ResourceCache::LoadJSONProviders()
{
    if (query != nullptr && !query->isFinished())
        return;
    if (query != nullptr)
        delete query;
    query = new QueryProvidersThread();
    query->start();
}

size_t WriteCallback(char *ptr, size_t size, size_t nmemb, std::string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

void QueryProvidersThread::run()
{
    QJsonObject& json = ResourceCache::providers;
    const QString fileJson = "providers.json";
    if (QFile::exists(fileJson))
    {
        QFile file(fileJson);
        if (!file.open(QIODevice::ReadOnly))
            throw std::exception("Failed to open providers.json file");
        json = QJsonDocument::fromJson(file.readAll()).object();
    }
    else
    {
        curl_global_init(CURL_GLOBAL_ALL);
        CURL *curl = curl_easy_init();
        if (curl != nullptr)
        {
            std::string response;
            curl_easy_setopt(curl, CURLOPT_URL, URL_JSON_PROVIDERS);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
               std::cerr << "HTTP request failed: " << curl_easy_strerror(res) << std::endl;
            else {
               try {
                   json = QJsonDocument::fromJson(QByteArray(response.c_str(), response.length())).object();
                   SaveJSONProviders(json);
               }  catch (const QException& e) {
                   QMessageBox::critical(nullptr, "Error", e.what());
               }
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
}
