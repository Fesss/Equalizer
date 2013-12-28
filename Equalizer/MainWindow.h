
#ifndef MAINWINDOW_H
#define	MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <Point.h>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include "Sound.h"


class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    void setModel(Sound *snd);
private:
    QGraphicsScene *scene;
    Sound *snd;

public slots:
    void repaint();


};

#endif	/* MAINWINDOW_H */
