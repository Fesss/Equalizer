
#include <QtGui/QApplication>

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
    // create and show your widgets here
    Walkman *wm = new Walkman();
    wm->show();
    app.exec();
    return 1;
}

