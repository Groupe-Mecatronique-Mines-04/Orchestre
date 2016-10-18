#include "music.h"
#include "ui_music.h"
#include <math.h>


Music::Music(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Music)
{
    ui->setupUi(this);

    //Boutons et icônes
    QIcon inserer = QIcon(":/icons/inserer.png");
    ui->insert->setIcon(inserer);
    ui->insert->setIconSize(QSize(20,20));
    ui->insert->setText("");

    QIcon save = QIcon(":/icons/enregistrer.png");
    ui->sauver->setIcon(save);
    ui->sauver->setIconSize(QSize(20,20));
    ui->sauver->setText("");
    ui->sauver_2->setIcon(save);
    ui->sauver_2->setIconSize(QSize(20,20));
    ui->sauver_2->setText("");


    QIcon jouer = QIcon(":/icons/jouer.png");
    ui->convertir->setIcon(jouer);
    ui->convertir->setIconSize(QSize(20,20));
    ui->convertir->setText("");
    QIcon load = QIcon(":/icons/ouvrir.png");
    ui->charger->setIcon(load);
    ui->charger->setIconSize(QSize(25,25));
    ui->charger->setText("");
    ui->charger_2->setIcon(load);
    ui->charger_2->setIconSize(QSize(25,25));
    ui->charger_2->setText("");
    QIcon add = QIcon(":/icons/ajouter.png");
    ui->add->setIcon(add);
    ui->add->setIconSize(QSize(15,15));
    ui->add->setText("");
    QIcon retirer = QIcon(":/icons/retirer.png");
    ui->retirer->setIcon(retirer);
    ui->retirer->setIconSize(QSize(15,15));
    ui->retirer->setText("");
    QIcon vider = QIcon(":/icons/toutsuppr.png");
    ui->clear->setIcon(vider);
    ui->clear->setIconSize(QSize(20,20));
    ui->clear->setText("");

    ui->seq->setRowCount(0);
    ui->seq->setColumnCount(4);
    ui->piste->addItem("0");
    ui->piste->addItem("1");
    ui->piste->addItem("2");
    ui->piste->addItem("3");
    ui->piste->addItem("4");
    ui->piste->addItem("5");
    ui->piste->addItem("6");
    ui->piste->addItem("7");
    ui->piste->addItem("8");
    ui->piste->addItem("9");
    ui->piste->addItem("10");
    ui->piste->addItem("11");
    ui->piste->addItem("12");
    ui->piste->addItem("13");
    ui->piste->addItem("14");
    ui->piste->addItem("15");
    ui->piste->addItem("16");



    setWindowTitle("Editeur de Partition");


    QList<QString> header;
    header.append("Start");
    header.append("Note");
    header.append("Duration");
    header.append("Velocity");
    ui->seq->setHorizontalHeaderLabels(QStringList(header));
    int largeur =80;
    ui->dyna->setText("0");
    for (int i=0 ; i<ui->seq->columnCount() ; i++) {
    ui->seq->setColumnWidth(i,largeur);
    }
//    ui->seq->setEditTriggers( QAbstractItemView::NoEditTriggers );


    connect(ui->charger, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->charger_2, SIGNAL(clicked()), this, SLOT(load2()));

    connect(ui->piste,SIGNAL(currentIndexChanged(int)),this,SLOT(changeTrack(int)));
    connect(ui->sauver,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->sauver_2,SIGNAL(clicked()),this,SLOT(save2()));

    connect(ui->convertir,SIGNAL(clicked()),this,SLOT(convertir()));
    connect(ui->actionConvertir,SIGNAL(triggered()),this,SLOT(convertir()));
    connect(ui->actionCharger,SIGNAL(triggered()),this,SLOT(load()));
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->insert,SIGNAL(clicked()),this,SLOT(inserer()));
     connect(ui->retirer,SIGNAL(clicked()),this,SLOT(retirer()));
     connect(ui->add,SIGNAL(clicked()),this,SLOT(ajouter()));
     connect(ui->clear,SIGNAL(clicked()),this,SLOT(vider()));

}
void Music::actualiserDonnee() {
   donneeutile.clear();
    bool ok = false;
    for (int i=0 ; i< ui->seq->rowCount() ; i++) {
        QVector<double> instruction;
        for (int j=0 ; j<4 ; j++) {
            if (ui->seq->item(i,j) != NULL) {
            double value = ui->seq->item(i,j)->text().toDouble(&ok);
            instruction.append(value);
            }
            else {
                instruction.append(0.0);
            }
        }
        donneeutile.append(instruction);
    }
}

