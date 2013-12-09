
#include <QtGui/QApplication>
#include <winsock2.h>
#include <iostream>
#include "QWidget"
#include "QVBoxLayout"
#include "QSlider"
#include "QTimer"
#include "QSound"
#include "Sound.h"
#include "Walkman.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);
   
    QApplication app(argc, argv);
    /*
    MainWindow *mw = new MainWindow();
    mw->show();
    mw->timerStart();
*/
    //QSound *snd = new QSound("03.wav");
   // Sound *snd = new Sound("03.wav");
   // std::cout<<snd->getLength();
    // snd->play("03.wav");
    /*    QWidget *box = new QWidget();
    WalkmanView *wv = new WalkmanView();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(wv,100);
    box->setLayout(layout);
    box->show(); 

    QTimer *tm = new QTimer();
    QObject::connect(tm,SIGNAL(timeout()),wv,SLOT(sheck()));
    tm->start(100);
    */
    
    // create and show your widgets here
    Walkman *wm = new Walkman();
    wm->show();
    app.exec();
   return 1;
}

