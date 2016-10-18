#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QMainWindow>
#include "mainwindow.h"
#include "moteuroption.h"
#include <QCheckBox>
#include <QCloseEvent>


namespace Ui {
class Preferences;
}
class MainWindow;
class Music;

class Preferences : public QMainWindow
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = 0, MainWindow* w=0, Music* m=0);
    ~Preferences();
    void closeEvent(QCloseEvent *ce);
    void openPort(bool b, int index);
    QString requestStatus();
    int requestNbreMoteurs();
    int requestNbreConnectes();
    bool requestBoolStatus();
    void actualiserActivatesAndConnected();
    QList<QSerialPort*> serials;
    QString getDefaultTime();
    bool checkPos(int position);
    bool checkSpeed(int speed);
    bool checkAcc(int acc,int dec);
    QString readDataWithDelimiters(QString delimiter, QFile *file);
    void ChangeParameters(QString variable, QString value);
    QString getDefaultPath();
    void aide(bool b);

public slots:
   void openMoteurOption1();
   void openMoteurOption2();
   void openMoteurOption3();
   void openPortSlot1(bool);
   void openPortSlot2(bool);
   void openPortSlot3(bool);
   void activateSlot1(bool);
   void activateSlot2(bool);
   void activateSlot3(bool);
   void bypass();
   void openAll(bool);
   void actionSave();
   void actionLoad();
   void actionLoadExtended();
   void setInfos(double duree,int taille);

private:
    Ui::Preferences *ui;
    MainWindow *w;
    Music* m;
    QList<MoteurOption*> mo;
    QList<QSerialPortInfo> listport;
    QList<QCheckBox*> connectCheckBoxes;
    QList<QComboBox*> portComboBoxes;
    QList<QPushButton*> options;
    QList<QCheckBox*> activates;

    bool status;
};

#endif // PREFERENCES_H
