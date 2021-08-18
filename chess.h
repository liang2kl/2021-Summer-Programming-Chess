#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QPoint>
#include "chesspoint.h"

class Chess {
public:
    enum Type {
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
        Flag
    };

    enum Side { Blue, Red };

    enum EncounterResult {
        Success, Failure, Draw
    };

private:
    Type _type;
    ChessPoint _position;
    Side _side;
    bool _isFlipped = false;

public:
    Chess() {}
    Chess(Type type, Side side, ChessPoint position = ChessPoint()):
        _type(type), _position(position), _side(side) {}

    Type type() const { return _type; }
    ChessPoint position() const { return _position; }
    Side side() const { return _side; }
    bool isFlipped() { return _isFlipped; }

    void setPosition(const ChessPoint &position) { _position = position; }
    void flip() { _isFlipped = true; }

    QString name() const;
    bool isMovable();


    EncounterResult encounter(Chess anotherChess);

    // Having no assertion about whether the points are valid.
    bool allowingMoveTo(const ChessPoint &dest);
};

#endif // CHESS_H
