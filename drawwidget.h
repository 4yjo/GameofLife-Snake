#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

// needed own classes
#include "cabase.h"

// needed QtClasses
#include <QWidget>
#include <QtGui>
#include <QSize>



class DrawWidget : public QWidget, public CAbase{
    // widget class which displays and calculates the GOL
    // class inherits from multiple classes

    Q_OBJECT

public:
   // constructor must be public
   DrawWidget(int start_size, QWidget *parent=0);



    // getter for private data member size
    int getSize(){
        return size;
    };
    
    // setter for for private data member size
    void setSize(int new_size){
        new_size = size;
    };


private:
    // private data member
    int size;




protected:
    // definition of paint and mouse events
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

};

#endif // DRAWWIDGET_H
