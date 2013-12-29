

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

void Sound::setStart(int st) {
    start = (st / 10)*(in->samplespersec())+(st - (st / 10)*10 + 1)*(in->samplespersec() / 10);
}

QList<Point>* Sound::getPoints() {
    QList<Point>* part = new QList<Point > ();
    QList<int>* sorts = new QList<int>();
    int len = 8192,j = 0;
    double xd[8192], xi[8192];
    int sps = in->samplespersec() / 10;
    if ((start + sps) < (in->data_size())) {
        for (int i = start; i < (start + 8192); i++) {
            xd[j] = data[i];
            xi[j] = 0;
            j++;
        }
        cifft(xd, xi, len);
        xd[0] = xd[0] / 2;
        j=0;
        part->append(Point(j,abs(xd[0])/50));
        for (int i = 1; i < 4096; i++) {
            if ((i % 32 == 0) && (i != 0)){
                sorts->append(2 * abs(xd[i]));
                qSort(sorts->begin(), sorts->end());
                j++;
                part->append(Point(j, ( sorts->at(16)+sorts->at(17) )/50 ));
                sorts->clear();
            }else
                sorts->append(2 * abs(xd[i]));
        }
    }
    return part;
}
/*
 * 
+    QList<int>* sorts = new QList<int>();
 +    int i=0;
 +    for (int j = 0; j < in->data_size() ; j++) {
 +        if ((j % 44 == 0) && (j != 0)) {
 +            sorts->append((*in)(j, riffwave_reader::LEFT));
 +            qSort(sorts->begin(), sorts->end());
 +            data[i] = ( (sorts->at(22) + sorts->at(23))/2 );
 +            i++;
 +            sorts->clear();
 +        } else
 +            sorts->append((*in)(j, riffwave_reader::LEFT));
*/


void Sound::readData() {
    int len = in->data_size();
    data = static_cast<int*> (malloc(len * sizeof (int)));
    int i = 0, second = 0;
    for (int j = 0; j < in->data_size(); j++) {
        data[j] = (*in)(j, riffwave_reader::LEFT);
    }
}

void Sound::reflash() {
    start = 0;
}

Sound::~Sound() {
}

