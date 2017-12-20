

// needed libs for save & load

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ios>
//#include <experimental/random>

// needed custom class headers

#include "mainwindow.h"
#include "drawwidget.h"
#include "cabase.h"

// needed Qt-Classes

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QSpinBox>
#include <QComboBox>
#include <QBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QString>

MainWindow::MainWindow()
    {
    // layout generation
    QHBoxLayout *drawLayout = new QHBoxLayout;
    QGridLayout *buttonLayout = new QGridLayout;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // buttons
    QPushButton* startButton = new QPushButton(tr("Start"));
    QPushButton* stopButton = new QPushButton(tr("Stop"));
    QPushButton* clearButton = new QPushButton(tr("Clear"));
    QPushButton* randButton = new QPushButton(tr("Set Random Cell Values"));
    QPushButton* evolveButton = new QPushButton(tr("Evolve One Step"));
    QPushButton* loadButton = new QPushButton(tr("Load"));
    QPushButton* saveButton = new QPushButton(tr("Save"));

    // labels
    QLabel* sizeLabel = new QLabel(tr("Universe Size"));
    QLabel* modeLabel = new QLabel(tr("Color Palette"));
    QLabel* gameChangeLabel = new QLabel(tr("Select Game"));
    QLabel* intervallLabel = new QLabel(tr("Generation Intervall in msec"));

    // spinBoxes
    QSpinBox* sizeBox = new QSpinBox;
    sizeBox->setMinimum(1);
    sizeBox->setMaximum(255);
    sizeBox->setValue(30);

    QSpinBox* intervallBox = new QSpinBox;
    intervallBox->setMinimum(20);
    intervallBox->setMaximum(5000);
    intervallBox->setValue(300); //sets default value for spinbox
    intervallBox->setSingleStep(10);

    // comboBox
    QComboBox* modeBox = new QComboBox;
    modeBox->addItem(tr("base"));
    modeBox->addItem(tr("dark"));

    QComboBox* chooseGameBox = new QComboBox;
    chooseGameBox->addItem(tr("Game of Life"));
    chooseGameBox->addItem(tr("Snake"));

    int size = sizeBox->value();

    // adding widgets to layout
    buttonLayout->addWidget(startButton, 1, size+1, 1, 1);
    buttonLayout->addWidget(stopButton, 1, size+2, 1, 1);
    buttonLayout->addWidget(clearButton, 1, size+3, 1, 1);
    buttonLayout->addWidget(sizeLabel, 2, size+1, 1, 3);
    buttonLayout->addWidget(sizeBox, 3, size+1, 1, 3);
    buttonLayout->addWidget(modeLabel, 5, size+1, 1, 3);
    buttonLayout->addWidget(modeBox, 6, size+1, 1, 3);
    buttonLayout->addWidget(gameChangeLabel,7,size+1,1,3);
    buttonLayout->addWidget(chooseGameBox, 8, size+1,1,3);
    buttonLayout->addWidget(intervallLabel, 9, size+1, 1, 3);
    buttonLayout->addWidget(intervallBox, 10, size+1, 1, 3);
    buttonLayout->addWidget(randButton, 11, size+1, 1, 3);
    buttonLayout->addWidget(evolveButton, 12, size+1, 1, 3);
    buttonLayout->addWidget(loadButton, 13, size+1, 1, 1);
    buttonLayout->addWidget(saveButton, 13, size+3, 1, 1);
    
    // start buttton is default button
    startButton->setDefault(true);

    // get a DrawWidget
    drawArea = new DrawWidget(size, this);
    
    // add layours
    drawLayout->addWidget(drawArea);
    mainLayout->addLayout(drawLayout);
    mainLayout->addLayout(buttonLayout);
    
    // set layout
    setLayout(mainLayout);
    
    // set window title
    setWindowTitle(tr("Conway's Game of Life and Snake"));

    //create Timer, set Timer Intervall and connect to evolution function from cabase
    timer->setInterval(intervallBox->value());

    // connecting buttons
    connect(startButton, SIGNAL(clicked()),
            this, SLOT(startGame()));
    connect(stopButton, SIGNAL(clicked()),
            this, SLOT(stopGame()));
    connect(clearButton, SIGNAL(clicked()),
            this, SLOT(clearGame()));
    connect(randButton, SIGNAL(clicked()),
            this, SLOT(setRandom()));
    connect(evolveButton, SIGNAL(clicked()),
            this, SLOT(evolve()));
    connect(loadButton, SIGNAL(clicked()),
            this, SLOT(load()));
    connect(saveButton, SIGNAL(clicked()),
            this, SLOT(save()));

    // connecting boxes
    connect(sizeBox, SIGNAL(valueChanged(int)),
            this, SLOT(sizeChanged(int)));
    connect(intervallBox, SIGNAL(valueChanged(int)),
            this, SLOT(intervallChanged(int)));
    connect(modeBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(modeChanged(int)));
    connect(chooseGameBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(gameChanged(int)));

    // connecting timer to evolve function
    connect(timer,SIGNAL(timeout()),
            this, SLOT(evolve()));

    setRandom();
}

