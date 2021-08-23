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
    connect(manager->game(), &ChessGame::didStarted,
            this, &ChessGamePanelView::chessGameDidStarted);

    // Hierarchy
    hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(10);

    sideLabel = new QLabel();
    stateLabel = new QLabel();
    stepsLabel = new QLabel();
    timeoutVLayout = new QVBoxLayout();
    lcdNumber = new QLCDNumber();
    startButton = new QPushButton();

    hLayout->addWidget(stateLabel);
    hLayout->addWidget(sideLabel);
    hLayout->addWidget(lcdNumber);
    hLayout->addLayout(timeoutVLayout);
    hLayout->addStretch();
    hLayout->addWidget(stepsLabel);
    hLayout->addWidget(startButton);

    // Configuration
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
    sideLabel->setFont(font);

    lcdNumber->setVisible(false);
    lcdNumber->setDigitCount(2);

    startButton->setText("开始");
    connect(startButton, &QPushButton::clicked,
            [this]() {
        this->manager->startGame();
        this->startButton->setText("等待中");
        this->startButton->setEnabled(false);
    });

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

    if (manager->game()->isStarted()) {
        updateUnsidedLabel();
    }
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

void ChessGamePanelView::chessGameDidStarted() {
    this->startButton->setVisible(false);
    this->hLayout->removeWidget(this->startButton);
    updateUnsidedLabel();
}

void ChessGamePanelView::updateUnsidedLabel() {
    bool isCurrentSide = manager->game()->canAct();
    sideLabel->setText(isCurrentSide ? "轮到你" : "轮到对手");
}
