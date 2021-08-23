#ifndef CHESSGAMEPANELVIEW_H
#define CHESSGAMEPANELVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>

#include "chessgamemanager.h"

class ChessGamePanelView : public QWidget {
    Q_OBJECT

private:
    ChessGameManager *manager;
    QHBoxLayout *hLayout;
    QVBoxLayout *timeoutVLayout;
    QLabel *stateLabel;
    QLabel *sideLabel;
    QLabel *stepsLabel;
    QPushButton *surrenderButton = nullptr;
    QPushButton *startButton = nullptr;
    QLCDNumber *lcdNumber;
    QLabel *thisTimeoutLabel;
    QLabel *thatTimeoutLabel;

    bool didSetSide = false;

    void updateWithState(ChessGame::State state);
private slots:
    void updateUnsidedLabel();

public:
    ChessGamePanelView(ChessGameManager *manager, QWidget *parent = nullptr);

private slots:
    void chessGameDidChangeState(ChessGame::State state);
    void chessGameDidChangeIndex();
    void chessGameDidChangeThisPlayerTimeoutCount(int count);
    void chessGameDidChangeAnotherPlayerTimeoutCount(int count);
    void chessGameDidUpdateRemainingSeconds(int seconds);
    void chessGameDidStarted();
signals:

};

#endif // CHESSGAMEPANELVIEW_H
