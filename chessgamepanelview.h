#ifndef CHESSGAMEPANELVIEW_H
#define CHESSGAMEPANELVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "chessgamemanager.h"

class ChessGamePanelView : public QWidget {
    Q_OBJECT

private:
    ChessGameManager *manager;
    QHBoxLayout *hLayout;
    QLabel *stateLabel;
    QLabel *stepsLabel;
    QPushButton *surrenderButton;

    void updateWithState(ChessGame::State state);

public:
    ChessGamePanelView(ChessGameManager *manager, QWidget *parent = nullptr);

private slots:
    void chessGameDidChangeState(ChessGame::State state);
    void chessGameDidChangeIndex();
signals:

};

#endif // CHESSGAMEPANELVIEW_H
