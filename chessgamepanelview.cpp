#include "chessgamepanelview.h"
#include "constants.h"
#include <QFont>
#include <QMessageBox>

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
    connect(manager->game(), &ChessGame::remainingTimeDidChange,
            this, &ChessGamePanelView::chessGameDidUpdateRemainingSeconds);
    connect(manager->game(), &ChessGame::anotherPlayerDidTimeout,
            this, &ChessGamePanelView::chessGameDidChangeAnotherPlayerTimeoutCount);
    connect(manager->game(), &ChessGame::thisPlayerDidTimeout,
            this, &ChessGamePanelView::chessGameDidChangeThisPlayerTimeoutCount);

    hLayout = new QHBoxLayout(this);

    stateLabel = new QLabel();
    stepsLabel = new QLabel();
    timeoutVLayout = new QVBoxLayout();
    lcdNumber = new QLCDNumber();

    hLayout->addWidget(stateLabel);
    hLayout->addWidget(lcdNumber);
    hLayout->addLayout(timeoutVLayout);
    hLayout->addStretch();
    hLayout->addWidget(stepsLabel);

    timeoutVLayout->setSpacing(0);

    thatTimeoutLabel = new QLabel();
    auto timeoutFont = QFont();
    timeoutFont.setPointSize(11);
    thatTimeoutLabel->setFont(timeoutFont);

    thisTimeoutLabel = new QLabel();
    thisTimeoutLabel->setFont(timeoutFont);

    auto font = QFont();
    font.setPointSize(19);
    stateLabel->setFont(font);
    stepsLabel->setFont(font);

    lcdNumber->setVisible(false);
    lcdNumber->setDigitCount(2);

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
    case ChessGame::State::ThisWin:
        text = "你胜利";
        color = Qt::black;
        break;
    case ChessGame::State::ThatWin:
        text = "对手胜利";
        color = Qt::black;
        break;
    }

    stateLabel->setText(text);
    setColorFor(stateLabel, color);

    if (state == ChessGame::State::RedWin || state == ChessGame::State::BlueWin ||
            state == ChessGame::State::ThisWin || state == ChessGame::State::ThatWin) {
        if (surrenderButton) {
            surrenderButton->setEnabled(false);
        }

        auto box = QMessageBox();
        box.setText(text);
        box.setWindowModality(Qt::ApplicationModal);
        box.exec();
    }

}

void ChessGamePanelView::chessGameDidChangeIndex() {
    if (manager->game()->canSurrender() && !surrenderButton) {
        surrenderButton = new QPushButton();
        surrenderButton->setText("投降");
        hLayout->addWidget(surrenderButton);
        // TODO: Surrender Action
        connect(surrenderButton, &QPushButton::clicked,
                [this]() { this->manager->surrender(); });
    }

    stepsLabel->setText(QString::number(manager->game()->steps()));
}

void ChessGamePanelView::chessGameDidChangeThisPlayerTimeoutCount(int count) {
    qDebug() << "This player, timeout" << count;
    if (count == 1) {
        timeoutVLayout->addWidget(thisTimeoutLabel);
    }

    thisTimeoutLabel->setText("你已超时 " + QString::number(count) + " 次");
}

void ChessGamePanelView::chessGameDidChangeAnotherPlayerTimeoutCount(int count) {
    qDebug() << "Another player, timeout" << count;
    if (count == 1) {
        timeoutVLayout->addWidget(thatTimeoutLabel);
    }
    thatTimeoutLabel->setText("对手超时 " + QString::number(count) + " 次");
}

void ChessGamePanelView::chessGameDidUpdateRemainingSeconds(int seconds) {
    if (!lcdNumber->isVisible()) {
        lcdNumber->setVisible(true);
    }
    lcdNumber->display(seconds);
    qDebug() << seconds;
}
