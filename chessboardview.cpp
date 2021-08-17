#include "chessboardview.h"
#include "chessboardscene.h"

ChessboardView::ChessboardView() {
    chessboardScene = new ChessboardScene();
    setScene(chessboardScene);
    connect(this, &ChessboardView::didResize, chessboardScene, &ChessboardScene::resizeEvent);
    connect(chessboardScene, &ChessboardScene::didFinishRender, this, &ChessboardView::sceneDidFinishRender);

    setMinimumHeight(600);
    setMinimumWidth(400);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
}

void ChessboardView::sceneDidFinishRender() {
//    resize(scene()->itemsBoundingRect().width(), height());
    setFixedWidth(scene()->itemsBoundingRect().width());
}

void ChessboardView::resizeEvent(QResizeEvent *event) {
    emit didResize(event);
}
