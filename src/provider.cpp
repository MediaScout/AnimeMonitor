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

#include "provider.h"
#include "constants.h"
#include "rescache.h"

QList<Provider*> Provider::_providers;

Provider::Provider(QObject *parent) : QObject(parent) {}

Provider::Provider(const Provider &provider)
{
    _name        = provider._name;
    _url_path    = provider._url_path;
    _language    = provider._language;
    _url         = provider._url;
    _description = provider._description;
}

Provider::Provider(const QJsonObject &provider)
{
    _name        = provider["name"].toString();
    _url_path    = provider["apiID"].toString();
    _language    = provider["language"].toString();
    _url         = provider["url"].toString();
    _description = provider["description"].toString();
}

QImage Provider::getImage() const
{
    return ResourceCache::getImage(*this);
}

QString Provider::createFilterUrl(const QString &search) const
{
    QString text = search;
    text.replace(" ", "%20");
    return QString::asprintf(URLFORMAT_API_FILTER, _C_STR(_url_path), _C_STR(text));
}

void Provider::setProviders(const QJsonArray &array) noexcept
{
    if (array.count() != 0) {
        _providers.clear();
        for (int i = 0; i < array.count(); i++) {
            _providers.append(new Provider(array[i].toObject()));
        }
    }
}

const QList<Provider*> &Provider::getProviders() noexcept
{
    return _providers;
}
