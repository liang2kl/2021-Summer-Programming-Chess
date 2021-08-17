#include "chessboardview.h"
#include "chessboardscene.h"

ChessboardView::ChessboardView(ChessboardScene *scene)
    : QGraphicsView(scene)
{
    this->scene = scene;
    connect(this, &ChessboardView::resizeEvent, scene, &ChessboardScene::resizeEvent);

    this->setMinimumHeight(600);
    this->setMinimumWidth(400);
}
