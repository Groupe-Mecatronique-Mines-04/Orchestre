#ifndef PARTITION_H
#define PARTITION_H

#include "melody.h"
#include <QList>
#include <QVector>


class Partition
{
private:
public:
    Partition();
    ~Partition();

    QList< QVector<double> > donnee;
    void load(QString path, int track);
    void      convertToMelody   (MidiFile& midifile, vector<Melody>& melody, int track);
    void      sortMelody        (vector<Melody>& melody);
    void print();



};



#endif // PARTITION_H
