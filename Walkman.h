
#ifndef WALKMAN_H
#define	WALKMAN_H
#include "QObject"
#include "QMainWindow"
#include "QLayout"
#include "QPushButton"
#include "QSlider"
#include "QTimer"
#include "Sound.h"

class Walkman : public QMainWindow {
    Q_OBJECT;
    
public:
    Walkman();
  //  Walkman(const Walkman& orig);
    virtual ~Walkman();

public slots:
    void check();
    void reflash();

private:
    Sound *snd;
    QPushButton *play;
    QPushButton *pause;
    QSlider *slider;
};

#endif	/* WALKMAN_H */

