#include "mainwindow.h"
#include <QApplication>

// main function to bind everything together
int main(int argc, char *argv[])
{   
    // instantiate application
    QApplication a(argc, argv);
    
    // instantiate mainWindow
    MainWindow mw;
    // set exit control
    mw.setAttribute(Qt::WA_QuitOnClose);
    // let it show
    mw.show();
    
    // return execution
    return a.exec();
}
