#include "chessboardview.h"
#include "chessboardscene.h"
#include "constants.h"

ChessboardView::ChessboardView(QWidget *parent) : QGraphicsView(parent) {
    chessboardScene = new ChessboardScene(800);
    setScene(chessboardScene);
    connect(this, &ChessboardView::didResize, chessboardScene, &ChessboardScene::resizeEvent);
    connect(chessboardScene, &ChessboardScene::didFinishRender, this, &ChessboardView::sceneDidFinishRender);

    setMinimumHeight(800);
    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void ChessboardView::sceneDidFinishRender() {
    setFixedWidth(scene()->itemsBoundingRect().width());
}

void ChessboardView::resizeEvent(QResizeEvent *event) {
    emit didResize(event);
}
