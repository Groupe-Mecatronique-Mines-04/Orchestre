#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation de l'interface
    setFixedSize(QSize(800, 600));
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
    largeur = 80;
    for (int i = 1 ; i < ui->seq->columnCount() ; i++) {
        ui->seq->setColumnWidth(i, largeur);
    }
    ui->seq->setColumnWidth(0, 30);

    //Boutons et icones
    QIcon jouer = QIcon(":/icons/jouer.png");
    ui->jouer->setIcon(jouer);
    ui->jouer->setIconSize(QSize(20, 20));
    ui->jouer->setText("");
    QIcon prefers = QIcon(":/icons/prefers.png");
    ui->param->setIcon(prefers);
    ui->param->setIconSize(QSize(20, 20));
    ui->param->setText("");
    QIcon init = QIcon(":/icons/reinitialiser.png");
    ui->init->setIcon(init);
    ui->init->setIconSize(QSize(20, 20));
    ui->init->setText("");
    QIcon load = QIcon(":/icons/ouvrir.png");
    ui->load->setIcon(load);
    ui->load->setIconSize(QSize(25, 25));
    ui->load->setText("");
    QIcon save = QIcon(":/icons/enregistrer.png");
    ui->save->setIcon(save);
    ui->save->setIconSize(QSize(20, 20));
    ui->save->setText("");
    QIcon add = QIcon(":/icons/ajouter.png");
    ui->add->setIcon(add);
    ui->add->setIconSize(QSize(15, 15));
    ui->add->setText("");
    QIcon retirer = QIcon(":/icons/retirer.png");
    ui->retirer->setIcon(retirer);
    ui->retirer->setIconSize(QSize(15, 15));
    ui->retirer->setText("");
    QIcon vider = QIcon(":/icons/toutsuppr.png");
    ui->clear->setIcon(vider);
    ui->clear->setIconSize(QSize(20, 20));
    ui->clear->setText("");
    QIcon partition = QIcon(":/icons/partition.png");
    ui->partition->setIcon(partition);
    ui->partition->setIconSize(QSize(20, 20));
    ui->partition->setText("");
    QIcon dupliquer = QIcon(":/icons/dupliquer.png");
    ui->duplicate->setIcon(dupliquer);
    ui->duplicate->setIconSize(QSize(20, 20));
    ui->duplicate->setText("");
    QIcon stopIcon = QIcon(":/icons/stop.png");
    ui->pause->setIcon(stopIcon);
    ui->pause->setIconSize(QSize(15, 15));
    ui->pause->setText("");
    QIcon inserer = QIcon(":/icons/inserer.png");
    ui->insert->setIcon(inserer);
    ui->insert->setIconSize(QSize(20, 20));
    ui->insert->setText("");

    //Initialisation des données
    m = new Music();
    m->setMainWindow(this);

    pref = new Preferences(0, this, m);

    seq = new Sequence();
    seq->setPreferences(pref);

    error = new QMessageBox();
    info = new QMessageBox();
    stop = false;
    aide(false);


    //Connections des SLOTS
    connect(ui->param, SIGNAL(clicked()), this, SLOT(openPreferences()));
    connect(ui->add, SIGNAL(clicked()), this, SLOT(ajouterUneLigne()));
    connect(ui->duplicate, SIGNAL(clicked()), this, SLOT(dupliquer()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(vider()));
    connect(ui->load, SIGNAL(clicked()), this, SLOT(openLoad()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(openSave()));
    connect(ui->jouer, SIGNAL(clicked()), this, SLOT(jouer()));
    connect(ui->init, SIGNAL(clicked()), this, SLOT(init()));
    connect(ui->retirer, SIGNAL(clicked()), this, SLOT(retirer()));
    connect(ui->actionCharger, SIGNAL(triggered()), this, SLOT(openLoad()));
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(openSave()));
    connect(ui->pause, SIGNAL(clicked()), this, SLOT(stopSlot()));
    connect(ui->partition, SIGNAL(clicked()), this, SLOT(openPartition()));
    connect(ui->actionCharger_2, SIGNAL(triggered()), this, SLOT(openPartition()));
    connect(ui->actionCharger_2, SIGNAL(triggered()), m, SLOT(load()));
    connect(ui->seq, SIGNAL(cellClicked(int, int)), this, SLOT(formater(int, int)));
    connect(ui->actionPremi_re_fois, SIGNAL(triggered()), this, SLOT(informer()));
    connect(ui->actionCacher_l_aide, SIGNAL(triggered()), this, SLOT(aide()));
    connect(ui->insert, SIGNAL(clicked()), this, SLOT(inserer()));
    //Actualisation
    actualiserParametres();
}


