#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>


#define USB_DEVICE "/dev/ttyUSB0"
#define READ_DELAY 100


class serialPort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE sendMessage NOTIFY messageReceived)
    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit serialPort(QObject *parent = nullptr);
    ~serialPort();

    QString device();
    QString message();
    QString status();

    void setDevice(const QString &deviceName);
    void sendMessage(const QString &message);

    void connect2SerialPort();

signals:
    void messageReceived();
    void deviceChanged();
    void statusChanged();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleTimeout();

private:
    QSerialPort serial;
    QByteArray m_message;
    QTimer m_timer;
    QString m_device = QString(USB_DEVICE);
    QString m_status;
    bool firstMessage=true;
};

#endif // SERIALPORT_H