void Music::vider() {
    ui->seq->clearContents();
    ui->seq->setRowCount(0);
}
void Music::retirer() {
    if (ui->seq->rowCount() > 0) {
    ui->seq->setRowCount(ui->seq->rowCount()-1);
    }
}
void Music::ajouter() {
    int ligne = ui->seq->rowCount();
    ui->seq->setRowCount(ligne+1);
    if (ligne > 0) {
    for (int i=0 ; i<4 ; i++) {
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(ui->seq->item(ligne-1,i)->text());
    ui->seq->setItem(ui->seq->rowCount()-1,i,item);
    }
    }
    else {
        for (int i=0 ; i<4 ; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText("0");
        ui->seq->setItem(ligne,i,item);
        }

    }
}
void Music::inserer(){
    int ligne = 0;
    if (ui->seq->selectedItems().size()>0) {
        ligne = ui->seq->selectedItems().at(0)->row();
    }
    std::cout << ligne << std::endl;
    ui->seq->setRowCount(ui->seq->rowCount()+1);

    for (int j=ui->seq->rowCount()-1  ; j>ligne ; j--) {
        for (int i=0 ; i<4 ; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(ui->seq->item(j-1,i)->text());
        ui->seq->setItem(j,i,item);
        }
    }
    for (int i=0 ; i<4 ; i++){
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText("0");
        ui->seq->setItem(ligne,i,item);
    }
}


Music::~Music()
{
    delete ui;
for (Partition* p : music)
 {
    delete p;
 }
}

void Music::aide(bool b){
    ui->aide->setVisible(b);
    ui->aide_2->setVisible(b);
}

void Music::load() {
    music.clear();
    metadonnee.clear();
    QString s = QFileDialog::getOpenFileName(this, tr("Charger un fichier Midi"),w->getPref()->getDefaultPath(),tr("Midi File (*.mid);;Other (*)"));
    for (int i=0 ; i<= 16 ; i++) {
        Partition* p = new Partition();
        p->load(s,i);
        music.append(p);
        metadonnee.append(p->donnee);
    }
    changeTrack(ui->piste->currentIndex());
    }

void Music::load2(){
    metadonnee.clear();
    bool ok = false;
    QString path = QFileDialog::getOpenFileName(this, tr("Charger un fichier Guitar Elec"),w->getPref()->getDefaultPath(),tr("Guitar File (*.gui);;Other (*)"));
    Data data;
    QList< QVector<QString> > donnee = data.readData2(path);
    QList< QVector<double> > donnee2;
    QVector<double> line;
    ui->piste->setCurrentIndex(0);
    if (donnee.size()>0) {
        ui->seq->clearContents();
        ui->seq->setRowCount(donnee.size());
    }

    for (int i=0 ; i<donnee.size() ; i++) {
        for (int j=0 ; j<4 ; j++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(donnee.at(i).at(j));
            ui->seq->setItem(i,j,item);
            line.append(donnee.at(i).at(j).toDouble(&ok));
        }
        donnee2.append(line);
    }
    metadonnee.append(donnee2);
}

void Music::setMainWindow(MainWindow *w) {
    this->w = w;
}
void Music::setTranslation(int t){
    ui->translate->setText(QString::number(t));
}
int Music::getTranslation(){
    bool ok = false;
    return ui->translate->text().toInt(&ok);
}

void Music::changeTrack(int piste) {
        ui->piste->setCurrentIndex(piste);
        std::cout << piste << std::endl;
       int num_track = metadonnee.size();
       if (piste<num_track){
       ui->seq->setRowCount(metadonnee.at(piste).size());
       for (int i=0 ; i<metadonnee.at(piste).size() ; i++) {
           for (int j=0 ; j < 4 ; j++) {
               QTableWidgetItem* item = new QTableWidgetItem();
               item->setText(QString::number(metadonnee.at(piste).at(i).at(j)));
               ui->seq->setItem(i,j,item);
           }
           }
       }

}
void Music::save(){
    QString name = QFileDialog::getSaveFileName(this,tr("Sauvegarder la séquence"),w->getPref()->getDefaultPath(),tr("Midi File (*.mid)"));
      MidiFile outputfile;      // create an empty MIDI file with one track
      outputfile.absoluteTicks();  // time information stored as absolute time
                                  // (will be coverted to delta time when written)
      outputfile.addTrack(16);     // Add another two tracks to the MIDI file
      vector<uchar> midievent;     // temporary storage for MIDI events

      int num_track = music.size();

for (int j=0 ; j<num_track ; j++) {
      int actiontime = 0;      // temporary storage for MIDI event time
      QList <QVector<double>> donnee =music.at(j)->donnee;
      outputfile.addTrackName(j, actiontime, "Line");
      outputfile.addPatchChange(j, actiontime, j, 78); // 58=tuba

      if (donnee.size()>0){
      for (int i=0 ; i<donnee.size() ; i++) {
         actiontime = donnee.at(i).at(0);
         outputfile.addNoteOn(j, actiontime, j, donnee.at(i).at(1), donnee.at(i).at(3));
         actiontime += donnee.at(i).at(2);
         outputfile.addNoteOff(j, actiontime, j, donnee.at(i).at(1), donnee.at(i).at(3));
      }
      }
}
      outputfile.sortTracks();         // make sure data is in correct order
      outputfile.write(name.toStdString()); // write Standard MIDI File twinkle.mid
}

