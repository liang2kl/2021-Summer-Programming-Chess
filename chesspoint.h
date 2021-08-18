#ifndef CHESSPOINT_H
#define CHESSPOINT_H

#include <QPoint>
#include <QVector>

class ChessPoint: public QPoint {
public:
    static const QVector<ChessPoint> CAMP_POINTS;
    static const QVector<ChessPoint> BREAK_POINTS;
    ChessPoint(int x, int y): QPoint(x, y) {}
    ChessPoint() = default;
    bool isCamp() const;
    bool isOnRailway() const;
    bool isAround(const ChessPoint &another) const;
};


#endif // CHESSPOINT_H
