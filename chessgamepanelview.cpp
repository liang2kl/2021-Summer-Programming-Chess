#include "chessgamepanelview.h"
#include "constants.h"
#include <QFont>
#include <QHBoxLayout>

ChessGamePanelView::ChessGamePanelView(ChessGameManager *manager, QWidget *parent) :
    QWidget(parent), manager(manager) {
    connect(manager->game(), &ChessGame::stateDidChange, this, &ChessGamePanelView::chessGameDidChangeState);

    auto *layout = new QHBoxLayout(this);

    stateLabel = new QLabel();
    layout->addWidget(stateLabel);
    layout->addStretch();

    auto font = QFont();
    font.setPointSize(19);
    stateLabel->setFont(font);

    updateWithState(manager->game()->state());
}

void ChessGamePanelView::chessGameDidChangeState(ChessGame::State state) {
    updateWithState(state);
}

void ChessGamePanelView::updateWithState(ChessGame::State state) {
    QString text;
    QColor color;

    switch (state) {
    case ChessGame::State::Flip:
        text = "翻棋";
        color = Qt::gray;
        break;
    case ChessGame::State::RedMove:
        text = "红方移动";
        color = Constant::red;
        break;
    case ChessGame::State::BlueMove:
        text = "蓝方移动";
        color = Constant::blue;
        break;
    case ChessGame::State::BlueWin:
        text = "蓝方胜利";
        color = Constant::blue;
        break;
    case ChessGame::State::RedWin:
        text = "红方胜利";
        color = Constant::red;
        break;
    }

    stateLabel->setText(text);
    auto palette = stateLabel->palette();
    palette.setColor(stateLabel->foregroundRole(), color);
    stateLabel->setPalette(palette);
}