void Music::save2(){
    QString s = QFileDialog::getSaveFileName(this,tr("Sauvegarder la partition"),w->getPref()->getDefaultPath(),tr("Guitar File (*.gui)"));
    QList< QVector<QString> > donnee;
     for (int i=0 ; i< ui->seq->rowCount() ; i++) {
         QVector<QString> instruction;
         for (int j=0 ; j<4 ; j++) {
             if (ui->seq->item(i,j) != NULL) {
             QString value = ui->seq->item(i,j)->text();
             instruction.append(value);
             }
         }
         donnee.append(instruction);
     }
     Data *data = new Data();
     data->writeData2(s,donnee);
}

void Music::convertir(){
    bool ok = false;
    int track =ui->piste->currentIndex();
        actualiserDonnee();
        if (donneeutile.size()>0) {

            actiontime=0;
            epsilon=0;
            num_instruction = 0;
            w->getTab()->setRowCount(1);

               for (int i=0 ; i<donneeutile.size() ; i++) {
                   start = donneeutile.at(i).at(0);
                   note = donneeutile.at(i).at(1);
                   duree = donneeutile.at(i).at(2);
                   velocity = donneeutile.at(i).at(3);
                   epsilon=(ui->dyna->text().toDouble(&ok))/100*duree;

                   if ((start-epsilon-actiontime)>0) {
                       attendre(start-epsilon-actiontime);
                   }
                    modifierLongueur();
                    modifierTension();
                    activerMediator();
               }
               w->getTab()->setRowCount(w->getTab()->rowCount()-1);
            }
        this->hide();

}

void Music::convertirTensionOnly(){
    bool ok = false;
    int track =ui->piste->currentIndex();
    if (music.size()>0){
    if (music.at(track)->donnee.size()!=0){
    this->hide();
    actiontime=0;
    epsilon=0;
    num_instruction = 0;
    w->getTab()->setRowCount(1);

       for (int i=0 ; i<music.at(track)->donnee.size() ; i++) {
           start = music.at(track)->donnee.at(i).at(0);
           note = music.at(track)->donnee.at(i).at(1);
           duree = music.at(track)->donnee.at(i).at(2);
           velocity = music.at(track)->donnee.at(i).at(3);
           epsilon=(ui->dyna->text().toDouble(&ok))/100*duree;

           if ((start-epsilon-actiontime)>0) {
               attendre(start-epsilon-actiontime);
           }
           double tension = note2TTensionOnly(note);
           modifierTab(num_instruction,0,"2");
           modifierTab(num_instruction,1, QString::number(tension));
           modifierTab(num_instruction,2,"none");
           modifierTab(num_instruction,3,"none");
           modifierTab(num_instruction,4,"none");
           modifierTab(num_instruction,5,QString::number(epsilon));
           nextInstruction();
         //   activerMediator();
       }
       w->getTab()->setRowCount(w->getTab()->rowCount()-1);
    }
    }
}

void Music::modifierLongueur() {
    bool ok = false;
    modifierTab(num_instruction,0,"1");
    modifierTab(num_instruction,1, QString::number(note2Lbis(note+ui->translate->text().toDouble(&ok))));
    modifierTab(num_instruction,2,"100");
    modifierTab(num_instruction,3,"100");
    modifierTab(num_instruction,4,"100");
    modifierTab(num_instruction,5,"0");
    nextInstruction();
}

