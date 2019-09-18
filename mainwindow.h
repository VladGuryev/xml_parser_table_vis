#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xmlparser.h>
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void operate();

signals:
    void processedFileNum(int);
    void headerReady(QVector<QString>);
    void okFileNumber(int);

public slots:
    void importBtnHandler();
    void okFileLabelUpdater(int);
    void errorLogHandler(QString);

private:
    Ui::MainWindow *ui;
    XmlParser* parser;
    QStringList files;
    CustomModel* customModel;
};

#endif // MAINWINDOW_H
