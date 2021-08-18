#ifndef CHESS_H
#define CHESS_H

#include <QString>

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

    enum EncounterResult {
        Success, Failure, Draw
    };

private:
    Type _type;

public:
    Chess() {}
    Chess(Type type): _type(type) {}

    Type type() { return _type; }

    QString name() {
        switch (_type) {
        case Engineer: return "工兵";
        case PlatoonCommander: return "排长";
        case CompanyCommander: return "连长";
        case BattalionCommander: return "营长";
        case RegimentalCommander: return "团长";
        case Brigadier: return "旅长";
        case DivisionCommander: return "师长";
        case ArmyCommander: return "军长";
        case Commander: return "司令";
        case Bomb: return "炸弹";
        case Landmine: return "地雷";
        case Flag: return "军旗";
        }
    }

    bool isMovable() {
        switch (_type) {
        case Landmine: return false;
        case Flag: return false;
        default: return true;
        }
    }

    EncounterResult encounter(Chess anotherChess) {
        Type another = anotherChess.type();
        // These type of chesses cannot move
        assert(_type != Landmine && _type != Flag);

        if (_type == Bomb) { return Draw; }
        if (_type == Engineer && another == Landmine) {
            return Success;
        }
        if (_type == another) { return Draw; }
        return _type > another ? Success : Failure;
    }
};

#endif // CHESS_H
