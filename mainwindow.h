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

    QSerialPort myport;
    void ReadSerialPort();
    void SetSeriaPortAttribute();
    void SetCommBoxTrue();
    void SetCommBoxFalse();
    void TransmitAsText();
    void TransmitAsHex();
    int  ReceiveBytesNumber = 0;
    int  TransmitBytesNumber = 0;


private slots:

    void ReadComDataSlot();

    void ReadComDataSlot2();

    void on_ClosePortButton_clicked();

    void on_OpenPortButton_clicked();

    void ReceiveAsHexButtonSlot();
    void ReceiveAsTextButtonSlot();
    void on_ClearButton_clicked();
    void AutoSendBoxSlot();
    void WriteComDataSlot();
    void on_TransmitButton_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
