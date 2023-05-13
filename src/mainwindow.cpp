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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rescache.h"
#include "apijimov.h"
#include "provider.h"
#include "infoprovdlg.h"
#include "constants.h"
#include "aboutappdlg.h"
#include "themestyle.h"

#include <qtoolbar.h>
#include <QTabWidget>
#include <QIcon>
#include <QResizeEvent>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComponents();
}

void MainWindow::initComponents()
{
    auto addButton     = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "bookmark_32x32.png"), "Agregar Anime");
    auto editButton    = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "trash_32x32.png"), "Editar");
    auto searchButton  = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "hard-disc-drive_32x32.png"), "Buscar");
    auto optionsButton = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "settings_32x32.png"), "Opciones");
    auto aboutButton   = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "star_32x32.png"), "Acerca de");

    connect(optionsButton,    &QAction::triggered, this, &MainWindow::showOptionsDialog);
    connect(aboutButton,      &QAction::triggered, this, &MainWindow::showAboutAppDialog);
    connect(addButton,        &QAction::triggered, this, &MainWindow::addAnime);
    connect(editButton,       &QAction::triggered, this, &MainWindow::editAnime);
    connect(searchButton,     &QAction::triggered, this, &MainWindow::initBrowserAnime);
    connect(ui->search_button, &QPushButton::clicked, this, &MainWindow::searchAction);
    connect(ui->button_info_provider, &QPushButton::clicked, this, &MainWindow::showInfoProviderDialog);

    ui->listado->setColumnWidth(0, 250);
    ui->listado->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->setColumnWidth(3, 150);
    ui->listado->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);

    QFile file("../html/panel-search/index.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString html = file.readAll();
        ui->textBrowser->setHtml(html);
        file.close();
    }

    //connect(ui->listAnimes, &QListView::clicked, this, &MainWindow::listResultClicked);

    /** set styles **/
    ThemeStyle::setStyleSheet(ui->top_panel_search);
    /*ThemeStyle::setTheme(ui->combobox_providers);
    ThemeStyle::setTheme(ui->search_lineedit);
    ThemeStyle::setTheme(ui->search_button);*/

    initComboBoxProviders();
    initToolBarIconProviders();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Provider MainWindow::getSelectedProvider() const
{
    const int index = ui->combobox_providers->currentIndex();
    if (index != -1) {
        QString item = ui->combobox_providers->itemText(index);
        for (const auto& provider : Provider::getProviders()) {
            if (provider->getName() == item) {
                return *provider;
            }
        }
    }
    return Provider();
}

void MainWindow::addAnime()
{
}

void MainWindow::editAnime()
{
}

void MainWindow::showOptionsDialog()
{

}

void MainWindow::showInfoProviderDialog()
{
    Provider provider = getSelectedProvider();
    if (!provider.getName().isEmpty()) {
        InfoProviderDialog* dialog = new InfoProviderDialog(provider, this);
        dialog->setModal(true);
        dialog->show();
    }
}

void MainWindow::showAboutAppDialog()
{
    AboutAppDialog* dialog = new AboutAppDialog(this);
    dialog->setModal(true);
    dialog->show();
}

void MainWindow::searchAction()
{
    QString url = getFilterUrlProvider();
    if (!url.isEmpty()) {
        if (_qthsearch != nullptr && _qthsearch->isRunning())
            return;
        qDebug() << "Buscando " << url;
        _qthsearch = QThread::create(MainWindow::searchAnimeCallback, ui, url, this);
        _qthsearch->start();
    }
}

void MainWindow::searchAnimeCallback(Ui::MainWindow *ui, const QString& url, MainWindow* window)
{
    ui->notification->setText("Buscando en el filtro: " + url);
    auto array_result = APIJimov::searchAnime(url);
    const int count = array_result.count();
    if (count == 0) {
        ui->notification->setText("No se encontraron resultados para " + url);
    } else {
        window->results = array_result;
        ui->notification->setText("Se han encontrado " + QString::number(count) + " resultados.");
        for (int i = 0; i < count; i++) {
            QJsonValueRef value = array_result[i].toObject()["name"];
            if (value.isString()) {
                //ui->listAnimes->addItem(value.toString());
            }
        }
    }
}

void MainWindow::initBrowserAnime()
{

}

void MainWindow::initComboBoxProviders()
{
    ResourceCache::addComboBox(ui->combobox_providers);
}

void MainWindow::initToolBarIconProviders()
{
    ResourceCache::addToolBar(ui->toolBarProviders);
}

void MainWindow::listResultClicked(const QModelIndex &index)
{
    qDebug() << results[index.row()];
}

QString MainWindow::getFilterUrlProvider()
{
    if (ResourceCache::isLoadedProviders()) {
        QString text = ui->search_lineedit->text().trimmed();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Warning", "The text box is empty. Enter the name of the anime");
        } else {
            Provider provider = getSelectedProvider();
            if (!provider.getName().isEmpty()) {
                return provider.createFilterUrl(text);
            }
        }
    }
    return QString();
}
