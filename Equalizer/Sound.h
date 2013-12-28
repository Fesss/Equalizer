
#ifndef SOUND_H
#define	SOUND_H
#include "QSound"
#include "QTimer"
#include "riff.h"
#include "QObject"
#include "Point.h"
#include <stdio.h>
#include <stdlib.h>
#include <fft.h>
#include <cmath>

class Sound : public QObject{
    Q_OBJECT;
public:
    Sound(QString filepath);
    void setFile(QString filepath);
    int getLength(){return (in->buf_size()/in->samplespersec());}; //время воспроизведения в секундах
    QList<Point>* getPoints();
    void setStart(int);
    void readData();
    virtual ~Sound();
    
public slots:
    void reflash();
    
    
private:

    riffwave_reader *in;
    int start;
    int* data;
};

#endif	/* SOUND_H */

