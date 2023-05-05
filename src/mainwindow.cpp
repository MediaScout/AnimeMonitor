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
#include "newanimedialog.h"
#include "browseranime.h"

#include <qtoolbar.h>
#include <QTabWidget>
#include <QIcon>
#include <QResizeEvent>
#include <QMessageBox>


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
    InitComponents();
}

void MainWindow::InitComponents()
{
    auto addButton    = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "bookmark_24x24.png"), "Agregar Anime");
    auto editButton   = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "trash_24x24.png"), "Editar");
    auto searchButton = ui->toolBar->addAction(QIcon(APP_IMAGES_DIR + "hard-disc-drive_24x24.png"), "Buscar");

    connect(addButton,    &QAction::triggered, this, &MainWindow::AddAnime);
    connect(editButton,   &QAction::triggered, this, &MainWindow::EditAnime);
    connect(searchButton, &QAction::triggered, this, &MainWindow::InitBrowserAnime);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::SearchAction);

    ui->listado->setColumnWidth(0, 250);
    ui->listado->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
    ui->listado->setColumnWidth(3, 150);
    ui->listado->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignmentFlag::AlignLeft);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddAnime()
{
    NewAnimeDialog* dialog = new NewAnimeDialog(this);
    dialog->setModal(true);
    dialog->show();
}

void MainWindow::EditAnime()
{

}

void MainWindow::SearchAction()
{
    QString text = ui->searchTextBox->text().trimmed();
    if (text.isEmpty())
        QMessageBox::warning(this, "Warning", "The text box is empty. Enter the name of the anime");
    else
    {

    }
}

void MainWindow::InitBrowserAnime()
{
    BrowserAnime* dialog = new BrowserAnime(this);
    dialog->show();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
//#ifdef  QT_DEBUG
//    printf("Width %d Height %d\n", event->size().width(), event->size().height());
//#endif

//    QSize sz = event->size();

//    tabControl->setGeometry(8, toolBar->size().height() + 8, sz.width() - 16, sz.height() - (toolBar->size().height() + 8 + statusBar()->size().height() + 8));
}
