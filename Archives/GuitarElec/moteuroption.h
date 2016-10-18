#ifndef MOTEUROPTION_H
#define MOTEUROPTION_H

#include <QDialog>
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDialogButtonBox>
#include <iostream>

namespace Ui {
class MoteurOption;
}

class MoteurOption : public QDialog
{
    Q_OBJECT

public:
    explicit MoteurOption(QWidget *parent = 0);
    ~MoteurOption();

    void setSerial(QSerialPort *serial);
    void actualiserConfiguration();
    void actualiserBox();
    void setConfiguration(int baud, int databits, int parity, int stopbits, int flowcontrol);

public slots:
    void back();
    void ok();

private:
    Ui::MoteurOption *ui;
    QList<QVariant> baudRateList;
    QList<QSerialPort::DataBits> dataBitsList;
    QList<QSerialPort::Parity> parityList;
    QList<QSerialPort::StopBits> stopBitsList;
    QList<QSerialPort::FlowControl> flowControlList;
    int configuration[5];

    QDialogButtonBox *bouton;
    QSerialPort *serial;

};

#endif // MOTEUROPTION_H
