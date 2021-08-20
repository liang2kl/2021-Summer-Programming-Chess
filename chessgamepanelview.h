#ifndef CHESSGAMEPANELVIEW_H
#define CHESSGAMEPANELVIEW_H

#include <QWidget>
#include <QLabel>
#include "chessgamemanager.h"

class ChessGamePanelView : public QWidget {
    Q_OBJECT

private:
    ChessGameManager *manager;
    QLabel *stateLabel;

    void updateWithState(ChessGame::State state);

public:
    ChessGamePanelView(ChessGameManager *manager, QWidget *parent = nullptr);

private slots:
    void chessGameDidChangeState(ChessGame::State state);

signals:

};

#endif // CHESSGAMEPANELVIEW_H
