#ifndef DATA_H
#define DATA_H
#include <QObject>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QTextStream>
#include <QVector>


class Data
{
public:
    Data();
    ~Data();

     QList< QVector<QString> > readData(QString nom);
     void writeData(QString nom, QList< QVector<QString> > donnee);


};

#endif // DATA_H