void MainWindow::startGame(){
    timer->start();
    if (drawArea->getSnakeMode()){
        drawArea->setFocus();
    }
   // startet timer, bei timer->timeout()wird evolve aufgerufen (s.u.)
}

void MainWindow::stopGame(){
    timer->stop();
}

void MainWindow::clearGame(){
    MainWindow::stopGame();
    int rows = drawArea->getRows();
    int columns = drawArea->getColumns();
    drawArea->resetSize(rows, columns);
    if (drawArea->getSnakeMode()){
        MainWindow::PrepareFieldSnake();
    }
    drawArea->setAlive();
    drawArea->update();
}

void MainWindow::sizeChanged(int new_size){
    if (drawArea->getGolMode()){
       drawArea->resetSize(new_size, new_size);
       drawArea->setMinimumSize(new_size*13, new_size*13);
       drawArea->update();}
    else{
        MainWindow::stopGame();
        QMessageBox noResizeNachricht;
        noResizeNachricht.setWindowTitle("Achtung!");
        noResizeNachricht.setText("Im Snake-Modus steht diese Funktion nicht zur Verfügung!");
        noResizeNachricht.exec();
    }
}

void MainWindow::modeChanged(int index){
    switch (index) {
    case 1:
        drawArea->setBackgroundRole(QPalette::Dark);
        break;
    default:
        drawArea->setBackgroundRole(QPalette::Base);
        break;
    }
}

void MainWindow::intervallChanged(int new_intervall){
    timer->setInterval(new_intervall);
}

void MainWindow::setRandom(){
    if (drawArea->getGolMode()){
        // seed rand()
        srand(time(0));
        int rows = drawArea->getRows();
        int columns = drawArea->getColumns();
        int* now = drawArea->getNow();
        for ( int i = 0; i < rows; i++ ) {
              for ( int j = 0; j < columns; j++ ) {
                // fill with pseudo-random number [0,1]
                int numberfill = rand() % 2;
                now[((i*rows)+j)] = numberfill;
                }
            }
        drawArea->repaint();
    }else{
        MainWindow::clearGame();
        drawArea->repaint();
    }
}

void MainWindow::save(){
    MainWindow::stopGame();
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Cell Pattern"), "",
        tr("Cell Patterns (*.txt);;All Files (*)"));
    // if user has not chosen file
    if (fileName.isEmpty())
        // abort
        return;
    else {
        // check if file is writable
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        
        // old function from console.cpp
        std::ofstream outFile;
        // in call, convert QString to StdString
        outFile.open(fileName.toStdString(), std::ios::trunc);
        if (outFile.is_open()){
            outFile << drawArea->getRows();
            outFile << std::endl;
            outFile << drawArea->getColumns();
            outFile << std::endl;
            for( int i = 0; i < drawArea->getRows(); i++){
                for( int j = 0; j < drawArea->getColumns(); j++){
                    outFile << drawArea->getNow()[((i*drawArea->getRows())+j)];
                   }
            }
        }
        outFile.close();
    }
}

