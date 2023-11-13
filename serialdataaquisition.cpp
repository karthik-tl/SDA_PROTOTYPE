#include "serialdataaquisition.h"

SerialDataAcquisition::SerialDataAcquisition(QObject *parent)
    : QObject(parent)
{
    configureSerialPort();

    // Initialize and configure the QTimer
    readTimer = new QTimer(this);
    connect(readTimer, &QTimer::timeout, this, &SerialDataAcquisition::readSerialData);
    readTimer->start(1000);  // Set the interval to 1000 milliseconds (1 second)
    connect(&serial, &QSerialPort::readyRead, this, &SerialDataAcquisition::handleReadyRead);
}

void SerialDataAcquisition::configureSerialPort()
{
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
}

QStringList SerialDataAcquisition::availablePorts()
{
    QStringList ports;

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
    {
        ports.append(info.portName());
    }

    return ports;
}

void SerialDataAcquisition::connectPort()
{
    serial.setPortName(port);

    if (serial.open(QIODevice::ReadWrite))
    {
        qDebug() << "Port opened";
        // No need to continuously read here; it will be done through QTimer
    }
    else
    {
        qDebug() << "Port opening failed: " << serial.errorString();
    }
}

void SerialDataAcquisition::handleComboBoxSelection(const QString &selectedPort)
{
    qDebug() << "Selected Port: " << selectedPort;
    port = selectedPort;
}

void SerialDataAcquisition::readSerialData()
{
    // Implement logic for continuous reading here
    // For example, you might want to check for incoming data length and read it
    if (serial.bytesAvailable() > 0)
    {
        handleReadyRead(); // Call handleReadyRead to process the available data
    }
}

// serialdataacquisition.cpp

void SerialDataAcquisition::handleReadyRead()
{
    // Read data from the serial port when readyRead signal is emitted
    QByteArray data = serial.readAll();
    // Process the data as needed
    QString receivedText = QString::fromLatin1(data);

    // Append the received data to the buffer
    receivedBuffer += receivedText;

    // Check if a complete line is received
    int newlineIndex = receivedBuffer.indexOf('\n');
    while (newlineIndex != -1) {
        // Extract the complete line
        QString completeLine = receivedBuffer.left(newlineIndex + 1);
        m_receivedData.append(completeLine);
        emit newDataReceived(m_receivedData);

        // Remove the processed data from the buffer
        receivedBuffer.remove(0, newlineIndex + 1);

        // Check for the next complete line
        newlineIndex = receivedBuffer.indexOf('\n');
    }
}

