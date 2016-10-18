#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent,MainWindow* w,Music *m) :
    QMainWindow(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
  this->w = w;
    this->m = m;
//TAB2
    //Initialisation de l'interface
    setWindowTitle("Guitar Elec");
    setFixedSize(QSize(800,480));
    ui->tabWidget->setTabText(0,"Moteurs");
    ui->tabWidget->setTabText(1,"Séquences");
    ui->tabWidget->setCurrentIndex(0);
    ui->info->setText("");
    ui->status->setText("PAS PRÊT");

    QIcon connecter = QIcon(":/icons/connecter.png");
    int size = 30;
    ui->connect1->setIcon(connecter);
    ui->connect1->setIconSize(QSize(size,size));
    ui->connect1->setText("");
    ui->connect2->setIcon(connecter);
    ui->connect2->setIconSize(QSize(size,size));
    ui->connect2->setText("");
    ui->connect3->setIcon(connecter);
    ui->connect3->setIconSize(QSize(size,size));
    ui->connect3->setText("");


    portComboBoxes.append(ui->box1);
    portComboBoxes.append(ui->box2);
    portComboBoxes.append(ui->box3);
    connectCheckBoxes.append(ui->connect1);
    connectCheckBoxes.append(ui->connect2);
    connectCheckBoxes.append(ui->connect3);

    options.append(ui->option1);
    options.append(ui->option2);
    options.append(ui->option3);
    activates.append(ui->activate1);
    activates.append(ui->activate2);
    activates.append(ui->activate3);

   for (int i=0 ; i<3 ; i++) {
       activates.at(i)->setChecked(true);
   }

    listport =  QSerialPortInfo::availablePorts(); //Ajout des ports disponibles
    for (int i=0 ; i<listport.size() ; i++)
    {
        for (int j=0 ; j<3 ; j++) {
            portComboBoxes.at(j)->addItem(listport.at(i).portName());
        }
    }


    //Initialisation des données
    status = false;
    serials.append(new QSerialPort());
    serials.append(new QSerialPort());
    serials.append(new QSerialPort());
    mo.append(new MoteurOption(this));
    mo.append(new MoteurOption(this));
    mo.append(new MoteurOption(this));

    mo.at(0)->setConfiguration(2,3,0,0,0);
    mo.at(1)->setConfiguration(0,3,0,0,0);
    mo.at(2)->setConfiguration(4,3,0,0,0);

    for (int i = 0 ; i<3 ; i++) {
        mo.at(i)->setSerial(serials.at(i));
        mo.at(i)->ok();
    }

    //Connection des SLOTS
    connect(ui->option1,SIGNAL(clicked()),this,SLOT(openMoteurOption1()));
    connect(ui->option2,SIGNAL(clicked()),this,SLOT(openMoteurOption2()));
    connect(ui->option3,SIGNAL(clicked()),this,SLOT(openMoteurOption3()));
    connect(ui->connect1,SIGNAL(clicked(bool)),this,SLOT(openPortSlot1(bool)));
    connect(ui->connect2,SIGNAL(clicked(bool)),this,SLOT(openPortSlot2(bool)));
    connect(ui->connect3,SIGNAL(clicked(bool)),this,SLOT(openPortSlot3(bool)));
    connect(ui->all,SIGNAL(clicked(bool)),this,SLOT(openAll(bool)));
    connect(ui->activate1,SIGNAL(clicked(bool)),this,SLOT(activateSlot1(bool)));
    connect(ui->activate2,SIGNAL(clicked(bool)),this,SLOT(activateSlot2(bool)));
    connect(ui->activate3,SIGNAL(clicked(bool)),this,SLOT(activateSlot3(bool)));
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(actionSave()));
    connect(ui->actionCharger,SIGNAL(triggered()),this,SLOT(actionLoad()));
    connect(ui->actionCharger_2,SIGNAL(triggered()),this,SLOT(actionLoadExtended()));
    connect(ui->bypass,SIGNAL(clicked()),this,SLOT(bypass()));

 //TAB1
   actionLoad();

}

Preferences::~Preferences()
{
    delete ui;
    for (MoteurOption* m : mo) {
        delete m;
    }
    for (QCheckBox* c : connectCheckBoxes) {
        delete c;
    }
    for (QComboBox* m : portComboBoxes) {
        delete m;
    }

    for (QPushButton* m : options) {
             delete m;
    }
    for (QCheckBox* m : activates) {
         delete m;
    }
    QList<QCheckBox*> connectCheckBoxes;
    QList<QComboBox*> portComboBoxes;
    QList<QPushButton*> options;
    QList<QCheckBox*> activates;
}

void Preferences::aide(bool b){
    ui->aide->setVisible(b);
}

