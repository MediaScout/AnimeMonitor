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

#include "aboutappdlg.h"
#include "ui_aboutappdlg.h"

#include <QPixmap>
#include <QImage>

AboutAppDialog::AboutAppDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutAppDialog)
{
    ui->setupUi(this);
    ui->label_about_image->setPixmap(QPixmap::fromImage(QImage("about_image.png")));
}

AboutAppDialog::~AboutAppDialog()
{
    delete ui;
}
