#include "chessgameview.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *view = new ChessGameView();

    view->show();
    return a.exec();
}
