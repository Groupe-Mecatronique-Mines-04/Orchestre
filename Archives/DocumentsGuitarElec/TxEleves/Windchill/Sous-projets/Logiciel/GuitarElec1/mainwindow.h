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


class Preferences;
class Sequence;

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
    bool load(QString path);

public slots:
    void openPreferences();
    void ajouterUneLigne();
    void vider();
    void openLoad();
    void openSave();
    void jouer();
    void init();
    void retirer();

private:
    Ui::MainWindow *ui;
    Preferences *pref;
    Sequence *seq;
    int largeur;
    QMessageBox *error;

};

#endif // MAINWINDOW_H
