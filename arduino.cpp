#include "arduino.h"
#include <QThread>
#include <QDebug>
#include <QSerialPort>
#include<QEventLoop>
#include <QTimer>
#include <QSqlQuery>
#include <QMessageBox>
Arduino::Arduino()
{
    // Initialize member variables or perform setup
    arduino_is_available = false;
    arduino_port_name = "COM5";

    // Allocate memory for QSerialPort and set its parent to 'this'
    serial = new QSerialPort();
}

 int Arduino:: connect_arduino()
{
     foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts())
     {
         if(serial_port_info.hasVendorIdentifier()&& serial_port_info.hasProductIdentifier())
         {
             if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                     == arduino_uno_producy_id)
             {
                 arduino_is_available = true;
                 arduino_port_name=serial_port_info.portName();
             } } }
     qDebug() << "arduino_port_name is: " << arduino_port_name;
     if(arduino_is_available){
         serial->setPortName(arduino_port_name);
         if(serial->open(QSerialPort::ReadWrite)){
             serial->setBaudRate(QSerialPort::Baud9600);
             serial->setDataBits(QSerialPort::Data8);
             serial->setParity(QSerialPort::NoParity);
             serial->setStopBits(QSerialPort::OneStop);
             serial->setFlowControl(QSerialPort::NoFlowControl);
             return 0;

         }
         return 1;
     }
     return -1;
 }

 QString Arduino:: getarduino_port_name(){
     return arduino_port_name;
 }

 int Arduino:: close_arduino()
 {
     if(serial->isOpen()){
         serial->close();
         return 0;
     }
     return 1;
 }

 QSerialPort* Arduino:: getserial()
 {
    return serial;
 }

 QByteArray Arduino :: read_from_arduino()
 {
     if(serial->isReadable()){
         data=serial->readAll();
         return data;
     }
     return QByteArray();
 }


int Arduino::write_to_arduino(QByteArray d)
{

    if (serial->isWritable()) {
           serial->write(d);
           return 0; // Return 0 to indicate success
       } else {
           qDebug() << "Couldn't write to serial";
           return 1; // Return 1 to indicate failure
       }
}



QString Arduino::read_RFID_card()
{
         QByteArray data;

         // Use QTimer and QEventLoop to introduce a delay before reading data
         QEventLoop loop;
         QTimer::singleShot(1000, &loop, &QEventLoop::quit);
         loop.exec();

         // Read the RFID UID from the Arduino
         data = read_from_arduino();

         // Convert the QByteArray to a QString
         QString uidString = QString::fromUtf8(data);

         // Remove "\r\n" from the end of the string
         uidString = uidString.trimmed();

         // If the string ends with "\r\n", remove it
         if (uidString.endsWith("\r\n")) {
             uidString.chop(2); // Remove last two characters ("\r\n")
         }
         return uidString;

}
