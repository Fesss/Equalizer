

#include "MainWindow.h"
#include "Point.h"
#include <stdio.h>
#include <QDesktopWidget>
#include <QApplication>

MainWindow::MainWindow() {
    scene = new QGraphicsScene();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGraphicsView *view = new QGraphicsView(scene);
    view->rotate(180);
    mainLayout->addWidget(view);
    QWidget *box = new QWidget();
    box->setLayout(mainLayout);
    setCentralWidget(box);
    pen = QPen(QBrush(Qt::black), 1);
    QRect rect = QApplication::desktop()->screenGeometry();
    scene->setSceneRect(rect);
    resolution=QPoint(rect.bottomRight());
}

void MainWindow::setModel(Sound *snd){
    this->snd = snd;
    repaint();
}

void MainWindow::repaint(){
    int j=0;
    scene->clear();
    QList<Point>* part=snd->getPoints();
    for(int i=0;i<part->size();i++){
        j+=part->at(i).getY()/10;   
        scene->addRect(i*resolution.x()/128, j, resolution.x()/128, 1000,  pen, QBrush(QColor(130-i,i*2/3,50)));
        j=0;
    }
}



