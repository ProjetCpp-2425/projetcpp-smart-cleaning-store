#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QByteArray>
#include <QDebug>

class Arduino
{
public:
    Arduino(); // Constructor

    QString getarduino_port_name(); // Get the port name
    QSerialPort *getserial(); // Get the serial port object

    int connect_arduino(); // Connect to the Arduino
    int close_arduino(); // Close the connection
    QByteArray read_from_arduino(); // Read data from Arduino
    void write_to_arduino(QByteArray d); // Write data to Arduino

private:
    QString arduino_port_name; // Port name
    bool arduino_is_available; // Arduino availability
    QSerialPort *serial; // Serial object to handle communication
    QByteArray data; // Data to be read or written
    static const int arduino_uno_vendor_id = 0x2341; // Example vendor ID (update if needed)
    static const int arduino_uno_product_id = 0x0043; // Example product ID (update if needed)
};

#endif // ARDUINO_H
