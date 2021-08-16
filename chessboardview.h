#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "chessboardscene.h"

class ChessboardView : public QGraphicsView
{
private:
    void resizeEvent(QResizeEvent*);
    QGraphicsScene * scene;
public:
    ChessboardView(ChessboardScene * scene);
};

#endif // CHESSBOARDVIEW_H
