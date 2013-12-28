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
    start=st*1000;
}

QList<Point>* Sound::getPoints() {
    QList<Point>* part = new QList<Point > ();
    int j = 0;
    if ((start + 1000) < (in->data_size()/44) ) {
        for (int i = start; i < (start + 1000); i++) {
            part->append(Point(j, data[i]));
            j++;
        }
    }
    return part;
}

void Sound::readData() {
    int len = in->data_size()/44+1;
    data = static_cast<int*> (malloc(len* sizeof (int)));
    QList<int>* sorts = new QList<int>();
    int i=0;
    for (int j = 0; j < in->data_size() ; j++) {
        if ((j % 44 == 0) && (j != 0)) {
            sorts->append((*in)(j, riffwave_reader::LEFT));
            qSort(sorts->begin(), sorts->end());
            data[i] = ( (sorts->at(22) + sorts->at(23))/2 );
            i++;
            sorts->clear();
        } else
            sorts->append((*in)(j, riffwave_reader::LEFT));
    }
    delete sorts;
}

void Sound::reflash() {
    start = 0;
}

Sound::~Sound() {
}

