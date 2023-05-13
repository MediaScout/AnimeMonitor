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

#include "themestyle.h"
#include "constants.h"
#include <exception>

#include <QFile>

ThemeStyle::ThemeStyle() {}

void ThemeStyle::setStyleSheet(QWidget *widget)
{
    if (widget == nullptr || widget->objectName().isEmpty()) return;
    QFile css_file(APP_CSS_DIR + widget->objectName() + ".css");
    if (css_file.exists()) {
        if (!css_file.open(QIODevice::ReadOnly))
            throw std::exception(_C_STR(css_file.errorString()));
        widget->setStyleSheet(QString(css_file.readAll()));
        css_file.close();
    }
}
