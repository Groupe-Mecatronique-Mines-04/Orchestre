#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation de l'interface
    setFixedSize(QSize(800,600));
    setWindowTitle("Guitar Elec");
    ui->jouer->setEnabled(false);
    ui->init->setEnabled(false);
    ui->seq->setRowCount(0);
    ui->seq->setColumnCount(6);


    QList<QString> header;
    header.append("ID");
    header.append("Position");
    header.append("Vitesse");
    header.append("Accélération");
    header.append("Décélération");
    header.append("Attente");
    ui->seq->setHorizontalHeaderLabels(QStringList(header));
    largeur =80;
    for (int i=1 ; i<ui->seq->columnCount() ; i++) {
    ui->seq->setColumnWidth(i,largeur);
    }
    ui->seq->setColumnWidth(0,30);

    QIcon champ = QIcon("/Users/thibaultalexandre/Desktop/champ.png");
    ui->jouer->setIcon(champ);
    ui->jouer->setText("");

    //Initialisation des données
    pref = new Preferences();
    pref->setMainWindow(this);
    pref->actionLoad();


    seq = new Sequence();
    seq->setPreferences(pref);

    error = new QMessageBox();



    //Connections des SLOTS
    connect(ui->param,SIGNAL(clicked()),this,SLOT(openPreferences()));
    connect(ui->add,SIGNAL(clicked()),this,SLOT(ajouterUneLigne()));
    connect(ui->clear,SIGNAL(clicked()),this,SLOT(vider()));
    connect(ui->load,SIGNAL(clicked()),this,SLOT(openLoad()));
    connect(ui->save,SIGNAL(clicked()),this,SLOT(openSave()));
    connect(ui->jouer,SIGNAL(clicked()),this,SLOT(jouer()));
    connect(ui->init,SIGNAL(clicked()),this,SLOT(init()));
    connect(ui->retirer,SIGNAL(clicked()),this,SLOT(retirer()));
    connect(ui->actionCharger,SIGNAL(triggered()),this,SLOT(openLoad()));
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(openSave()));

    //Actualisation
    actualiserParametres();
    ajouterUneLigne();
    load("/Users/thibaultalexandre/desktop/Séquence/sequence");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPreferences() {
    pref->setInfos(getDureeTotale(),ui->seq->rowCount());
    pref->show();
    setEnabled(false);
}

void MainWindow::actualiserParametres() {
    ui->status->setText(pref->requestStatus());
    ui->nbreconnecte->setText(QString::number(pref->requestNbreConnectes(),10));
    ui->nbremoteur->setText(QString::number(pref->requestNbreMoteurs(),10));
    ui->init->setEnabled(pref->requestBoolStatus());
    ui->jouer->setEnabled(pref->requestBoolStatus());
}
void MainWindow::ajouterUneLigne() {
    int ligne = ui->seq->rowCount();
    ui->seq->setRowCount(ligne+1);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(pref->getDefaultTime());
    ui->seq->setItem(ligne,5,item);
}
void MainWindow::vider() {
    ui->seq->clearContents();
    ui->seq->setRowCount(1);
}

void MainWindow::openLoad() {
QString s = QFileDialog::getOpenFileName(this, tr("Charger un fichier séquence"),"/");
load(s);
}

bool MainWindow::load(QString path) {
    Data *data = new Data();
    QList< QVector<QString> > donnee = data->readData(path);
    if (donnee.size()>0) {
        ui->seq->clearContents();
        ui->seq->setRowCount(donnee.size());
    }

    for (int i=0 ; i<donnee.size() ; i++) {
        for (int j=0 ; j<6 ; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(donnee.at(i).at(j));
            ui->seq->setItem(i,j,item);
        }
    }
}

void MainWindow::openSave() {
   QString s = QFileDialog::getSaveFileName(this,tr("Sauvegarder la séquence"),"/Users/thibaultalexandre/desktop/");
   QList< QVector<QString> > donnee;
    for (int i=0 ; i< ui->seq->rowCount() ; i++) {
        QVector<QString> instruction;
        for (int j=0 ; j<6 ; j++) {
            if (ui->seq->item(i,j) != NULL) {
            QString value = ui->seq->item(i,j)->text();
            instruction.append(value);
            }
        }
        donnee.append(instruction);
    }
    Data *data = new Data();
    data->writeData(s,donnee);
}

QList< QVector<double> > MainWindow::getSeq() {
    QList< QVector<double> > donnee;
    bool ok = false;
    for (int i=0 ; i< ui->seq->rowCount() ; i++) {
        QVector<double> instruction;
        for (int j=0 ; j<6 ; j++) {
            if (ui->seq->item(i,j) != NULL) {
            double value = ui->seq->item(i,j)->text().toDouble(&ok);
            instruction.append(value);
            }
            else {
                instruction.append(0.0);
            }
        }
        donnee.append(instruction);
    }
    return donnee;
}

void MainWindow::jouer() {
if (checkSequence()) {
    QList< QVector<double> > donnee = getSeq();
    for (int i = 0 ; i<donnee.size() ; i++) {
        seq->goToPos(donnee.at(i).at(1),donnee.at(i).at(2),donnee.at(i).at(3),donnee.at(i).at(4),donnee.at(i).at(0)-1);
        seq->delay(donnee.at(i).at(5));
    }
}
else {
    error->setText("Erreur Format Séquence incorrect");
    error->show();
}
}

void MainWindow::init() {
    seq->starting(0);
    seq->delay(1);
    seq->homingOn(0);
    seq->delay(5);
    seq->homingOff(0);
    ui->jouer->setEnabled(true);
}

void MainWindow::retirer() {
    if (ui->seq->rowCount() > 1) {
    ui->seq->setRowCount(ui->seq->rowCount()-1);
    }
}

bool MainWindow::checkSequence() {
    QList< QVector<double> > donnee = getSeq();
    bool ok = true;
    for (int i = 0 ; i<donnee.size() ; i++) {
        ok = ok && (pref->checkPos(donnee.at(i).at(1))) && (pref->checkSpeed(donnee.at(i).at(2))) && (pref->checkAcc(donnee.at(i).at(3),donnee.at(i).at(4)));
    }
    return ok;
}
double MainWindow::getDureeTotale() {
    double duree = 0;
    QList< QVector<double> > donnee = getSeq();
    for (int i=0 ; i<ui->seq->rowCount() ; i++) {
        double temp = donnee.at(i).at(5);
        duree += temp;
    }
    return duree;
}
