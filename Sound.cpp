
#include <string>
#include "Sound.h"

Sound::Sound(QString filepath) {
    sng = new QSound(filepath);
    in = new riffwave_reader(filepath.toStdString().data());
    timer = new QTimer();
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUP()));
}

void Sound::setFile(QString filepath){
    delete(sng);
    sng = new QSound(filepath);
    delete(in);
    in = new riffwave_reader(filepath.toStdString().data());
    timer->setInterval(in->buf_size()/in->samplespersec());
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUP()));
}

void Sound::play(){
    sng->play();
    timer->start();
}

void Sound::pause(){
    sng->stop();
    timer->stop();
}

void Sound::timeUP(){
    emit secUP();
}

Sound::~Sound() {
}

