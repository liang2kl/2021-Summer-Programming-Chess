#include "chessgameview.h"
#include "constants.h"
#include <QVBoxLayout>

ChessGameView::ChessGameView(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    chessboardView = new ChessboardView();
    panelView = new ChessGamePanelView();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(panelView);
    layout->addWidget(chessboardView);

    setStyleSheet("background-color: #1b1b1b");
}

void ChessGameView::resizeEvent(QResizeEvent *) {
    setFixedWidth(chessboardView->width());
}
