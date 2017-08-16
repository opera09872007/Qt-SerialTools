#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <qt_windows.h>
#include <QString>
#include <QSettings>
class SerialPort
{
public:
    SerialPort();
    bool OpenPort();
    QString getcomm(int index,QString keyorvalue); //读取键名

private:
    ///*获取串口号相关
        QStringList m_listcomboName;
        HKEY hKey;
        LPCWSTR subkey;
        wchar_t keyname[256]; //键名数组
        char keyvalue[256];  //键值数组
        DWORD keysize,type,valuesize;
        int indexnum;
    ///*获取串口号相关
};

#endif // SERIALPORT_H
