#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

QTimer *timer1 = new QTimer();
QTimer *timer2 = new QTimer();
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
    myport.close();
    myport.setPortName(ui->PortBox->currentText());
    if(!myport.open(QIODevice::ReadWrite)){
        QMessageBox::warning(this,tr("error"),tr("串口打开失败"),QMessageBox::Ok);
        ui->StateLabel->setText(ui->PortBox->currentText()+"打开失败");
        ui->StateLabel->setStyleSheet("color:red;");
    }
    else{
        ui->StateLabel->setText(ui->PortBox->currentText()+tr("打开成功"));
        ui->StateLabel->setStyleSheet("color:green;");

        timer1->stop();
        timer2->stop();
        myport.clearError();
        myport.clear();
        SetSeriaPortAttribute();
        SetCommBoxFalse();
        connect(timer1, SIGNAL(timeout()), this, SLOT(ReadComDataSlot()));
        timer1->start(100);


        connect(ui->ReceiveAsTextButton, SIGNAL(toggled(bool)), this, SLOT(ReceiveAsTextButtonSlot()));
        connect(ui->ReceiveAsHexButton, SIGNAL(toggled(bool)), this, SLOT(ReceiveAsHexButtonSlot()));
    }
}


void MainWindow::on_ClosePortButton_clicked(){
    ui->StateLabel->clear();
    myport.clear();
    myport.close();
    SetCommBoxTrue();
}

void MainWindow::ReadComDataSlot(){
    QByteArray buf;
    if(myport.bytesAvailable()>0){
        ReceiveBytesNumber += myport.bytesAvailable();
        buf = myport.readAll();
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

void MainWindow::ReadComDataSlot2(){
    QByteArray buf;
    if(myport.bytesAvailable()>0){
        ReceiveBytesNumber += myport.bytesAvailable();
        buf = myport.readAll();
        ui->RxLabel->setText(tr("Rx: ") + QString::number(ReceiveBytesNumber) +  tr(" Bytes"));
        QString str = ui->ReceiveText->toPlainText();
        str += buf.toHex();
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
    myport.setBaudRate(ui->BaudBox->currentText().toInt());
    //设置校验位
    if(ui->ParityBox->currentText() == tr("None"))
    {
        myport.setParity(QSerialPort::NoParity);
    }
    else if(ui->ParityBox->currentText() == tr("Even"))
    {
        myport.setParity(QSerialPort::OddParity);
    }
    else if(ui->ParityBox->currentText() == tr("Odd"))
    {
        myport.setParity(QSerialPort::EvenParity);
    }
    else if(ui->ParityBox->currentText() == tr("Mark"))
    {
        myport.setParity(QSerialPort::EvenParity);
    }
    else if(ui->ParityBox->currentText() == tr("Space"))
    {
        myport.setParity(QSerialPort::EvenParity);
    }
    //设置数据位
    if(ui->DataBitsBox->currentText() == tr("5"))
    {
        myport.setDataBits(QSerialPort::Data5);
    }
    else if(ui->DataBitsBox->currentText() == tr("6"))
    {
        myport.setDataBits(QSerialPort::Data6);
    }
    else if(ui->DataBitsBox->currentText() == tr("7"))
    {
        myport.setDataBits(QSerialPort::Data7);
    }
    else if(ui->DataBitsBox->currentText() == tr("8"))
    {
        myport.setDataBits(QSerialPort::Data8);
    }
    //设置停止位
    if(ui->StopBitsBox->currentText() == tr("1"))
    {
        myport.setStopBits(QSerialPort::OneStop);
    }
    else if(ui->StopBitsBox->currentText() == tr("2"))
    {
        myport.setStopBits(QSerialPort::TwoStop);
    }
    else if(ui->StopBitsBox->currentText() == tr("1.5"))
    {
        myport.setStopBits(QSerialPort::OneAndHalfStop);
    }
    //设置流量
    if(ui->FlowControlBox->currentText() == tr("None"))
    {
        myport.setFlowControl(QSerialPort::NoFlowControl);
    }
    else if(ui->FlowControlBox->currentText() == tr("RTS/CTS"))
    {
        myport.setFlowControl(QSerialPort::HardwareControl);
    }
    else if(ui->FlowControlBox->currentText() == tr("XON/XOFF"))
    {
        myport.setFlowControl(QSerialPort::SoftwareControl);
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



void MainWindow::on_ClearButton_clicked()
{
    ui->ReceiveText->clear();
    ReceiveBytesNumber = 0;
    TransmitBytesNumber = 0;
    ui->RxLabel->setText(tr("Rx: 0 Bytes"));
    ui->TxLabel->setText(tr("Tx: 0 Bytes"));
}


void MainWindow::on_TransmitButton_clicked()
{
    TransmitBytesNumber += ui->TransmitText->toPlainText().toLocal8Bit().size();
    ui->TxLabel->setText(tr("Rx: ") + QString::number(TransmitBytesNumber) +  tr(" Bytes"));
    if(ui->TransmitAsTextButton->isChecked()){
       //qDebug()<<ui->TransmitText->toPlainText().toLocal8Bit();
       myport.write(ui->TransmitText->toPlainText().toLocal8Bit());
    }
    else if(ui->TransmitAsHexButton->isChecked()){
       //qDebug()<<ui->TransmitText->toPlainText().toLatin1().toHex();
       myport.write(ui->TransmitText->toPlainText().toLatin1().toHex());
    }
}


void MainWindow::ReceiveAsHexButtonSlot(){
    timer1->stop();
    myport.clearError();
    myport.clear();
    SetSeriaPortAttribute();

    SetCommBoxFalse();

    connect(timer2, SIGNAL(timeout()), this, SLOT(ReadComDataSlot2()));
    timer2->start(100);
}
void MainWindow::ReceiveAsTextButtonSlot(){
    timer2->stop();
    timer1->stop();
    myport.clearError();
    myport.clear();
    SetSeriaPortAttribute();

    SetCommBoxFalse();

    connect(timer1, SIGNAL(timeout()), this, SLOT(ReadComDataSlot()));
    timer1->start(100);
}