void Preferences::closeEvent(QCloseEvent *ce) { //Lancé à la fermeture de Préférences
    w->actualiserParametres();
    w->setEnabled(true);
    ce->accept();
}

void Preferences::openMoteurOption1() {
   mo.at(0)->actualiserBox();
   mo.at(0)->show();
}
void Preferences::openMoteurOption2() {
   mo.at(1)->actualiserBox();
   mo.at(1)->show();
}
void Preferences::openMoteurOption3() {
    mo.at(2)->actualiserBox();
    mo.at(2)->show();
 }
void Preferences::openPortSlot1(bool b) {
    openPort(b,0);
}
void Preferences::openPortSlot2(bool b) {
    openPort(b,1);
}
void Preferences::openPortSlot3(bool b) {
    openPort(b,2);
}
void Preferences::activateSlot1(bool) {
    openPort(false,0);
    actualiserActivatesAndConnected();
}
void Preferences::activateSlot2(bool) {
    openPort(false,1);
    actualiserActivatesAndConnected();
}
void Preferences::activateSlot3(bool) {
    openPort(false,2);
    actualiserActivatesAndConnected();
}
void Preferences::bypass(){
    actualiserActivatesAndConnected();
    if (ui->bypass->isChecked()){
        this->close();
    }
    std::cout << "bypass" << std::endl;
}

void Preferences::openPort(bool b, int i) { //Ouverture d'un port
    if (b && (!portComboBoxes.at(i)->size().isEmpty()))
    {
        serials.at(i)->setPort(listport.at(portComboBoxes.at(i)->currentIndex()));
        if (!serials.at(i)->isOpen()) {
        serials.at(i)->open(QSerialPort::ReadWrite);
        if (serials.at(i)->isOpen())
        {
         connectCheckBoxes.at(i)->setChecked(true);
         options.at(i)->setEnabled(false);
         portComboBoxes.at(i)->setEnabled(false);
         ui->info->setText("");
        }
        else {
            connectCheckBoxes.at(i)->setChecked(false);
            ui->info->setText(QString("Erreur lors de l'ouverture du port ") + serials.at(i)->portName());
        }
        }
    }
    else {
        if (serials.at(i)->isOpen())
        {
            serials.at(i)->close();
            connectCheckBoxes.at(i)->setChecked(false);

            options.at(i)->setEnabled(true);
            portComboBoxes.at(i)->setEnabled(true);
            ui->info->setText("");
        }
    }
    actualiserActivatesAndConnected();
}
void Preferences::openAll(bool b) {
    if (activates.size() >0) {
    for (int i=0 ; i<3 ; i++) {
        if (activates.at(i)->isChecked()) {
        openPort(b,i);
    }
  }
}
}
QString Preferences::requestStatus() {
    return ui->status->text();
}
int Preferences::requestNbreMoteurs() {
    int nbre = 0;
    for (int i=0 ; i<3 ; i++) {
        if (activates.at(i)->isChecked()) {
            nbre++;
        }
    }
    return nbre;
}
int Preferences::requestNbreConnectes() {
    int nbre = 0;
    for (int i=0 ; i<3 ; i++) {
        if (serials.at(i)->isOpen()) {
            nbre++;
        }
    }
    return nbre;
}
bool Preferences::requestBoolStatus() {
    return status;
}

void Preferences::actualiserActivatesAndConnected() { //MAJ Moteurs activés et connectés
    for (int i = 0 ; i<3 ; i++) { //MAJ Activés
        bool b = activates.at(i)->isChecked();
        if (!serials.at(i)->isOpen()) {
        options.at(i)->setEnabled(b);
        portComboBoxes.at(i)->setEnabled(b);
        }
       connectCheckBoxes.at(i)->setEnabled(b);
    }
    bool ready = true; //MAJ du statut
    bool noMoteur = true;
    for (int j=0 ; j<3 ; j++) {
        if ((activates.at(j)->isChecked() && serials.at(j)->isOpen()) || !activates.at(j)->isChecked()) {
            if (activates.at(j)->isChecked()) {
                noMoteur = false;
            }
        }
        else {
            ready = false;
        }
    }
    if (ready && !noMoteur || ui->bypass->isChecked()) {
       ui->status->setText("PRÊT");
       status = true;
    }
    else {
        ui->status->setText("PAS PRÊT");
        status = false;
    }
}

QString Preferences::getDefaultTime() {
    return ui->time->text();
}

bool Preferences::checkPos(int position) {
    bool ok = false;
    int minposition = ui->minPos->text().toInt(&ok,10);
    int maxposition = ui->maxPos->text().toInt(&ok,10);
    return (ok && (position >= minposition) && (position <= maxposition));
}

