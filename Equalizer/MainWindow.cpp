

#include "MainWindow.h"
#include "Point.h"
#include <stdio.h>

MainWindow::MainWindow() {
    scene = new QGraphicsScene();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGraphicsView *view = new QGraphicsView(scene);
    mainLayout->addWidget(view);
    QWidget *box = new QWidget();
    box->setLayout(mainLayout);
    setCentralWidget(box);
}

void MainWindow::setModel(Sound *snd){
    this->snd = snd;
    repaint();
}

void MainWindow::repaint(){
    scene->clear();
    QList<Point>* part=snd->getPoints();
    for(int i=1;i<part->size();i++){
       scene->addLine(part->at(i-1).getX()/4,part->at(i-1).getY()/100,
                part->at(i).getX()/4,part->at(i).getY()/100);

    }
    delete part;
}


