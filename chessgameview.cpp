#include "chessgameview.h"
#include "constants.h"
#include <QVBoxLayout>

ChessGameView::ChessGameView(ChessGameManager *manager, QWidget *parent)
    : QWidget(parent), manager(manager) {
    auto *layout = new QVBoxLayout(this);
    chessboardView = new ChessboardView(manager);
    panelView = new ChessGamePanelView(manager);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(panelView);
    layout->addWidget(chessboardView);

    setStyleSheet("background-color: #1b1b1b");
}

void ChessGameView::resizeEvent(QResizeEvent *) {
    setFixedWidth(chessboardView->width());
}
