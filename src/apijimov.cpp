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

#include "apijimov.h"
#include "curlconnection.h"
#include "constants.h"

#include <curl/curl.h>
#include <iostream>

#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

APIJimov::APIJimov() {}

QJsonArray APIJimov::searchAnime(const QString &url)
{
    try {
        CurlConnection curl;
        curl.setURL(url);
        QByteArray data;
        if (curl.readData(data)) {
            QJsonObject json = QJsonDocument::fromJson(data).object();
            QJsonValueRef data = json["results"];
            if (data.isArray()) return data.toArray();
        }
    }  catch (const std::exception& e) {
        qDebug() << e.what();
    }
    return QJsonArray();
}

QString APIJimov::concat(const QString &url)
{
    QString path = url;
    if (path.length() > 0 && path[0] != '/')
        path = "/" + path;
    return API_SERVER + path;
}
