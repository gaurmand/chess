#include "chesswindow.h"
#include "tests/test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if (RUN_TESTS)
    {
        Chess::Tester::run();
        return 1;
    }

    QApplication a(argc, argv);
    ChessWindow w;
    w.show();
    return a.exec();
}
