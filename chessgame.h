#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chess.h"
#include "graph.h"

#include <QPoint>
#include <QVector>
#include <QObject>
#include <QMap>
#include <QQueue>
#include <QTimer>

class ChessGame: public QObject {
    Q_OBJECT

private:
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
    ChessGame();
    QVector<int> availablePointsFor(const Chess *) const;
    bool canMoveChess(const ChessPoint &source, const ChessPoint &dest) const;
    QVector<const Chess *> chesses() const;
    void randomize();
    void setChesses(QVector<Chess> chesses);

// Indexing and timer
private:
    bool isServer;
    qint32 startIndex = -1;
    qint32 __index = -1;

    // Manage timeout state locally
    QTimer *updateTimer;
    int currentSecond;

    int thisTimeoutCount = 0;
    int anotherTimeoutCount = 0;

    void increaseIndex();
    void startTimer();
    void stopTimer();
    void updateTimeout();

public:
    void setIsServer(bool isServer) { this->isServer = isServer; }
    void setStartIndex(qint32 index);
    qint32 index() const { return __index; }
    bool canAct() const;
    int steps() const { return __index - startIndex; }
    bool canSurrender() const { return steps() >= 20; }

// Playing
public:
    void flipChess(const ChessPoint &pos);
    void moveChess(const ChessPoint &source, const ChessPoint &dest);
    void surrender(bool isOpposite);
// Game state
public:
    enum State { Flip, BlueMove, RedMove, BlueWin, RedWin, ThisWin, ThatWin };

    State state() const { return _state; }

private:
    State _state = Flip;
    int secondLatestFlippedSide = -1;
    int lastFlippedSide = -1;
    void updateFlipState(Chess::Side side);
    void updateResultState();
    void setState(State state);

signals:
    void didUpdateChesses();
    void chessDidFlip(const ChessPoint &pos);
    void chessDidMove(const ChessPoint &source, const ChessPoint &dest);
    void chessDidRemoved(const ChessPoint &point);
    void stateDidChange(State state, State oldState);
    void indexDidChange();
    void remainingTimeDidChange(int seconds);
    void thisPlayerDidTimeout(int times);
    void anotherPlayerDidTimeout(int times);
};


#endif // CHESSGAME_H