void Music::modifierTension() {
    bool ok = false;
    modifierTab(num_instruction,0,"2");
    modifierTab(num_instruction,1, QString::number(note2Tbis(note+ui->translate->text().toDouble(&ok))));
    modifierTab(num_instruction,2,"none");
    modifierTab(num_instruction,3,"none");
    modifierTab(num_instruction,4,"none");
    modifierTab(num_instruction,5,QString::number(epsilon));
    nextInstruction();
}

void Music::activerMediator(){
    modifierTab(num_instruction,0,"4");
    if (up) {
    modifierTab(num_instruction,1,"520");
    up = false;
    }
    else{
        modifierTab(num_instruction,1,"570");
        up=true;
    }
    modifierTab(num_instruction,2,"800");
    modifierTab(num_instruction,3,"none");
    modifierTab(num_instruction,4,"none");
    modifierTab(num_instruction,5,QString::number(duree-epsilon));
    nextInstruction();
}

void Music::modifierTab(int i, int j, QString value){
    QTableWidgetItem* item1 = new QTableWidgetItem();
    if (j==5){
        bool ok = false;
        double time = value.toDouble(&ok);
        actiontime+=time;
        time = time/(ui->tempo->text().toDouble(&ok)*2);
        value = QString::number(time);
    }
    item1->setText(value);
    w->getTab()->setItem(i,j,item1);
 }

void Music::nextInstruction(){
    num_instruction +=1;
    w->getTab()->setRowCount(w->getTab()->rowCount()+1);
}

void Music::attendre(double time){
    modifierTab(num_instruction,0,"0");
    modifierTab(num_instruction,1,"none");
    modifierTab(num_instruction,2,"none");
    modifierTab(num_instruction,3,"none");
    modifierTab(num_instruction,4,"none");
    modifierTab(num_instruction,5,QString::number(time));
    nextInstruction();
}

double Music::getTempo(){
    bool ok = false;
    return (ui->tempo->text().toDouble(&ok));
}

void Music::setTempo(double t){
   ui->tempo->setText(QString::number(t));
}

double Music::getDyna(){
    bool ok = false;
    return (ui->dyna->text().toDouble(&ok));
}

void Music::setDyna(double d){
    ui->dyna->setText(QString::number(d));
}

double Music::frequence2Tension(double f){
    return (382.86*f+16600);
}

double Music::note2TTensionOnly(int note){
    double power = (double)(note-69)/((double)12);
    std::cout << power << std::endl;
    double frequence = pow(2,power)*440;
    std::cout << "Frequence " << frequence << std::endl;
    return (frequence*frequence);
}

double Music::note2Lbis(int note){
    switch (note) {
    case 48:
        return 387;
        break;
    case 49:
        return 370;
        break;
    case 50:
        return 342;
        break;
    case 51:
        return 315;
        break;
    case 52:
        return 283;
        break;
    case 53:
        return 256;
        break;
    case 54:
        return 240;
        break;
    case 55:
        return 215;
        break;
    case 56:
        return 390;
        break;
    case 57:
        return 355;
        break;
    case 58:
        return 330;
        break;
    case 59:
        return 300;
        break;
    case 60:
        return 275;
        break;
    case 61:
        return 250;
        break;
    case 62:
        return 230;
        break;
    case 63:
        return 210;
        break;
    case 64:
        return 188;
        break;
    case 65:
        return 171;
        break;
    case 66:
        return 150;
        break;
    case 67:
        return 135;
        break;
    case 68:
        return 117;
        break;

    default:
       return -1;
       break;
    }
}
    double Music::note2Tbis(int note){
        switch (note) {

        case 48:
            return 16500;
            break;
        case 49:
            return 16500;
            break;
        case 50:
            return 16500;
            break;
        case 51:
            return 16500;
            break;
        case 52:
            return 16500;
            break;
        case 53:
            return 16500;
            break;
        case 54:
            return 16500;
            break;
        case 55:
            return 16500;
            break;
        case 56:
            return 50000;
            break;
        case 57:
            return 50000;
            break;
        case 58:
            return 50000;
            break;
        case 59:
            return 50000;
            break;
        case 60:
            return 50000;
            break;
        case 61:
            return 50000;
            break;
        case 62:
            return 50000;
            break;
        case 63:
            return 50000;
            break;
        case 64:
            return 50000;
            break;
        case 65:
            return 50000;
            break;
        case 66:
            return 50000;
            break;
        case 67:
            return 50000;
            break;
        case 68:
            return 50000;
            break;

        default:
           return -1;
           break;
        }
    }
