#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtConcurrent/QtConcurrent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDir>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(880, 515));

    customModel = new CustomModel;
    ui->tableView->setModel(customModel);
    setWindowTitle("XML Parser");

    parser = new XmlParser(this);

    connect(ui->importXmlBtn, &QPushButton::clicked,
            this, &MainWindow::importBtnHandler);
    connect(ui->exportXmlBtn, &QPushButton::clicked,
            this, &MainWindow::exportBtnHandler);
    connect(parser, &XmlParser::errorLogSender,
            this, &MainWindow::errorLogHandler, Qt::QueuedConnection);

    connect(this, &MainWindow::processedFileNum, this, [this](int num){
        ui->progressBar->setValue(num);}, Qt::QueuedConnection);
    connect(this, SIGNAL(okFileNumber(int)), this, SLOT(okFileLabelUpdater(int)),
            Qt::QueuedConnection);
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
    ui->okFilesLabel->clear();
    QtConcurrent::run(this, &MainWindow::processImport);
}

void MainWindow::exportBtnHandler()
{
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save table data to XML file"), "",
           tr("XML file (*.XML);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {

        QtConcurrent::run(this, &MainWindow::processExport, fileName);
    }
}

void MainWindow::okFileLabelUpdater(int num)
{
    ui->okFilesLabel->setText(QString::number(num));
}

void MainWindow::errorLogHandler(QString e)
{
    ui->errorField->appendPlainText(e + "\n");
}

void MainWindow::processImport()
{
    int numberOfParsedFiles = 0;
    int errorParsedFileNum = 0;
    QVector<QString> header;
    CustomModel::Row currentRow;
    QVector<CustomModel::Row> rows;

    std::function<void(XmlParser::keyValue)> headerComposer =
            [&header](const XmlParser::keyValue& kv){
                        header.push_back(kv.first);};

    std::function<void(XmlParser::keyValue)> rowComposer =
            [&currentRow](const XmlParser::keyValue& kv){
                        currentRow.push_back(kv.second);};

    for(const auto& filename: files){
        bool isParsedOk = false;
        auto parsedData = parser->importXmlFile(filename, isParsedOk);
        emit processedFileNum(++numberOfParsedFiles);

        if(!isParsedOk){
            ++errorParsedFileNum;
            continue;
        }
        emit okFileNumber(numberOfParsedFiles - errorParsedFileNum);

        if(numberOfParsedFiles == 1){
            std::for_each(parsedData.begin(), parsedData.end(), headerComposer);
        }
        currentRow.clear();
        std::for_each(parsedData.begin(), parsedData.end(), rowComposer);

        rows.push_back(currentRow);
    }

    if(rows.count() > 0){
        customModel->setRows(rows);
        customModel->setHeader(header);
    }
}

void MainWindow::processExport(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to create xml file for export"),
            file.errorString());
        return;
    }
    parser->exportXmlToFile(customModel->getHeader(),
                            customModel->getRows(),
                            &file);
    qDebug() << "processExport";
    file.close();
}
