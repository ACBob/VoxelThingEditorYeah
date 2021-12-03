#include <QApplication>

#include "widgets/mainwindow.hpp"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    auto pEditor = new MainWindow( nullptr );
    pEditor->show();

    return app.exec();
}