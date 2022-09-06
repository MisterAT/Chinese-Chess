#include <QApplication>
#include <QTime>
#include "Board.h"
#include "SingleGame.h"
#include "start_interface.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    start_interface start;
    start. show();
//    SingleGame board;
//    board.show();
    return app.exec();
}
