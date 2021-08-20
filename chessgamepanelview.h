#ifndef CHESSGAMEPANELVIEW_H
#define CHESSGAMEPANELVIEW_H

#include <QWidget>
#include <QLabel>
#include "chessgame.h"

class ChessGamePanelView : public QWidget {
    Q_OBJECT

private:
    QLabel *stateLabel;

    void updateWithState(ChessGame::State state);

public:
    explicit ChessGamePanelView(QWidget *parent = nullptr);

private slots:
    void chessGameDidChangeState(ChessGame::State state);

signals:

};

#endif // CHESSGAMEPANELVIEW_H