void MainWindow::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_P) {
        jouer();
    }
    std::cout << event->key() << std::endl;
}
void MainWindow::formater(int i, int j) {
    for (int k = 0 ; k < ui->seq->rowCount() ; k++) {
        if (ui->seq->item(k, 0)->text() == "2" || ui->seq->item(k, 0)->text() == "3") {
            ui->seq->item(k, 3)->setBackgroundColor(QColor("grey"));
            ui->seq->item(k, 4)->setBackgroundColor(QColor("grey"));
        }
        else {
            ui->seq->item(k, 3)->setBackgroundColor(QColor("white"));
            ui->seq->item(k, 4)->setBackgroundColor(QColor("white"));
        }
    }
}
void MainWindow::informer() {
    info->setText("1 - Connectez les ports \n2 - Appuyez sur l'interrupteur \n3 - Chargez une séquence \n4 - Appuyez sur play");
    aide(true);
    info->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pref;
    delete seq;
    delete error;
    delete info;
}

void MainWindow::openPreferences() {
    pref->setInfos(getDureeTotale(), ui->seq->rowCount());
    pref->show();
    setEnabled(false);
}

void MainWindow::aide() {
    aide(false);
}

void MainWindow::aide(bool b) {
    ui->aide1->setVisible(b);
    ui->aide2->setVisible(b);
    ui->aide3->setVisible(b);
    ui->aide3_2->setVisible(b);
    ui->aide4->setVisible(b);
    pref->aide(b);
    m->aide(b);
}
void MainWindow::actualiserParametres() {
    ui->status->setText(pref->requestStatus());
    ui->nbreconnecte->setText(QString::number(pref->requestNbreConnectes(), 10));
    ui->nbremoteur->setText(QString::number(pref->requestNbreMoteurs(), 10));
    ui->init->setEnabled(pref->requestBoolStatus());
    ui->jouer->setEnabled(pref->requestBoolStatus());
}
void MainWindow::ajouterUneLigne() {
    int ligne = ui->seq->rowCount();
    ui->seq->setRowCount(ligne + 1);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(pref->getDefaultTime());
    ui->seq->setItem(ligne, 5, item);
    for (int i = 0 ; i < 5 ; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText("0");
        ui->seq->setItem(ligne, i, item);
    }
}
void MainWindow::dupliquer() {
    int ligne = ui->seq->rowCount();
    if (ligne > 0) {
        ui->seq->setRowCount(ligne + 1);
        for (int i = 0 ; i < 6 ; i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(ui->seq->item(ligne - 1, i)->text());
            ui->seq->setItem(ui->seq->rowCount() - 1, i, item);
        }
    }
}
void MainWindow::inserer() {
    int ligne = 0;
    if (ui->seq->selectedItems().size() > 0) {
        ligne = ui->seq->selectedItems().at(0)->row();
    }
    std::cout << ligne << std::endl;
    ui->seq->setRowCount(ui->seq->rowCount() + 1);

    for (int j = ui->seq->rowCount() - 1  ; j > ligne ; j--) {
        for (int i = 0 ; i < 6 ; i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(ui->seq->item(j - 1, i)->text());
            ui->seq->setItem(j, i, item);
        }
    }
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(pref->getDefaultTime());
    ui->seq->setItem(ligne, 5, item);
    for (int i = 0 ; i < 5 ; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText("0");
        ui->seq->setItem(ligne, i, item);
    }
}

void MainWindow::vider() {
    ui->seq->clearContents();
    ui->seq->setRowCount(0);
}

void MainWindow::openLoad() {
    QString s = QFileDialog::getOpenFileName(this, tr("Charger un fichier séquence"), pref->getDefaultPath(), tr("Sequence File (*.seq);;Other (*)"));
    load(s);
}

void  MainWindow::openPartition() {
    m->show();
}


