
#ifndef SOUND_H
#define	SOUND_H
#include "QSound"
#include "QTimer"
#include "riff.h"
#include "QObject"

class Sound : public QObject{
    Q_OBJECT;
public:
    Sound(QString filepath);
    void setFile(QString filepath);
    int getLength(){return (in->buf_size()/in->samplespersec());}; //время воспроизведения в секундах
    virtual ~Sound();
    
public slots:
    void play();
    void pause();
    void timeUP();
    
signals:
    void secUP();
    
    
private:
    QSound *sng;
    QTimer *timer;
    riffwave_reader *in;
};

#endif	

