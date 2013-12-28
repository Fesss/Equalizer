

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
    int len=8192;
    double xd[8192], xi[8192];
    int j = 0;
    int sps = in->samplespersec()/5;
    if ((start + sps) < (in->data_size()) ) {
        for (int i = start; i < (start + 8192); i++) {
            xd[j]=data[i];
            xi[j]=0;
            j++;
        }
        cifft(xd,xi,len);
        xd[0]=xd[0]/4;
        for(int i=0;i<4096;i++)
            part->append(Point(i, 2*abs(xd[i])/500));
    }
    return part;
}

void Sound::readData() {
    int len = in->data_size();
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

