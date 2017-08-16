#include "serialport.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QString>
SerialPort::SerialPort()
{

}
///*获取串口号相关的键值和键名
QString SerialPort::getcomm(int index,QString keyorvalue)
//index为索引号，keyorvalue为选择返回值应该为key还是value
{
    QString commresult;
    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_READ,&hKey)!=0)
    {
        QString error="Cannot open regedit!";//无法打开注册表时返回error
        return error;
    }
    QString keymessage;//键名
    QString message;
    QString valuemessage;//键值
    indexnum = index;//要读取键值的索引号
    keysize = sizeof(keyname);
    valuesize = sizeof(keyvalue);
    if(::RegEnumValue(hKey,indexnum,keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0)//列举键名和值
    {
        //读取键名
        for(int i=0;i<keysize;i++)
        {
            message = keyname[i];
            keymessage.append(message);
        }
        //读取键值
        for(int j=0;j<valuesize;j++)
        {
             if(keyvalue[j]!=0x00)
             {
                 valuemessage.append(keyvalue[j]);
             }
        }
        if(keyorvalue=="key")
        {
             commresult=keymessage;
        }
        if(keyorvalue=="value")
        {
             commresult=valuemessage;
        }
    }
    else
    {
         commresult="nokey";
    }
    return commresult;
    ::RegCloseKey(hKey);//关闭注册表
}
bool SerialPort::OpenPort(){
    ///*获取串口号相关
        QString path="HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
        QSettings *settings = new QSettings(path,QSettings::NativeFormat);
        QStringList key = settings->allKeys();
        int num=(int)key.size();
        QString value;
        for(int i=0;i<num;i++)
        {
            value = getcomm(i,"value");
            m_listcomboName<<value;

            //添加文本浏览器textBrowser中
            //ui->textBrowser->insertPlainText(value+"\r\n");
            //添加到下拉选择框portNameComboBox中
            //ui->comboBox->addItem(value);
        }
    ///*获取串口号相关
}