bool Preferences::checkAcc(int acc, int dec) {
    bool ok = false;
    int maxacc = ui->acc->text().toInt(&ok,10);
    return (ok && (acc <= maxacc) && (dec <= maxacc));
}

bool Preferences::checkSpeed(int speed) {
    bool ok = false;
    int maxspeed = ui->speed->text().toInt(&ok,10);
    return (ok && (speed <= maxspeed));
}

void Preferences::actionSave() {
    QFile file(QCoreApplication::applicationDirPath()+"/pref");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cout << "Erreur écriture" << std::endl;
    }
    else {
       QTextStream out(&file);
        out << "%maxSpeed%=$" + ui->speed->text()+"$\n";
        out << "%maxAcc%=$" + ui->acc->text()+"$\n";
        out << "%minPos%=$" + ui->minPos->text()+"$\n";
        out << "%maxPos%=$" + ui->maxPos->text()+"$\n";
        out << "%defaultTime%=$" + ui->time->text()+"$\n";
        out << "%defaultPath%=$" + ui->path->text()+"$\n";
        if (ui->activate1->isChecked()) {
            out << "%moteur1%=$1$\n";
        }
        else {
            out << "%moteur1%=$0$\n";
        }
        if (ui->activate2->isChecked()) {
            out << "%moteur2%=$1$\n";
        }
        else {
            out << "%moteur2%=$0$\n";
        }
        if (ui->activate3->isChecked()) {
            out << "%moteur3%=$1$\n";
        }
        else {
            out << "%moteur3%=$0$\n";
        } 
      out << "%tempo%=$" + QString::number(m->getTempo())+"$\n";
      out << "%dynamique%=$" + QString::number(m->getDyna())+"$\n";
      out << "%translation%=$" + QString::number(m->getTranslation())+"$\n";


    }
}

void Preferences::actionLoad() {
    QFile file(QCoreApplication::applicationDirPath()+"/pref");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Erreur" << std::endl;
    }
    else {
        QString value;
        QString variable;
    while (!file.atEnd()) {
        variable = readDataWithDelimiters("%",&file);
        value = readDataWithDelimiters("$",&file);
        ChangeParameters(variable,value);
    }
    }
}

void Preferences::actionLoadExtended() {
    QString s = QFileDialog::getOpenFileName(this, tr("Charger un fichier préférences"),"/Users");
    QFile file(s);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Erreur" << std::endl;
    }
    else {
        QString value;
        QString variable;
    while (!file.atEnd()) {
        variable = readDataWithDelimiters("%",&file);
        value = readDataWithDelimiters("$",&file);
        ChangeParameters(variable,value);
    }
    actionSave();
    }

}



QString Preferences::readDataWithDelimiters(QString delimiter, QFile *file) {
    QString next;
    QString value;
    bool record =false;
     while (!file->atEnd()) {
            next = file->read(1);
            if (next == delimiter) {
                if (record) {
                   return value;
                }
                record = !record;
            }
            if (record && (next != delimiter)) {
                value = value + next;
            }
}
     return NULL;
}

void Preferences::ChangeParameters(QString variable, QString value) {
    bool ok =false;
   if (variable == "maxSpeed") {
       ui->speed->setText(value);
   }
   else if (variable == "maxAcc") {
       ui->acc->setText(value);
   }
   else if (variable == "minPos") {
       ui->minPos->setText(value);
   }
   else if (variable == "maxPos") {
       ui->maxPos->setText(value);
   }
   else if (variable == "defaultTime") {
       ui->time->setText(value);
   }
   else if (variable == "defaultPath") {
       ui->path->setText(value);
   }
   else if (variable == "tempo") {
      m->setTempo(value.toDouble(&ok));
   }
   else if (variable == "dynamique") {
       m->setDyna(value.toDouble(&ok));
   }
   else if (variable == "translation") {
       m->setTranslation(value.toDouble(&ok));
   }
   else if (variable == "moteur1") {
       if (value == "1") {
           ui->activate1->setChecked(true);
       }
       else {
           ui->activate1->setChecked(false);
       }
   }
   else if (variable == "moteur2") {
       if (value == "1") {
           ui->activate2->setChecked(true);
       }
       else {
           ui->activate2->setChecked(false);
       }
   }
   else if (variable == "moteur3") {
       if (value == "1") {
           ui->activate3->setChecked(true);
       }
       else {
           ui->activate3->setChecked(false);
       }
   }
}

void Preferences::setInfos(double duree, int taille) {
    char f ='g';
    ui->duree->setText(QString::number(duree,f,2) + " s");
    ui->tailleseq->setText(QString::number(taille,10));
}

QString Preferences::getDefaultPath() {
    return ui->path->text();
}
