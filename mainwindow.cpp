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
    QVector<QString> header;
    CustomModel::Row currentRow;
    QVector<CustomModel::Row> rows;

    std::function<void(XmlParser::keyValue)> headerComposer =
            [&header](const XmlParser::keyValue& kv){
                        header.push_back(kv.first);};

    std::function<void(XmlParser::keyValue)> rowComposer =
            [&currentRow](const XmlParser::keyValue& kv){
                        currentRow.push_back(kv.second);};

    foreach(QString filename,  files){
        qDebug() << "****" << filename << endl;
        auto parsedData = parser->parseXmlFile(filename);
        numberOfParsedFiles++;

        if(numberOfParsedFiles == 1){
            std::for_each(parsedData.begin(), parsedData.end(), headerComposer);
        }
        std::for_each(parsedData.begin(), parsedData.end(), rowComposer);

        rows.push_back(currentRow);
        emit processedFileNum(numberOfParsedFiles);
    }
    qDebug() << "HEADER END<><><><><><><>" << header;
}
