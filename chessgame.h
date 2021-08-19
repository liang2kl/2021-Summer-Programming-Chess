#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "chess.h"
#include "graph.h"

#include <QPoint>
#include <QVector>
#include <QObject>
#include <QMap>

class ChessGame: QObject {
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
    bool _canMoveChess(const ChessPoint &source, const ChessPoint &dest, const Graph &railwayGraph) const;


    Graph railwayGraph() const;
    int graphIndexToIndex() const;
signals:
    void chessDidMove(const ChessPoint &source, const ChessPoint &dest);
    void chessDidRemoved(const ChessPoint &point);

public:
    QVector<int> availablePointsFor(const Chess *) const;
    bool canMoveChess(const ChessPoint &source, const ChessPoint &dest) const;
    QVector<const Chess *> chesses() const;
    void randomize();
    // Game singleton
    static const ChessGame *shared;
};



#endif // CHESSGAME_H
