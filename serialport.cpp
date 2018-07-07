#include "serialport.h"
#include <QtCore/QDebug>

serialPort::serialPort(QObject *parent)
{
    connect2SerialPort();
}



serialPort::~serialPort()
{
    serial.close();
}

QString serialPort::device()
{
    return m_device;
}

void serialPort::setDevice(const QString &deviceName){
    m_device= QString(deviceName);
    qDebug() << "Changing device to: " << deviceName << endl;
    serial.close();
    emit deviceChanged();
    firstMessage=true;
    connect2SerialPort();
}




void serialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(serial.portName())
                            .arg(serial.errorString())
                         << endl;

    }
}
void serialPort::handleTimeout()
{
    if (m_message.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available "
                                        "for reading from port %1")
                            .arg(serial.portName())
                         << endl;
    } else {
      /*  qDebug() << QObject::tr("Data successfully received from port %1")
                            .arg(serial.portName())
                         << endl;
        qDebug() << m_message << endl;*/
    }


}


void serialPort::handleReadyRead()
{
    m_message = serial.readAll();

    if (!m_timer.isActive())
        m_timer.start(READ_DELAY);
    emit messageReceived();
    m_status = QString("Connected!");
    if(firstMessage){
        emit statusChanged();
        firstMessage=false;
    }



}

QString serialPort::message()
{
    float res;

    QString msg = QString(m_message);
    QStringList list = msg.split("\r\n");
    res = list.at(0).toFloat();

    QString RES = QString::number(res, 'g', 6);

    return RES;
}

QString serialPort::status()
{
   return m_status;
}

void serialPort::sendMessage(const QString &message)
{

    if (serial.isOpen() && serial.isWritable())
                {
                    QByteArray ba = message.toUtf8();
                    serial.write(ba);
                    serial.flush();
                    qDebug() << "data has been send" << endl;
                    serial.close();
    }
}

void serialPort::connect2SerialPort()
{
    QSerialPortInfo info(m_device);
    // Check info of the port

            qDebug() << "Name        : " << info.portName();
            qDebug() << "Manufacturer: " << info.manufacturer(); //if showing manufacturer, means Qstring &name is good
            qDebug() << "Busy: " << info.isBusy() ;
            qDebug() << "Device" << m_device << endl;

            // Initialize Serial
            serial.setPortName(m_device);
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);

            if (serial.open(QIODevice::ReadWrite))
            {
                qDebug() << "Serial Port opened ttyS3";

                connect(&serial, &QSerialPort::readyRead, this, &serialPort::handleReadyRead);
                connect(&serial, &QSerialPort::errorOccurred, this, &serialPort::handleError);
                connect(&m_timer, &QTimer::timeout, this, &serialPort::handleTimeout);
                m_timer.start(READ_DELAY);
                m_status = QString("Initializing");
                emit statusChanged();

            }
            else
            {
                qDebug() << "Failed to open ttyS3 serial port";
                qDebug() << serial.errorString();
                qDebug() << serial.error();
                m_status = QString("Unable to connect to serial port!");
                emit statusChanged();
            }



}
