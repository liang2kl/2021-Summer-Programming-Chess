#include "chess.h"

QString Chess::name() const {
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

bool Chess::isMovable() const {
    return _type != Landmine && _type != Flag;
}

Chess::EncounterResult Chess::encounter(Chess anotherChess) const {
    // FIXME: Flag
    assert(_side != anotherChess._side);

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

// Whether the moving rule allows this chess to move to a destination point.
// This does not indicate that the chess can actually move to that point.
bool Chess::allowingMoveTo(const ChessPoint &dest) {
    assert(isMovable());

    const bool isAround = _position.isAround(dest);

    if (isAround) { return true; }

    if (_type == Engineer) {
        return _position.isOnRailway() && dest.isOnRailway();
    }

    if (!_position.isOnRailway()) { return false; }

    if (_position.x() == dest.x() && (_position.x() == 1 || _position.x() == 5 || _position
                                      .x() == 6 || _position.x() == 10)) { return true; }

    if ((_position.y() == 0 && dest.y() == 0) ||
            (_position.y() == 4 && dest.y() == 4)) {
        // Exclude (0, 0) and (11, 0)
        return dest.x() != 0 && dest.x() != 11;
    }

    if ((_position.x() == 5 && _position.y() == 2 &&
         dest.x() == 6 && _position.y() == 2) ||
            (_position.x() == 6 && _position.y() == 2
             && dest.x() == 5 && _position.y() == 2)) {
        return true;
    }

    return false;
}
