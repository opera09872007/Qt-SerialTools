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


void MainWindow::on_OpenPortButton_clicked(){
    read.close();

    read.setPortName(ui->PortBox->currentText());
    if(!read.open(QIODevice::ReadWrite)){
        QMessageBox::warning(this,tr("error"),tr("串口打开失败"),QMessageBox::Ok);
        ui->StateLabel->setText(ui->PortBox->currentText()+"打开失败");
        ui->StateLabel->setStyleSheet("color:red;");
    }
    else{
        SetSeriaPortAttribute();

        read.clearError();
        read.clear();

        SetCommBoxFalse();
        ui->StateLabel->setText(ui->PortBox->currentText()+"打开成功");
        ui->StateLabel->setStyleSheet("color:green;");

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(ReadComDataSlot()));
        timer->start(1000);

    }
}


void MainWindow::on_pushButton_2_clicked(){
    ui->StateLabel->clear();
    read.clear();
    read.close();
    SetCommBoxTrue();
}

void MainWindow::ReadComDataSlot(){
    QByteArray buf;
    buf = read.readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->ReceiveText->toPlainText();
        str+=tr(buf);
        ui->ReceiveText->clear();
        ui->ReceiveText->append(str);
    }
    buf.clear();
}




void MainWindow::ReadSerialPort(){
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts()){
        ui->PortBox->addItem(qspinfo.portName());
    }
}

void MainWindow::SetSeriaPortAttribute(){
    //设置波特率
    read.setBaudRate(ui->BaudBox->currentText().toInt());
    //设置校验位
    if(ui->ParityBox->currentText() == tr("无校验"))
    {
        read.setParity(QSerialPort::NoParity);
    }
    else if(ui->ParityBox->currentText() == tr("奇校验"))
    {
        read.setParity(QSerialPort::OddParity);
    }
    else if(ui->ParityBox->currentText() == tr("偶校验"))
    {
        read.setParity(QSerialPort::EvenParity);
    }
    //设置数据位
    if(ui->DataBitsBox->currentText() == 5)
    {
        read.setDataBits(QSerialPort::Data5);
    }
    else if(ui->DataBitsBox->currentText() == 6)
    {
        read.setDataBits(QSerialPort::Data6);
    }
    else if(ui->DataBitsBox->currentText() == 7)
    {
        read.setDataBits(QSerialPort::Data7);
    }
    else if(ui->DataBitsBox->currentText() == 8)
    {
        read.setDataBits(QSerialPort::Data8);
    }
    //设置停止位
    if(ui->StopBitsBox->currentText() == 1)
    {
        read.setStopBits(QSerialPort::OneStop);
    }
    else if(ui->StopBitsBox->currentText() == 2)
    {
        read.setStopBits(QSerialPort::TwoStop);
    }
    //设置流量
    read.setFlowControl(QSerialPort::NoFlowControl);
}


void MainWindow::SetCommBoxTrue(){
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->DataBitsBox->setEnabled(true);
    ui->StopBitsBox->setEnabled(true);
}
void MainWindow::SetCommBoxFalse(){
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->DataBitsBox->setEnabled(false);
    ui->StopBitsBox->setEnabled(false);

}

void MainWindow::on_SendButton_clicked()
{
    read.write(ui->TransmitText->toPlainText().toLatin1());
}
