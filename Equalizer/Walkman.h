
#ifndef WALKMAN_H
#define	WALKMAN_H
#include "QObject"
#include "QMainWindow"
#include "QLayout"
#include "QPushButton"
#include "QSlider"
#include "QTimer"
#include "Sound.h"
#include "MainWindow.h"
#include <iostream>
#include <Phonon>
#include <iostream>
#include <QLCDNumber>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

class Walkman : public QMainWindow {
    Q_OBJECT;
    
public:
    Walkman();
    Walkman(const Walkman& orig);
    virtual ~Walkman();
    
public slots:
    void up(qint64);
    void readFile();

private:
    Sound *snd;
    QPushButton *play;
    QPushButton *pause;
    MainWindow *draw;
    
    Phonon::SeekSlider *seekSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
    Phonon::VolumeSlider *volumeSlider;
    QLCDNumber  *timeLcd;
    QMenu *fileMenu;
    QAction *openAction;
};

#endif	/* WALKMAN_H */

