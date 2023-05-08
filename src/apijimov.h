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

#ifndef APIJIMOV_H
#define APIJIMOV_H

#include <qjsonarray.h>

class APIJimov
{
private:
    APIJimov();

public:
    const QString API_SERVER = "https://jimov-api.onrender.com";

    static QJsonArray searchAnime(const QString& url);
    static QString concat(const QString& url);
};

#endif // APIJIMOV_H
