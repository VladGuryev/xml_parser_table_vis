#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xmlparser.h>


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

public slots:
    void importBtnHandler();


private:
    Ui::MainWindow *ui;
    XmlParser* parser;
    QStringList files;
};

#endif // MAINWINDOW_H
