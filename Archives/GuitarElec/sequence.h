#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QByteArray>
#include <iostream>
#include <QTime>
#include <QCoreApplication>
#include "preferences.h"

class Preferences;
class Sequence
{
public:
    Sequence();
    ~Sequence();

    void starting(int ID);
    void homingOn(int ID);
    void homingOff(int ID);
    void goToPos(long n,int ID);
    void goToPos(long pos, long velo, long acc, long dec, int ID);
    int getCount();
    void delay(double n);
    void setPreferences(Preferences *p);
    void write(QByteArray data, int ID);
    void sendMbed(int pos, int vit, int ID);
    void plectre(int pos,int vit);



private:
    int count;
    Preferences *pref;
};

#endif // SEQUENCE_H
