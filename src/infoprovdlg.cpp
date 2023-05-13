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

#include "infoprovdlg.h"
#include "ui_infoprovdlg.h"

#include <QPixmap>

InfoProviderDialog::InfoProviderDialog(const Provider& prov, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infoprovdlg)
{
    ui->setupUi(this);

    ui->label_name->setText(prov.getName());
    ui->label_language->setText(prov.getLanguage());
    ui->label_description->setText(prov.getDescription());
    ui->label_photo->setPixmap(QPixmap::fromImage(prov.getImage()));

    QString link = prov.getUrl();
    if (!link.isEmpty() && link != "NA")
        link = "<a href=\"" + link + "\">" + link + "</a>";
    ui->label_url->setText(link);
}

InfoProviderDialog::~InfoProviderDialog()
{
    delete ui;
}
