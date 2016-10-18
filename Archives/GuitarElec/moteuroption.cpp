#include "moteuroption.h"
#include "ui_moteuroption.h"

MoteurOption::MoteurOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoteurOption)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    setFixedSize(QSize(528,315));

    bouton = ui->buttonBox;

    baudRateList.append(QSerialPort::Baud9600);
    baudRateList.append(QSerialPort::Baud19200);
    baudRateList.append(QSerialPort::Baud57600);
    baudRateList.append(QSerialPort::Baud115200);
    baudRateList.append(QSerialPort::BaudRate(1000000));

    dataBitsList.append(QSerialPort::Data5);
    dataBitsList.append(QSerialPort::Data6);
    dataBitsList.append(QSerialPort::Data7);
    dataBitsList.append(QSerialPort::Data8);

    parityList.append(QSerialPort::NoParity);
    parityList.append(QSerialPort::EvenParity);
    parityList.append(QSerialPort::OddParity);
    parityList.append(QSerialPort::MarkParity);
    parityList.append(QSerialPort::SpaceParity);

    stopBitsList.append(QSerialPort::OneStop);
    stopBitsList.append(QSerialPort::OneAndHalfStop);
    stopBitsList.append(QSerialPort::TwoStop);


    flowControlList.append(QSerialPort::NoFlowControl);
    flowControlList.append(QSerialPort::HardwareControl);
    flowControlList.append(QSerialPort::SoftwareControl);


    for (int i=0 ; i<baudRateList.size() ; i++)
    {
        ui->baudRateBox->addItem(baudRateList.at(i).toString());
    }
    ui->baudRateBox->setCurrentIndex(2);

  ui->dataBitsBox->addItem("5");
  ui->dataBitsBox->addItem("6");
  ui->dataBitsBox->addItem("7");
  ui->dataBitsBox->addItem("8");
  ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem("None");
    ui->parityBox->addItem("Even");
    ui->parityBox->addItem("Odd");
    ui->parityBox->addItem("Mark");
    ui->parityBox->addItem("Space");

    ui->stopBitsBox->addItem("1");
    ui->stopBitsBox->addItem("1.5");
    ui->stopBitsBox->addItem("2");

    ui->flowControlBox->addItem("None");
    ui->flowControlBox->addItem("RTS/CTS");
    ui->flowControlBox->addItem("XON/XOFF");

    actualiserConfiguration();

//SIGNAL

    connect(bouton,SIGNAL(accepted()),this,SLOT(ok()));
    connect(bouton,SIGNAL(rejected()),this,SLOT(back()));
}

void MoteurOption::back()
{
    this->close();

}
void MoteurOption::ok()
{
    serial->setBaudRate(baudRateList.at(ui->baudRateBox->currentIndex()).toInt());
    serial->setDataBits(dataBitsList.at(ui->dataBitsBox->currentIndex()));
    serial->setParity(parityList.at(ui->parityBox->currentIndex()));
    serial->setStopBits(stopBitsList.at(ui->stopBitsBox->currentIndex()));
    serial->setFlowControl(flowControlList.at(ui->flowControlBox->currentIndex()));

    actualiserConfiguration();
    this->close();

}

void MoteurOption::setSerial(QSerialPort *serial)
{
    this->serial = serial;
}

void MoteurOption::actualiserBox()
{
    ui->baudRateBox->setCurrentIndex(configuration[0]);
    ui->dataBitsBox->setCurrentIndex(configuration[1]);
    ui->parityBox->setCurrentIndex(configuration[2]);
    ui->stopBitsBox->setCurrentIndex(configuration[3]);
    ui->flowControlBox->setCurrentIndex(configuration[4]);
}

void MoteurOption::actualiserConfiguration()
{
    configuration[0] = ui->baudRateBox->currentIndex();
    configuration[1] = ui->dataBitsBox->currentIndex();
    configuration[2] = ui->parityBox->currentIndex();
    configuration[3] = ui->stopBitsBox->currentIndex();
    configuration[4] = ui->flowControlBox->currentIndex();
}

MoteurOption::~MoteurOption()
{
    delete ui;
    delete bouton;
    delete serial;
}

void MoteurOption::setConfiguration(int baud, int databits, int parity, int stopbits, int flowcontrol){
    configuration[0] = baud;
    configuration[1] = databits;
    configuration[2] = parity;
    configuration[3] = stopbits;
    configuration[4] = flowcontrol;
    actualiserBox();
}
