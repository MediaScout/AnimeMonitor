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

#ifndef CURLCONNECTION_H
#define CURLCONNECTION_H

#include <QObject>
#include <QByteArray>
#include <curl/curl.h>

class CurlConnection : public QObject
{
    Q_OBJECT
public:
    explicit CurlConnection(QObject *parent = nullptr);
    ~CurlConnection();

    CurlConnection &setURL(const QString& url) noexcept;
    CurlConnection &followLocation(bool enable) noexcept;
    bool readData(QByteArray& byteArray);

private:
    CURL* curl = nullptr;

    static size_t writeCallback(char *ptr, size_t size, size_t nmemb, QByteArray *userdata);

signals:

};

#endif // CURLCONNECTION_H
