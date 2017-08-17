#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qt_windows.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSerialPort read;
    void ReadSerialPort();
    void SetSeriaPortAttribute();
    void SetCommBoxTrue();
    void SetCommBoxFalse();
    int  ReceiveBytesNumber = 0;


private slots:

    void ReadComDataSlot();


    void on_ClosePortButton_clicked();

    void on_OpenPortButton_clicked();

    void on_SendButton_clicked();


    void on_ClearButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
