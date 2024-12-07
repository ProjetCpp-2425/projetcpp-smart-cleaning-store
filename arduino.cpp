#include "arduino.h"
#include <QDebug>
#include <QSqlQuery>
arduino::arduino()
{
    serial = new QSerialPort(); // Initialize the QSerialPort object

}
int arduino::connect_arduino()
{
    foreach(const QSerialPortInfo &serial_port_info,QSerialPortInfo::availablePorts())
    {
        if(serial_port_info.hasVendorIdentifier()&&serial_port_info.hasProductIdentifier())
        {

            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier() == arduino_uno_product_id)
            {

                arduino_is_available = true;
                arduino_port_name=serial_port_info.portName();
            }
        }
    }
    qDebug()<<"arduino port_name is :"<<arduino_port_name;
    if(arduino_is_available)
    {
        serial->setPortName(arduino_port_name);

        if(serial->open(QSerialPort::ReadWrite))
        {

            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }

    }
    return 1;
}
int arduino::close_arduino()
{
    if(serial->isOpen())
    {
        serial->close();
        return 0;
    }
    return 1;
}
QByteArray arduino::read_from_arduino()
{
    if(serial->isReadable())
    {
        data=serial->readAll();
        return data;
    }
}
QString arduino::getarduino_port_name()
{
    return arduino_port_name;
}
QSerialPort* arduino::getserial()
{
    return serial;
}
bool arduino::checkUIDInDatabase(const QString &uid) {
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE ide = :uid");
    query.bindValue(":uid", uid);

    if (query.exec()) {
        if (query.next()) {
            return true; // UID found in database
        }
    }
    return false; // UID not found
}
