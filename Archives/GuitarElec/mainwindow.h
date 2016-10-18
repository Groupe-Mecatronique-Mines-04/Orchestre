#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "preferences.h"
#include "data.h"
#include "sequence.h"
#include <QFileDialog>
#include <QString>
#include <QVector>
#include <QList>
#include <QMessageBox>
#include <QTableWidget>
#include "partition.h"
#include "music.h"


class Preferences;
class Sequence;
class Music;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actualiserParametres();
    QList< QVector<double> > getSeq();
    bool checkSequence();
    double getDureeTotale();
    void load(QString path);
    Preferences* getPref();
    QTableWidget* getTab();
    void aide(bool b);
    void keyPressEvent(QKeyEvent * event);

public slots:
    void openPreferences();
    void ajouterUneLigne();
    void dupliquer();
    void inserer();
    void vider();
    void openLoad();
    void openPartition();
    void openSave();
    void jouer();
    void init();
    void retirer();
    void stopSlot();
    void homingOff(int);
    void formater(int i, int j);
    void informer();
    void aide();

private:
    Preferences *pref;
    Sequence *seq;
    int largeur;
    QMessageBox *error;
    QMessageBox *info;
    bool stop;
    Music* m;
    Ui::MainWindow *ui;


};

inline Preferences* MainWindow::getPref() {
    return pref;
}

#endif // MAINWINDOW_H
