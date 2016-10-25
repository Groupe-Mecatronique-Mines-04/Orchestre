#include "sequence.h"

Sequence::Sequence()
{
    count = 0;
}

Sequence::~Sequence()
{

}

void Sequence::homingOff(int ID)
{
    const char data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x00, 0x04};
     write(QByteArray(data,sizeof(data)),ID);
}

void Sequence::homingOn(int ID)
{
    const char data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x08, 0x04};
    write(QByteArray(data,sizeof(data)),ID);
}

void Sequence::starting(int ID)
{
    const char data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x00, 0x00, 0x04};
    write(QByteArray(data,sizeof(data)),ID);
}
void Sequence::goToPos(long n,int ID)
{
    QByteArray data;
    data.resize(13);

    int low = n & 0x000000ff;
    int middlelow = (n & 0x0000ff00) >> 8;
    int middlehigh = (n & 0x00ff0000) >> 16;
    int high = (n & 0xff000000) >> 24;

    data[0]  =  0x01;
    data[1]  =  0x3f;
    data[2]  =  0x09; //Length
    data[3]  =  0x02;
    data[4]  =  0x00; //Command ID
    data[5]  =  0x02;
    data[6]  =  getCount(); // Message Sub ID - Count
    data[7]  =  0x02; //Message Main ID
    data[8]  =  low;
    data[9]  =  middlelow;
    data[10] =  middlehigh;
    data[11] =  high;
    data[12] =  0x04; //End

    write(data,ID);
}

void Sequence::goToPos(long pos, long velo, long acc, long dec,int ID)
{
    QByteArray data;
    data.resize(25);

    int lowp = pos & 0x000000ff;
    int middlelowp = (pos & 0x0000ff00) >> 8;
    int middlehighp = (pos & 0x00ff0000) >> 16;
    int highp = (pos & 0xff000000) >> 24;

    int lowv = velo & 0x000000ff;
    int middlelowv = (velo & 0x0000ff00) >> 8;
    int middlehighv = (velo & 0x00ff0000) >> 16;
    int highv = (velo & 0xff000000) >> 24;

    int lowa = acc & 0x000000ff;
    int middlelowa = (acc & 0x0000ff00) >> 8;
    int middlehigha = (acc & 0x00ff0000) >> 16;
    int higha = (acc & 0xff000000) >> 24;

    int lowd = dec & 0x000000ff;
    int middlelowd = (pos & 0x0000ff00) >> 8;
    int middlehighd = (pos & 0x00ff0000) >> 16;
    int highd = (pos & 0xff000000) >> 24;

    data[0] = 0x01;
    data[1] = 0x3f;
    data[2] = 0x15; //Length
    data[3]= 0x02;
    data[4] = 0x00; //Command ID
    data[5] = 0x02;
    data[6] = getCount(); // Message Sub ID - Count
    data[7] = 0x01; //Message Main ID
    data[8]=lowp;
    data[9]=middlelowp;
    data[10]=middlehighp;
    data[11]=highp;
    data[12]=lowv;
    data[13]=middlelowv;
    data[14]=middlehighv;
    data[15]=highv;
    data[16]=lowa;
    data[17]=middlelowa;
    data[18]=middlehigha;
    data[19]=higha;
    data[20]=lowd;
    data[21]=middlelowd;
    data[22]=middlehighd;
    data[23]=highd;
    data[24] = 0x04; //End

    write(data,ID);
}

int Sequence::getCount()
{
    if (count == 15)
    {
        count =0;
    }
    else
    {
        count++;
    }
    return count;
}

void Sequence::delay(double n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n*1000);
    while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Sequence::setPreferences(Preferences *p) {
    pref = p;
}

void Sequence::write(QByteArray data,int ID)
{
    if (pref->serials.at(ID)->isOpen())
    {
        pref->serials.at(ID)->write(data);
    }
}

void Sequence::sendMbed(int pos, int vit, int ID){
    string posString = QString::number(pos,16).toStdString();
    string vitString = QString::number(vit,16).toStdString();
    posString = "0000"+posString;
    posString = posString.substr(posString.size()-4,4);
    vitString = "0000"+vitString;
    vitString = vitString.substr(vitString.size()-4,4);
    string esclave = QString::number(ID-2,10).toStdString();
    const string data ="$0"+esclave+posString+vitString+"#";
    write(QByteArray(data.c_str()),1);

}

void Sequence::plectre(int pos,int vit){
    char lowp = pos & 0x00ff;
    char highp = (pos & 0xff00) >> 8;
    char lowv =  vit & 0x00ff;
    char highv =  (vit & 0xff00) >> 8;
    char t1 = 255;
    char t2 = 255;
    char data[] = {t1, t2, 0x01, 0x07, 0x03, 0x1e, lowp, highp, lowv, highv, 0x00};
    for (int i=2; i<10 ; i++){
        data[10] = data[10]+data[i];
    }
    data[10] = ~data[10];
    write(QByteArray(data,sizeof(data)),2);
}
