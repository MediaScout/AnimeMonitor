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

#ifndef ABOUTAPPDLG_H
#define ABOUTAPPDLG_H

#include <QDialog>

namespace Ui {
class AboutAppDialog;
}

class AboutAppDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutAppDialog(QWidget *parent = nullptr);
    ~AboutAppDialog();

private:
    Ui::AboutAppDialog *ui;
};

#endif // ABOUTAPPDLG_H
