
#include "cabase.h"
#include "drawwidget.h"
// needed QtClasses
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

// constructor
// custom widget class to display the cells
// inherits from CAbase
DrawWidget::DrawWidget(int start_size, QWidget *parent)
    : QWidget(parent), CAbase(start_size, start_size)
    {
    // define the size of the widget
    DrawWidget::size = start_size;
    // set background color
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    // set minimum size so layout in parent widget fits
    setMinimumSize(start_size*13, start_size*13);
    // define alignment through geometry
    DrawWidget::setGeometry(0, 0, start_size, start_size);
}

// implementation of the paintEvent to draw the cells
void DrawWidget::paintEvent(QPaintEvent *e){
    // get the sizes
    int rows = (DrawWidget::getRows());
    int columns = (DrawWidget::getColumns());
    // set size of painted shape
    int rectSize = 13;
    // construct a painter
    QPainter painter(this);
    // paint
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            if ((DrawWidget::getNow()[(( i*columns)+j)]) > 0){
                // if cell alive, fill cell
                painter.setBrush(Qt::SolidPattern);
            }
            else if((DrawWidget::getNow()[(( i*columns)+j)])< 0){
                 painter.setBrush(Qt::Dense4Pattern);
            }
            else{
                // if cell dead, leave empty
                painter.setBrush(Qt::NoBrush);
            }
            // paint the cell
            painter.drawEllipse((i*rectSize), (j*rectSize), rectSize, rectSize);
        }
    }

}

// implementation of mouseEvent
void DrawWidget::mousePressEvent(QMouseEvent *e){
    if (DrawWidget::getGolMode()){
        // get sizes
        int column = (e->y()) / 13;
        int row = (e->x()) / 13;
        // check if cell is alive or dead
        int current_value = DrawWidget::getCell(row+1, column+1);
        // set new value to be painted
        int new_value;
        if (current_value){
            // if it was alive set a dead cell
            new_value = 0;
        }
        else {
            // if it was dead set a alive cell
            new_value = 1;
        }
        // reflect the change in the cell array
        DrawWidget::setCell( row+1, column+1, new_value, DrawWidget::getNow());
        // paint anew to show changes
        DrawWidget::repaint();
    }
}
// SNAKE-NAVIGATION:
void DrawWidget::keyPressEvent(QKeyEvent *e){
    if (DrawWidget::getSnakeMode()){
            DrawWidget::grabKeyboard();
            switch(e->key()){
            case Qt::Key_Left:
                if (DrawWidget::getLeftallowed())
                {DrawWidget::setDirection(4);}
                break;
            case Qt::Key_Right:
                if (DrawWidget::getRightallowed())
                {DrawWidget::setDirection(6);}
                break;
            case Qt::Key_Up:
                if (DrawWidget::getUpallowed())
                {DrawWidget::setDirection(8);}
                break;
            case Qt::Key_Down:
                if (DrawWidget::getDownallowed())
                {DrawWidget::setDirection(2);}
                break;

    }
}else{
       DrawWidget::releaseKeyboard();
}
}
