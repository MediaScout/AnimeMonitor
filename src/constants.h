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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define API_SERVER "https://jimov-api.onrender.com"

#define _C_STR(qstr) qstr.toStdString().c_str()

#ifdef QT_DEBUG
    #define APP_IMAGES_DIR QString("../images/")
    #define APP_CSS_DIR    QString("../css/"   )
#else
    #define APP_IMAGES_DIR QString()
    #define APP_CSS_DIR QString()
#endif

#endif // CONSTANTS_H
