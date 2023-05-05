#include "newanimedialog.h"
#include "ui_newanimedialog.h"

#include "rescache.h"

#include <qjsonarray.h>
#include <iostream>

NewAnimeDialog::NewAnimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAnimeDialog)
{
    ui->setupUi(this);

    auto array = ResourceCache::GetProviders()["data"];
    if (array.isArray())
    {
        QJsonArray providers = array.toArray();
        for (int i = 0; i < providers.count(); i++) {
            ui->providers->addItem(providers[i].toObject()["name"].toString());
        }
    }
}

NewAnimeDialog::~NewAnimeDialog()
{
    delete ui;
}
