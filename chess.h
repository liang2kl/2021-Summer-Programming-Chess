#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QPoint>
#include <QDataStream>
#include "chesspoint.h"

class Chess {
public:
    enum Type {
        Flag,
        Engineer, /*工兵*/
        PlatoonCommander, /*排长*/
        CompanyCommander, /*连长*/
        BattalionCommander, /*营长*/
        RegimentalCommander, /*团长*/
        Brigadier, /*旅长*/
        DivisionCommander, /*师长*/
        ArmyCommander, /*军长*/
        Commander, /*司令*/
        Bomb,
        Landmine,
    };

    enum Side { Blue, Red };

    enum EncounterResult {
        Success, Failure, Draw
    };

private:
    Type _type;
    Side _side;
    ChessPoint _position;
    bool _isFlipped = false;

public:
    Chess() {}
    Chess(Type type, Side side, ChessPoint position = ChessPoint()):
        _type(type), _side(side), _position(position) {}

    Type type() const { return _type; }
    ChessPoint position() const { return _position; }
    Side side() const { return _side; }
    Side reversedSide() const { return _side == Red ? Blue : Red; }
    bool isFlipped() const { return _isFlipped; }

    void setPosition(const ChessPoint &position) { _position = position; }
    void flip() { _isFlipped = true; }

    QString name() const;
    bool isMovable() const;


    EncounterResult encounter(Chess anotherChess) const;

    // Having no assertion about whether the points are valid.
    bool allowingMoveTo(const ChessPoint &dest);

    friend QDataStream &operator<<(QDataStream &stream, const Chess &chess);
    friend QDataStream &operator>>(QDataStream &stream, Chess &chess);
};

#endif // CHESS_H
