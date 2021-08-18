#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chess.h"

#include <QPoint>
#include <QVector>
#include <QObject>

class ChessGame: QObject {
    Q_OBJECT

private:
    ChessGame();
    QVector<Chess *> _chesses = QVector<Chess *>(60);

    int indexOfPoint(const ChessPoint &point) {
        assert(point.x() < 12 && point.y() < 5);
        return point.x() * 5 + point.y();
    }

signals:
    void chessDidMove(const ChessPoint &source, const ChessPoint &dest);
    void chessDidRemoved(const ChessPoint &point);

public:
    bool canMoveChess(const ChessPoint &source, const ChessPoint &dest);
    QVector<const Chess *> chesses() const;
    void randomize();
    // Game singleton
    static const ChessGame *shared;
};



#endif // CHESSGAME_H
