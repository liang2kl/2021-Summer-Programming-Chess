#include "chessgameview.h"
#include "constants.h"
#include "chessgameconfigurationview.h"
#include <QVBoxLayout>
#include <QMessageBox>

ChessGameView::ChessGameView(ChessGameManager *manager, QWidget *parent)
    : QWidget(parent), manager(manager) {
    auto *layout = new QVBoxLayout(this);
    chessboardView = new ChessboardView(manager);
    panelView = new ChessGamePanelView(manager);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(panelView);
    layout->addWidget(chessboardView);

    chessboardView->setStyleSheet("background-color: #1b1b1b");

    connect(manager, &ChessGameManager::didDisconnectToHost,
            [this]() {
        close();
        auto *box = new QMessageBox();
        box->setText("失去连接，游戏结束");
        box->exec();
    });
}

void ChessGameView::resizeEvent(QResizeEvent *) {
    setFixedWidth(chessboardView->width());
}

void ChessGameView::closeEvent(QCloseEvent *event) {
    auto *view = new ChessGameConfigurationView();
    view->show();

    QWidget::closeEvent(event);
}
