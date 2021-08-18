#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chess.h"

#include <QPoint>
#include <QVector>

class ChessGame {
private:
    struct ChessInfo {
        enum Side { Blue, Red };

        Chess chess;
        Side side;
        QPoint position;

        ChessInfo(Chess chess, Side side, QPoint position = QPoint()):
            chess(chess), side(side), position(position) {}
    };

    QVector<ChessInfo> blues;
    QVector<ChessInfo> reds;

    void randomize();

public:
    ChessGame();
};



#endif // CHESSGAME_H
