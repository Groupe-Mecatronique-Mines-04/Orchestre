#ifndef Music_H
#define Music_H

#include <QMainWindow>
#include "partition.h"
#include <QFileDialog>
#include "mainwindow.h"

class MainWindow;

namespace Ui {
class Music;
}

class Music : public QMainWindow
{
    Q_OBJECT

public:
    explicit Music(QWidget *parent = 0);
    ~Music();
    void setMainWindow(MainWindow* w);
    void modifierLongueur();
    void modifierTension();
    void activerMediator();
    void attendre(double time);
    void modifierTab(int i, int j, QString value);
    void nextInstruction();
    double getTempo();
    void setTempo(double t);
    double getDyna();
    int getTranslation();
    void setTranslation(int t);
    void setDyna(double d);
    double note2T(int note);
    double note2TTensionOnly(int note);
    double frequence2Tension(double f);
    double note2L(int note);
    void aide(bool b);

    double note2Tbis(int note);
    double note2Lbis(int note);
    void actualiserDonnee();

public slots:
    void load();
    void load2();
    void changeTrack(int piste);
    void save();
    void save2();
    void convertir();
    void convertirTensionOnly();
    void vider();
    void ajouter();
    void retirer();
    void inserer();

private:
    Ui::Music *ui;
    QList<Partition*> music;
    QList<QList< QVector<double> > > metadonnee;
    QList< QVector<double> > donneeutile;
    MainWindow* w;
    double start;
    double note;
    double duree;
    double velocity;
    double actiontime;
    double epsilon;
    int translation=-20;
    int num_instruction;
    bool up = true;
};

#endif // MUSIC_H
