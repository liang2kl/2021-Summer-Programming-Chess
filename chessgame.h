#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chess.h"
#include "graph.h"

#include <QPoint>
#include <QVector>
#include <QObject>
#include <QMap>
#include <QQueue>

class ChessGame: public QObject {
    Q_OBJECT

private:
    ChessGame();
    QVector<Chess *> _chesses = QVector<Chess *>(60);
    QVector<int> railwayIndices;
    QMap<int, int> railwayIndexMap;

    int indexOfPoint(const ChessPoint &point) const {
        assert(point.x() < 12 && point.y() < 5);
        return point.x() * 5 + point.y();
    }

    bool allowingPointOnRailwayMoveTo(const ChessPoint &source, const ChessPoint &dest, bool isEngineer, const Graph &railwayGraph) const;
    bool allowingVerticallyMoveTo(const ChessPoint &source, const ChessPoint &dest) const;
    bool allowingHorizontallyMoveTo(const ChessPoint &source, const ChessPoint &dest) const;
    bool isLandmineCleared(Chess::Side side) const;

    // Whether a chess has somewhere to go.
    bool chessCanChangePosition(const Chess *chess);

    Graph railwayGraph(int startIndex, int endIndex) const;

public:
    // TODO: Cache
    QVector<int> availablePointsFor(const Chess *) const;
    bool canMoveChess(const ChessPoint &source, const ChessPoint &dest) const;
    QVector<const Chess *> chesses() const;
    void randomize();
    // Game singleton
    static ChessGame * const shared;

// Playing
public:
    void flipChess(const ChessPoint &pos);
    void moveChess(const ChessPoint &source, const ChessPoint &dest);

// Game state
public:
    enum State { Flip, BlueMove, RedMove, BlueWin, RedWin };

    State state() { return _state; }

private:
    State _state = Flip;
    int lastFlippedSide = -1;
    void updateFlipState(Chess::Side side);
    void updateResultState();
    void setState(State state);

signals:
    void chessDidFlip(const ChessPoint &pos);
    void chessDidMove(const ChessPoint &source, const ChessPoint &dest);
    void chessDidRemoved(const ChessPoint &point);
    void stateDidChange(State state, State oldState);
};


#endif // CHESSGAME_H
