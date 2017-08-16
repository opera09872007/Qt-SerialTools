#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setWindowTitle(tr("串口调试工具 by dannywang"));
    ReadSerialPort();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReadSerialPort(){
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts()){
        ui->comboBox->addItem(qspinfo.portName());
    }
}


void MainWindow::on_pushButton_clicked()
{

    read.setPortName(ui->comboBox->currentText());
    read.open(QIODevice::ReadWrite);
        read.setBaudRate(ui->comboBox_2->currentText().toInt());
        if(ui->comboBox_3->currentText() == tr("无校验"))
        {
            read.setParity(QSerialPort::NoParity);
        }
        else if(ui->comboBox_3->currentText() == tr("奇校验"))
        {
            read.setParity(QSerialPort::OddParity);
        }
        else if(ui->comboBox_3->currentText() == tr("偶校验"))
        {
            read.setParity(QSerialPort::EvenParity);
        }
        if(ui->comboBox_4->currentText() == 5)
        {
            read.setDataBits(QSerialPort::Data5);
        }
        else if(ui->comboBox_4->currentText() == 6)
        {
            read.setDataBits(QSerialPort::Data6);
        }
        else if(ui->comboBox_4->currentText() == 7)
        {
            read.setDataBits(QSerialPort::Data7);
        }
        else if(ui->comboBox_4->currentText() == 8)
        {
            read.setDataBits(QSerialPort::Data8);
        }
        if(ui->comboBox_5->currentText() == 1)
        {
            read.setStopBits(QSerialPort::OneStop);
        }
        else if(ui->comboBox_5->currentText() == 2)
        {
            read.setStopBits(QSerialPort::TwoStop);
        }
       // read.setStopBits(ui->comboBox_5->currentText());

        read.setFlowControl(QSerialPort::NoFlowControl);

        read.clearError();
        read.clear();
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(readComDataSlot()));
        timer->start(1000);



   //QMessageBox::warning(this,tr("error"),tr("error open"),QMessageBox::Ok);
}


void MainWindow::readComDataSlot()
{
    QByteArray buf;
    buf = read.readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->textEdit_2->toPlainText();
        str+=tr(buf);
        ui->textEdit_2->clear();
        ui->textEdit_2->append(str);
    }
    buf.clear();
}
