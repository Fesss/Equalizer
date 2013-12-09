
#include <iostream>

#include "Walkman.h"

Walkman::Walkman() {
    QWidget *box = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    
    slider = new QSlider(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setSingleStep(1);
    slider->setValue(0);
    
    play = new QPushButton("Play");
    play->setMaximumSize(40,20);
    pause = new QPushButton("Pause");
    pause->setMaximumSize(40,20);
       
   // connect(pause,SIGNAL(clicked()),this,SLOT(check()));    connect(tm,SIGNAL(timeout()),wv,SLOT(check()));
    
    layout->addWidget(slider);
    
    QGridLayout *layoutB = new QGridLayout();
    layoutB->addWidget(play,0,0);
    layoutB->addWidget(pause,0,1);
    layoutB->setAlignment(Qt::AlignLeft);
    
    layout->addItem(layoutB);
    box->setLayout(layout);
    this->setCentralWidget(box); 
    
    snd = new Sound("03.wav");
    slider->setRange(0,snd->getLength());
    std::cout<<snd->getLength();
    
    connect(play,SIGNAL(clicked()),snd,SLOT(play()));
    connect(play,SIGNAL(clicked()),this,SLOT(reflash()));
    connect(pause,SIGNAL(clicked()),snd,SLOT(pause()));
    connect(snd,SIGNAL(secUP()),this,SLOT(check()));
}

void Walkman::check(){
    slider->setValue(slider->value()+1);
}

void Walkman::reflash(){
    slider->setValue(0);
}

//Walkman::Walkman(const Walkman& orig) {
//}

Walkman::~Walkman() {
}

