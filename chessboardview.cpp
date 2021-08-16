#include "chessboardview.h"
#include "chessboardscene.h"

ChessboardView::ChessboardView(ChessboardScene *scene)
    : QGraphicsView(scene)
{
    this->scene = scene;
}

void ChessboardView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);


}
