

#include <string>
#include "Sound.h"

Sound::Sound(QString filepath) {
    in = new riffwave_reader(filepath.toStdString().data());
    start = 0;
    readData();
}

void Sound::setFile(QString filepath) {
    delete(in);
    in = new riffwave_reader(filepath.toStdString().data());
    start = 0;
    readData();
}

void Sound::setStart(int st){
    start=(st/10)*(in->samplespersec()/2)+(st-(st/10)*10+1)*(in->samplespersec()/10);
}

QList<Point>* Sound::getPoints() {
    QList<Point>* part = new QList<Point > ();
    int j = 0;
    int sps = in->samplespersec()/5;
    if ((start + sps) < (in->data_size()/2) ) {
        for (int i = start; i < (start + sps); i++) {
            part->append(Point(j, data[i]));
            j++;
        }
    }
    return part;
}

void Sound::readData() {
    int len = in->data_size()/2;
    data = static_cast<int*> (malloc(len* sizeof (int)));
    int i=0,second=0;
    for (int j = 0; j < in->data_size() ; j++) {
        if ((j % 2 == 0) && (j != 0)) {
            data[i] = ( (second + (*in)(j, riffwave_reader::LEFT))/2 );
            i++;
        } else
            second=(*in)(j, riffwave_reader::LEFT);
    }
}

void Sound::reflash() {
    start = 0;
}

Sound::~Sound() {
}

