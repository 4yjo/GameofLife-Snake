
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QTimer>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ios>

#include "cabase.h"


class DrawWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    QTimer *timer = new QTimer;


private slots:
    //button slots
    void startGame();
    void stopGame();
    void clearGame();
    //box slots
    void sizeChanged(int new_size);
    void modeChanged(int index);
    void intervallChanged(int new_intervall);
    void gameChanged(int index);

    void PrepareFieldSnake();
    void food();

    // plain CAbase manipulation and repaint()
    void setRandom();
    void evolve();
    void load();
    void save();

private:
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *clearButton;
    QPushButton *randButton;
    QPushButton *evolveButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    DrawWidget *drawArea;
    QLabel *sizeLabel;
    QLabel *modeLabel;
    QLabel *intervallLabel;
    QComboBox *modeBox;
    QComboBox *chooseGameBox;
    QSpinBox *sizeBox;
    QSpinBox *intervallBox;
};

#endif // MAINWINDOW_H
