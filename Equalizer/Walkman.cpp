#include "Walkman.h"


Walkman::Walkman() {
    QWidget *box = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    draw = new MainWindow();
    
    play = new QPushButton("Play");
    play->setMaximumSize(40,20);
    pause = new QPushButton("Stop");
    pause->setMaximumSize(40,20);
    
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(1000);
     
    seekSlider = new Phonon::SeekSlider();
    seekSlider->setMediaObject(mediaObject);

    volumeSlider = new Phonon::VolumeSlider();
    volumeSlider->setAudioOutput(audioOutput);
     
    mediaObject->setCurrentSource(Phonon::MediaSource("03.wav"));
    Phonon::createPath(mediaObject, audioOutput);
    
    QPalette palette;
    palette.setBrush(QPalette::Light, Qt::blue);

    timeLcd = new QLCDNumber;
    timeLcd->setPalette(palette);
    
    fileMenu = this->menuBar()->addMenu("&File");
    openAction = new QAction("Open",this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setToolTip("Open file");
    fileMenu->addAction(openAction);
    
    layout->addWidget(draw);
    QGridLayout *layoutB = new QGridLayout();
    layoutB->addWidget(play,0,0);
    layoutB->addWidget(pause,0,1);
    layoutB->setAlignment(Qt::AlignLeft);
    
    layout->addWidget(volumeSlider);
    layout->addWidget(seekSlider);
    layout->addWidget(timeLcd);
    layout->addItem(layoutB);
    box->setLayout(layout);
    this->setCentralWidget(box); 
    
    snd = new Sound("03.wav");
    draw->setModel(snd);
    
    connect(openAction,SIGNAL(triggered()),this,SLOT(readFile()));
    connect(play,SIGNAL(clicked()),mediaObject,SLOT(play()));
    connect(play,SIGNAL(clicked()),snd,SLOT(reflash()));
    connect(pause,SIGNAL(clicked()),mediaObject,SLOT(stop()));
    connect(mediaObject,SIGNAL(tick(qint64)),this,SLOT(up(qint64)));
}

void Walkman::up(qint64 time){
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLcd->display(displayTime.toString("mm:ss"));
    snd->setStart(time/1000);
    draw->repaint();
}

void Walkman::readFile(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open file","",
            "*.wav");
    if(fileName.isEmpty())
        return;
    mediaObject->setCurrentSource(Phonon::MediaSource(fileName));
    snd->setFile(fileName);
    draw->setModel(snd);
}

Walkman::Walkman(const Walkman& orig) {
}

Walkman::~Walkman() {
}