void MainWindow:: load(){
    MainWindow::stopGame();
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Saved Cell Pattern"), "",
        tr("Cell Patterns (*.txt);;All Files (*)"));

    // if user selects none: abort
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        // check if file is readable
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        std::ifstream inFile;
        // in call, convert QString to StdString
        inFile.open(fileName.toStdString(), std::ios::trunc);
        if (inFile.is_open()){
            std::string line;
            delete [] drawArea->getNow();
            std::string new_rows;
            std::string new_columns;
            std::string new_now;
            while (inFile.good()){
                std::getline(inFile, new_rows);
                std::getline(inFile, new_columns);
                std::getline(inFile, new_now);
            }
            inFile.close();
            //convert loaded rows from string to int
            int rows_int = 0;
            std::stringstream tostringrow(new_rows);
            tostringrow >> rows_int;
            // convert loaded columns from string to int
            int columns_int = 0;
            std::stringstream tostringcolumns(new_columns);
            tostringcolumns >> columns_int;
            drawArea->resetSize(rows_int, columns_int);
            // convert loaded array from string to array
            //drawArea->getNow() = new int[new_now.length()];
            int new_value = 0;
            std::string new_value_str;
            for( int i = 0; i < new_now.length(); i++){
                // enables transformation from string to int by ?representation?
                new_value_str = new_now[i];
                new_value = std::stoi(new_value_str);
                drawArea->setLazyNow(i, new_value);
            }

        }
    }
    drawArea->repaint();
}

void MainWindow::evolve(){
    // do everything to decide and display next generation
    if (drawArea->getIsAlive()){
        drawArea->decideFuture();
        drawArea->timeTravel();
        drawArea->repaint();
        if (drawArea->getGrowth()){
            food();
            drawArea->setGrowth(false);
        }
    }
    // IF SNAKE DIES:
    else{
        MainWindow::stopGame();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Tja..");
        msgBox.setText("GAME OVER!");
        msgBox.exec();
    }

}

//SNAKE

void MainWindow::gameChanged(int index){
    switch(index) {
    case 1: //Snake
        drawArea->setSnake();
        MainWindow::clearGame();
        break;
    default: //Game of Life
        drawArea->setGol();
        MainWindow::clearGame();
        break;
    }
}

//FOOD
void MainWindow::food(){
    bool fed = false;
    while (fed == false){
        srand(time(0));
        int y = (rand() % drawArea->getRows())+1;
        int z = (rand() % drawArea->getColumns())+1;
        if (drawArea->getCell(y,z)==0){
            drawArea->setCell(y,z,-1,drawArea->getNow());
            fed = true;
        }
    }
}


void MainWindow::PrepareFieldSnake(){
    MainWindow::stopGame();
    //initialize Head to start snake
    //int new_direction = ((std::experimental::randint(1,4))*2);
    int new_direction = 8;
    int new_length = 3;
    int new_xpos = 10;
    int new_ypos = 20;
    //int new_xpos = ((std::experimental::randint(0,(drawArea->getColumns()))));
    //int new_ypos = ((std::experimental::randint(0,(drawArea->getRows()))));
    drawArea->setDirection(new_direction);
    drawArea->setLength(new_length);
    drawArea->setXpos(new_xpos);
    drawArea->setYpos(new_ypos);
    drawArea->setCell(new_xpos, new_ypos, new_length, drawArea->getNow());
    drawArea->setCell(new_xpos-1, new_ypos, new_length-1, drawArea->getNow());
    drawArea->setCell(new_xpos-2, new_ypos, new_length-2, drawArea->getNow());
    food();
}
