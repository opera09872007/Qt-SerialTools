#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>


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
        ui->StateLabel->setText(ui->PortBox->currentText()+tr("打开成功"));
        ui->StateLabel->setStyleSheet("color:green;");

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(ReadComDataSlot()));
        timer->start(100);

    }
}


void MainWindow::on_ClosePortButton_clicked(){
    ui->StateLabel->clear();
    read.clear();
    read.close();
    SetCommBoxTrue();
}

void MainWindow::ReadComDataSlot(){
    QByteArray buf;
    if(read.bytesAvailable()>0){
        ReceiveBytesNumber += read.bytesAvailable();
        buf = read.readAll();
        ui->RxLabel->setText(tr("Rx: ") + QString::number(ReceiveBytesNumber) +  tr(" Bytes"));
        QString str = ui->ReceiveText->toPlainText();
        str += QString::fromLocal8Bit(buf);
        if(ui->AutoLineBox->isChecked()){
            str += "\n";
        }
        ui->ReceiveText->clear();
        ui->ReceiveText->append(str);
        buf.clear();
    }
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
    if(ui->ParityBox->currentText() == tr("None"))
    {
        read.setParity(QSerialPort::NoParity);
    }
    else if(ui->ParityBox->currentText() == tr("Even"))
    {
        read.setParity(QSerialPort::OddParity);
    }
    else if(ui->ParityBox->currentText() == tr("Odd"))
    {
        read.setParity(QSerialPort::EvenParity);
    }
    else if(ui->ParityBox->currentText() == tr("Mark"))
    {
        read.setParity(QSerialPort::EvenParity);
    }
    else if(ui->ParityBox->currentText() == tr("Space"))
    {
        read.setParity(QSerialPort::EvenParity);
    }
    //设置数据位
    if(ui->DataBitsBox->currentText() == tr("5"))
    {
        read.setDataBits(QSerialPort::Data5);
    }
    else if(ui->DataBitsBox->currentText() == tr("6"))
    {
        read.setDataBits(QSerialPort::Data6);
    }
    else if(ui->DataBitsBox->currentText() == tr("7"))
    {
        read.setDataBits(QSerialPort::Data7);
    }
    else if(ui->DataBitsBox->currentText() == tr("8"))
    {
        read.setDataBits(QSerialPort::Data8);
    }
    //设置停止位
    if(ui->StopBitsBox->currentText() == tr("1"))
    {
        read.setStopBits(QSerialPort::OneStop);
    }
    else if(ui->StopBitsBox->currentText() == tr("2"))
    {
        read.setStopBits(QSerialPort::TwoStop);
    }
    else if(ui->StopBitsBox->currentText() == tr("1.5"))
    {
        read.setStopBits(QSerialPort::OneAndHalfStop);
    }
    //设置流量
    if(ui->FlowControlBox->currentText() == tr("None"))
    {
        read.setFlowControl(QSerialPort::NoFlowControl);
    }
    else if(ui->FlowControlBox->currentText() == tr("RTS/CTS"))
    {
        read.setFlowControl(QSerialPort::HardwareControl);
    }
    else if(ui->FlowControlBox->currentText() == tr("XON/XOFF"))
    {
        read.setFlowControl(QSerialPort::SoftwareControl);
    }
}


void MainWindow::SetCommBoxTrue(){
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->DataBitsBox->setEnabled(true);
    ui->StopBitsBox->setEnabled(true);
    ui->FlowControlBox->setEnabled(true);
}
void MainWindow::SetCommBoxFalse(){
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->DataBitsBox->setEnabled(false);
    ui->StopBitsBox->setEnabled(false);
    ui->FlowControlBox->setEnabled(false);

}

void MainWindow::on_SendButton_clicked()
{
    //qDebug()<<ui->TransmitText->toPlainText().toLocal8Bit();
    read.write(ui->TransmitText->toPlainText().toLocal8Bit());
}




void MainWindow::on_ClearButton_clicked()
{
    ui->ReceiveText->clear();
    ui->RxLabel->setText(tr("Rx: 0 Bytes"));
    ui->TxLabel->setText(tr("Tx: 0 Bytes"));
}
