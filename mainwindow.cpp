#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <model.h>

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CustomModel* customModel = new CustomModel();
    ui->tableView->setModel(customModel);

    parser = new XmlParser;

    connect(ui->importXmlBtn, SIGNAL(clicked()), this, SLOT(importBtnHandler()));
    connect(this, &MainWindow::processedFileNum, this, [this](int num){
        ui->progressBar->setValue(num);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::importBtnHandler()
{
    QFileDialog dialog;
    files = dialog.getOpenFileNames(this, QString(),
                            QDir::currentPath(),
                            "XML files (*.xml, *.XML)");

    ui->progressBar->setRange(0, files.count());
    QtConcurrent::run(this, &MainWindow::operate);
}

void MainWindow::operate()
{
    int numberOfParsedFiles = 0;
    foreach(QString filename,  files){
        qDebug() << "****" << filename << endl;
        parser->parseXmlFile(filename);
        emit processedFileNum(++numberOfParsedFiles);
    }
}
