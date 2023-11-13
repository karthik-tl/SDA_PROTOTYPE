#ifndef SERIALDATAACQUISITION_H
#define SERIALDATAACQUISITION_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QDebug>
// Add a QTimer for continuous reading
#include <QTimer>

class SerialDataAcquisition : public QObject
{
    Q_OBJECT

public:
    explicit SerialDataAcquisition(QObject *parent = nullptr);

    Q_PROPERTY(QStringList receivedData READ receivedData NOTIFY newDataReceived)

private:
    QString port;
    QSerialPort serial;
    QStringList m_receivedData;
    QString receivedBuffer;  // Add this line

    QTimer *readTimer;  // Add a QTimer for continuous reading

    QStringList receivedData() const {
        return m_receivedData;
    }

signals:
    void newDataReceived(QStringList data);

private slots:
    void readSerialData();
    void handleReadyRead();

public slots:
    QStringList availablePorts();
    void configureSerialPort();
    void connectPort();
    void handleComboBoxSelection(const QString &selectedPort);
};

#endif
