#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "chessboardscene.h"

class ChessboardView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene * scene;

signals:
    void resizeEvent(QResizeEvent*);
public:
    ChessboardView(ChessboardScene * scene);
};

#endif // CHESSBOARDVIEW_H
