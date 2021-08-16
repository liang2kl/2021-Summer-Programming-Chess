#include "mainwindow.h"
#include "chessboardscene.h"
#include "chessboardview.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChessboardView *view = new ChessboardView(new ChessboardScene());

    view->show();

    return a.exec();
}
