#include "chessgameconfigurationview.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *view = new ChessGameConfigurationView();

    view->show();
    return a.exec();
}
