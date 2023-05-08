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

#ifndef PROVIDER_H
#define PROVIDER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Provider : public QObject
{
    Q_OBJECT
public:
    const char* URLFORMAT_API_FILTER = "https://jimov.herokuapp.com/anime/%s/filter?title=%s";

    explicit Provider(QObject *parent = nullptr);
    Provider(const QJsonObject& provider);

    QString getName() const { return _name; }
    QString getUrlPath() const { return _url_path; }
    QString getUrl() const { return _url; }
    QString getDescription() const { return _description; }
    QString getLanguage() const { return _language; }
    QString createFilterUrl(const QString& search) const;

    static void setProviders(const QJsonArray& array) noexcept;
    static const QList<Provider*> &getProviders() noexcept;

private:
    static QList<Provider*> _providers;

    QString _name;
    QString _url_path;
    QString _url;
    QString _description;
    QString _language;

signals:

};

#endif // PROVIDER_H
