#include "chessgamepanelview.h"
#include "constants.h"
#include <QFont>

static void setColorFor(QLabel *label, QColor color) {
    auto palette = label->palette();
    palette.setColor(label->foregroundRole(), color);
    label->setPalette(palette);
}

ChessGamePanelView::ChessGamePanelView(ChessGameManager *manager, QWidget *parent) :
    QWidget(parent), manager(manager) {
    connect(manager->game(), &ChessGame::stateDidChange,
            this, &ChessGamePanelView::chessGameDidChangeState);
    connect(manager->game(), &ChessGame::indexDidChange,
            this, &ChessGamePanelView::chessGameDidChangeIndex);

    hLayout = new QHBoxLayout(this);

    stateLabel = new QLabel();
    stepsLabel = new QLabel();
    hLayout->addWidget(stateLabel);
    hLayout->addStretch();
    hLayout->addWidget(stepsLabel);

    auto font = QFont();
    font.setPointSize(19);
    stateLabel->setFont(font);
    stepsLabel->setFont(font);

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
    setColorFor(stateLabel, color);

}

void ChessGamePanelView::chessGameDidChangeIndex() {
    if (manager->game()->canSurrender() && !surrenderButton) {
        surrenderButton = new QPushButton();
        surrenderButton->setText("投降");
        hLayout->addWidget(surrenderButton);
        // TODO: Surrender Action
    }

    stepsLabel->setText(QString::number(manager->game()->steps()));
}
