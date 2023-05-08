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

#include <qtoolbar.h>
#include <QTabWidget>
#include <QIcon>
#include <QResizeEvent>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


#ifdef QT_DEBUG
    #define APP_IMAGES_DIR QString("../images/")
#else
    #define APP_IMAGES_DIR QString()
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComponents();
}

void MainWindow::initComponents()
{
    auto addButton    = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "bookmark_24x24.png"), "Agregar Anime");
    auto editButton   = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "trash_24x24.png"), "Editar");
    auto searchButton = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "hard-disc-drive_24x24.png"), "Buscar");

    connect(addButton,    &QAction::triggered, this, &MainWindow::addAnime);
    connect(editButton,   &QAction::triggered, this, &MainWindow::aditAnime);
    connect(searchButton, &QAction::triggered, this, &MainWindow::initBrowserAnime);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchAction);

    ui->listado->setColumnWidth(0, 250);
    ui->listado->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->setColumnWidth(3, 150);
    ui->listado->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);

    connect(ui->listAnimes, &QListView::clicked, this, &MainWindow::listResultClicked);

    /** styles **/

    ui->top_panel_search->setStyleSheet(
                "QWidget#top_panel_search {"
                "  background-color: #F0F0F0;"
                "  border-radius: 5px;"
                "  border: 1px solid white;"
                "}"
            );

    initComboBoxProviders();
    initToolBarIconProviders();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addAnime()
{
}

void MainWindow::aditAnime()
{
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
                ui->listAnimes->addItem(value.toString());
            }
        }
    }
}

void MainWindow::initBrowserAnime()
{
}

void MainWindow::initComboBoxProviders()
{
    ResourceCache::addComboBox(ui->comboBoxProviders);
}

void MainWindow::initToolBarIconProviders()
{
    ui->toolBarProviders->setStyleSheet("QToolBar { border: none; }");


    ui->toolBarProviders->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  border-radius: 16px;"
        "  background-image: url(:/images/myimage.png);"
        "  background-repeat: no-repeat;"
        "  background-position: center;"
        "}"
    );

    ResourceCache::addToolBar(ui->toolBarProviders);
}

void MainWindow::listResultClicked(const QModelIndex &index)
{
    qDebug() << results[index.row()];
}

QString MainWindow::getFilterUrlProvider()
{
    if (ResourceCache::isLoadedProviders()) {
        QString text = ui->searchTextBox->text().trimmed();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Warning", "The text box is empty. Enter the name of the anime");
        } else {
            const int index = ui->comboBoxProviders->currentIndex();
            if (index != -1) {
                QString item = ui->comboBoxProviders->itemText(index);
                for (const auto& provider : Provider::getProviders()) {
                    if (provider->getName() == item) {
                        return provider->createFilterUrl(text);
                    }
                }
            }
        }
    }
    return QString();
}