void MainWindow::load(QString path) {
    Data data;
    QList< QVector<QString> > donnee = data.readData(path);
    if (donnee.size() > 0) {
        ui->seq->clearContents();
        ui->seq->setRowCount(donnee.size());
    }

    for (int i = 0 ; i < donnee.size() ; i++) {
        for (int j = 0 ; j < 6 ; j++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(donnee.at(i).at(j));
            ui->seq->setItem(i, j, item);
        }
    }
    formater(0, 0);
}

void MainWindow::openSave() {
    QString s = QFileDialog::getSaveFileName(this, tr("Sauvegarder la séquence"), pref->getDefaultPath(), tr("Sequence File (*.seq)"));
    QList< QVector<QString> > donnee;
    for (int i = 0 ; i < ui->seq->rowCount() ; i++) {
        QVector<QString> instruction;
        for (int j = 0 ; j < 6 ; j++) {
            if (ui->seq->item(i, j) != NULL) {
                QString value = ui->seq->item(i, j)->text();
                instruction.append(value);
            }
        }
        donnee.append(instruction);
    }
    Data *data = new Data();
    data->writeData(s, donnee);
}

QList< QVector<double> > MainWindow::getSeq() {
    QList< QVector<double> > donnee;
    bool ok = false;
    for (int i = 0 ; i < ui->seq->rowCount() ; i++) {
        QVector<double> instruction;
        for (int j = 0 ; j < 6 ; j++) {
            if (ui->seq->item(i, j) != NULL) {
                double value = ui->seq->item(i, j)->text().toDouble(&ok);
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
    for (int k = 0 ; k < 1 ; k++) {
        if (1 || checkSequence()) {
            QList< QVector<double> > donnee = getSeq();
            for (int i = 0 ; i < donnee.size() ; i++) {
                if (stop) {
                    ui->seq->clearSelection();
                    break;
                }
                ui->seq->selectRow(i);
                if (donnee.at(i).at(0) == 1) {
                    seq->goToPos(donnee.at(i).at(1) * 10000, donnee.at(i).at(2) * 10000, donnee.at(i).at(3) * 10000, donnee.at(i).at(4) * 10000, donnee.at(i).at(0) - 1);
                }
                if (donnee.at(i).at(0) == 2 || donnee.at(i).at(0) == 3) {
                    seq->sendMbed(donnee.at(i).at(1), donnee.at(i).at(2), donnee.at(i).at(0));
                }
                if (donnee.at(i).at(0) == 4) {
                    seq->plectre(donnee.at(i).at(1), donnee.at(i).at(2));
                }
                seq->delay(donnee.at(i).at(5) / ((double)k / 5 + 1));
                //  seq->delay(donnee.at(i).at(5));

                if (stop) {
                    stop = false;
                    break;
                }
            }
        }
        else {
            error->setText("Erreur Format Séquence incorrect");
            error->show();
        }
    }
}



void MainWindow::init() {
    seq->starting(0);
    seq->delay(1);
    seq->homingOn(0);
    error->setText("Attendre la fin du homing avant de quitter...");
    error->show();
    connect(error, SIGNAL(finished(int)), this, SLOT(homingOff(int)));
}

void MainWindow::homingOff(int) { //A ameliorer avec requete moteur
    seq->homingOff(0);
    ui->homing->setText("true");
}

void MainWindow::retirer() {
    if (ui->seq->rowCount() > 0) {
        ui->seq->setRowCount(ui->seq->rowCount() - 1);
    }
}

bool MainWindow::checkSequence() {
    QList< QVector<double> > donnee = getSeq();
    bool ok = true;
    for (int i = 0 ; i < donnee.size() ; i++) {
        if (donnee.at(i).at(0) == 1) {
            ok = ok && (pref->checkPos(donnee.at(i).at(1))) && (pref->checkSpeed(donnee.at(i).at(2))) && (pref->checkAcc(donnee.at(i).at(3), donnee.at(i).at(4)));
        }
    }
    return ok;
}
double MainWindow::getDureeTotale() {
    double duree = 0;
    QList< QVector<double> > donnee = getSeq();
    for (int i = 0 ; i < ui->seq->rowCount() ; i++) {
        double temp = donnee.at(i).at(5);
        duree += temp;
    }
    return duree;
}

void MainWindow::stopSlot() {
    stop = !stop;
}

QTableWidget* MainWindow::getTab() {
    return ui->seq;
}
