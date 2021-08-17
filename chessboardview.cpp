#include "chessboardview.h"
#include "chessboardscene.h"
#include "constants.h"

ChessboardView::ChessboardView() {
    chessboardScene = new ChessboardScene(800);
    setScene(chessboardScene);
    connect(this, &ChessboardView::didResize, chessboardScene, &ChessboardScene::resizeEvent);
    connect(chessboardScene, &ChessboardScene::didFinishRender, this, &ChessboardView::sceneDidFinishRender);

    setMinimumHeight(800);
    setBackgroundBrush(Constant::backgroundColor);
    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
}

void ChessboardView::sceneDidFinishRender() {
//    resize(scene()->itemsBoundingRect().width(), height());
    setFixedWidth(scene()->itemsBoundingRect().width());
}

void ChessboardView::resizeEvent(QResizeEvent *event) {
    emit didResize(event);
}
