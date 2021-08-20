#ifndef CHESSPOINT_H
#define CHESSPOINT_H

#include <QPoint>
#include <QVector>
#include <QDataStream>

struct ChessPoint: public QPoint {
public:
    static const QVector<ChessPoint> CAMP_POINTS;
    static const QVector<ChessPoint> BREAK_POINTS;
    ChessPoint(int x, int y): QPoint(x, y) {}
    ChessPoint() = default;
    ChessPoint(int i): QPoint(i / 5, i % 5) {}
    operator int() const { return x() * 5 + y(); }
    bool isCamp() const;
    bool isOnRailway() const;
    bool isAround(const ChessPoint &another) const;

    friend QDataStream& operator<<(QDataStream &stream, const ChessPoint &point);
    friend QDataStream& operator>>(QDataStream &stream, ChessPoint &point);
};

#endif // CHESSPOINT_H
