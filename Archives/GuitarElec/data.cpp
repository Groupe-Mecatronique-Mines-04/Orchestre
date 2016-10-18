#include "data.h"

Data::Data()
{

}

Data::~Data()
{

}

QList< QVector<QString> > Data::readData(QString nom) {
        QFile file(nom);
        QList <QVector<QString> > seq;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cout << "Erreur" << std::endl;
        }
        else {
        while (!file.atEnd()) {
           QVector<QString> instruction;
           for (int i=0 ; i<6 ; i++) {
               QString s;
               QString next;
               next = file.read(1);
                while (!file.atEnd() && next != ":") {
                    s = s + next;
                    next = file.read(1);
                }
                instruction.append(s);
           }
           seq.append(instruction);
        }
        }
        return seq;
}

QList< QVector<QString> >  Data::readData2(QString nom) {
        QFile file(nom);
        QList <QVector<QString> > donnee;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cout << "Erreur" << std::endl;
        }
        else {
        while (!file.atEnd()) {
           QVector<QString> instruction;
           for (int i=0 ; i<4 ; i++) {
               QString s;
               QString next;
               next = file.read(1);
                while (!file.atEnd() && next != ":") {
                    s = s + next;
                    next = file.read(1);
                }
                instruction.append(s);
           }
           donnee.append(instruction);
        }
        }
        return donnee;
}

void Data::writeData(QString nom, QList< QVector<QString> > donnee) {
      QFile file(nom);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          std::cout << "Erreur écriture" << std::endl;
      }
      else {
         QTextStream out(&file);
          for (int i=0 ; i<donnee.size() ; i++) {
          for (int j=0 ; j<6 ; j++) {
              out << donnee.at(i).at(j);
              out << ":";
          }
          } 
      }
}

void Data::writeData2(QString nom, QList< QVector<QString> > donnee) {
      QFile file(nom);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          std::cout << "Erreur écriture" << std::endl;
      }
      else {
         QTextStream out(&file);
          for (int i=0 ; i<donnee.size() ; i++) {
              for (int j=0 ; j<4 ; j++){
              out << donnee.at(i).at(j);
              out << ":";
              }
          }
          }
      }
