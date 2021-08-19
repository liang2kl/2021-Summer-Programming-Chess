#include <algorithm>
#include <random>
#include <QDebug>

#include "chessgame.h"
#include "constants.h"

ChessGame::ChessGame() {
    randomize();
//    _chesses[5] = new Chess(Chess::Type::Bomb, Chess::Side::Red, ChessPoint(1, 0));

    QVector<int> indices;
    QMap<int, int> map;
    int index = 0;
    for (int row = 1; row < 11; row++) {
        for (int column = 0; column < 5; column++) {
            if (row != 1 && row != 10 && !(column == 0 || column == 4)) {
                continue;
            }

            indices.append(row * 5 + column);
            map[row * 5 + column] = index;

            index += 1;
        }
    }

    railwayIndices = indices;
}

ChessGame * const ChessGame::shared = new ChessGame();

void ChessGame::randomize() {
    QVector<Chess *> chesses;

    for (int s = 0; s < 2; s++) {
        auto side = Chess::Side(s);

        chesses.append(new Chess(Chess::Type::Flag, side));
        chesses.append(new Chess(Chess::Type::Commander, side));
        chesses.append(new Chess(Chess::Type::ArmyCommander, side));

        for (int i = 0; i < 2; i++) {
            chesses.append(new Chess(Chess::Type::DivisionCommander, side));
            chesses.append(new Chess(Chess::Type::Brigadier, side));
            chesses.append(new Chess(Chess::Type::RegimentalCommander, side));
            chesses.append(new Chess(Chess::Type::BattalionCommander, side));
            chesses.append(new Chess(Chess::Type::Bomb, side));
        }

        for (int i = 0; i < 3; i++) {
            chesses.append(new Chess(Chess::Type::CompanyCommander, side));
            chesses.append(new Chess(Chess::Type::PlatoonCommander, side));
            chesses.append(new Chess(Chess::Type::Engineer, side));
            chesses.append(new Chess(Chess::Type::Landmine, side));
        }
    }

    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(chesses), std::end(chesses), rng);

    int campIndex1 = 0;
    int campIndex2 = 0;

    for (int i = 0; i < 50; i++) {
        const int column = i % 5;
        const int row = i / 5;

        if (i == 11 || i == 13 || i == 17 || i == 21 || i == 23) {
            chesses[i]->setPosition(ChessPoint(10, campIndex1));
            this->_chesses[50 + campIndex1] = chesses[i];
            campIndex1++;
        } else if (i == 36 || i == 38 || i == 42 || i == 46 || i == 48) {
            chesses[i]->setPosition(ChessPoint(11, campIndex2));
            this->_chesses[55 + campIndex2] = chesses[i];
            campIndex2++;
        } else {
            chesses[i]->setPosition(ChessPoint(row, column));
            this->_chesses[i] = chesses[i];
        }
    }
}

QVector<int> ChessGame::availablePointsFor(const Chess * chess) const {
    QVector<int> points;

    Graph g = railwayGraph();

    for (int i = 0; i < 60; i++) {
        if (_canMoveChess(chess->position(), ChessPoint(i / 5, i % 5), g)) {
            points.append(i);
        }
    }

    return points;
}

bool ChessGame::canMoveChess(const ChessPoint &source, const ChessPoint &dest) const {
    return _canMoveChess(source, dest, railwayGraph());
}

bool ChessGame::_canMoveChess(const ChessPoint &source, const ChessPoint &dest, const Graph &railwayGraph) const {
    // FIXME: Path to dest
    auto sourceChess = _chesses[source.index()];
    auto destChess = _chesses[dest.index()];

    assert(sourceChess != nullptr);

    if (!sourceChess->isMovable()) {
        return false;
    }

    if (!destChess) {
//        qDebug() << "NOT" << dest;
        return sourceChess->allowingMoveTo(dest);
    }

    if (!destChess->isFlipped()) {
        return false;
    }

    if (sourceChess->side() == destChess->side()) { return false; }

    if (!sourceChess->allowingMoveTo(dest)) {
        return false;
    }

    // It takes special care for chesses that
    // can move over more than one slot.
    if (source.isOnRailway() && dest.isOnRailway()) {
        if (!allowingPointOnRailwayMoveTo(source, dest, sourceChess->type() == Chess::Type::Engineer, railwayGraph)) {
            return false;
        }
    }

    return sourceChess->encounter(*destChess) != Chess::EncounterResult::Failure;
}

QVector<const Chess *> ChessGame::chesses() const {
    QVector<const Chess *> constChesses;

    for (auto chess : _chesses) {
        constChesses.append(chess);
    }

    return constChesses;
}

// Call this after validating that the chess is allowed to move to the destination point.
bool ChessGame::allowingPointOnRailwayMoveTo(const ChessPoint &source, const ChessPoint &dest, bool isEngineer, const Graph &railwayGraph) const {

    if (source.x() == dest.x() && allowingHorizontallyMoveTo(source, dest)) {
        return true;
    }

    if (source.y() == dest.y() && allowingVerticallyMoveTo(source, dest)) {
        return true;
    }

    if (!isEngineer) { return false; }

    // Engineer scenario

    return railwayGraph.isConnected(railwayIndexMap[indexOfPoint(source)], railwayIndexMap[indexOfPoint(dest)]);
}


bool ChessGame::allowingVerticallyMoveTo(const ChessPoint &source, const ChessPoint &dest) const {
    assert(source.y() == dest.y());

    auto minX = std::min(source.x(), dest.x());
    auto maxX = std::max(source.x(), dest.x());

    for (int row = minX + 1; row < maxX; row++) {
        if (_chesses[row * 5 + source.y()]) {
            return false;
        }
    }
    return true;
}

bool ChessGame::allowingHorizontallyMoveTo(const ChessPoint &source, const ChessPoint &dest) const {
    assert(source.x() == dest.x());

    auto minY = std::min(source.y(), dest.y());
    auto maxY = std::max(source.y(), dest.y());

    for (int column = minY + 1; column < maxY; column++) {
        if (_chesses[source.x() * 5 + column]) {
            return false;
        }
    }
    return true;
}

Graph ChessGame::railwayGraph() const {
    Graph g = Graph(32);

    int i = -1;

    auto fullRow = [](int r) -> bool {
        return r == 1 || r == 5 || r == 6 || r == 10;
    };

    for (int row = 1; row < 11; row++) {
        for (int column = 0; column < 5; column++) {
            if (row != 1 && row != 10 && !(column == 0 || column == 4)) {
                continue;
            }
            i += 1;

            int index = row * 5 + column;
            // Occupied
            if (_chesses[index]) { continue; }

            if (fullRow(row)) {
                if (column != 4 && !_chesses[index + 1]) { g.addEdge(i, i + 1); }
                if (column != 0 && !_chesses[index - 1]) { g.addEdge(i, i - 1); }
            }

            if (column == 0 || column == 4) {
                if (row != 10 && !_chesses[index + 5]) { g.addEdge(i, i + (fullRow(row) ? 5 : 2)); }
                if (row != 1 && !_chesses[index - 5]) { g.addEdge(i, i - (fullRow(row - 1) ? 5 : 2)); }
            }

        }
    }

    if (!_chesses[27] && !_chesses[32]) {
        g.addEdge(13, 18);
        g.addEdge(18, 13);
    }

    return g;
}

// Playing

void ChessGame::flipChess(const ChessPoint &pos) {
    auto *chess = _chesses[indexOfPoint(pos)];
    chess->flip();
    emit chessDidFlip(pos);
    updateFlipState(chess->side());
}

void ChessGame::moveChess(const ChessPoint &source, const ChessPoint &dest) {
    assert(canMoveChess(source, dest));
    auto *srcChess = _chesses[indexOfPoint(source)];
    auto *desChess = _chesses[indexOfPoint(dest)];

    if (!desChess) {
        srcChess->setPosition(dest);
        _chesses[indexOfPoint(dest)] = _chesses[indexOfPoint(source)];
        _chesses[indexOfPoint(source)] = nullptr;
        emit chessDidMove(source, dest);
    } else {
        auto result = srcChess->encounter(*desChess);

        if (result == Chess::EncounterResult::Success) {
            srcChess->setPosition(dest);
            _chesses[indexOfPoint(dest)] = _chesses[indexOfPoint(source)];
            _chesses[indexOfPoint(source)] = nullptr;

            emit chessDidRemoved(dest);
            emit chessDidMove(source, dest);
        } else {
            _chesses[indexOfPoint(dest)] = nullptr;
            _chesses[indexOfPoint(source)] = nullptr;

            emit chessDidRemoved(dest);
            emit chessDidRemoved(source);
        }
    }

    updateResultState();
}


// Game state

void ChessGame::updateFlipState(Chess::Side side) {
    if (_state == Flip) {
        if (lastFlippedSide == side) {
            setState(side == Chess::Side::Red ? BlueMove : RedMove);
        } else {
            lastFlippedSide = side;
        }
    }
}

void ChessGame::updateResultState() {
    assert(_state == BlueMove || _state == RedMove);
    // Flags
    bool redFlagExist = false;
    bool blueFlagExist = true;
    for (const auto *chess : _chesses) {
        if (!chess) { continue; }
        if (chess->type() == Chess::Type::Flag) {
            if (chess->side() == Chess::Side::Red) {
                redFlagExist = true;
            } else {
                blueFlagExist = true;
            }
        }
    }

    if (!redFlagExist) {
        setState(BlueWin);
        return;
    }
    if (!blueFlagExist) {
        setState(RedWin);
        return;
    }

    // TODO: Not movable

    // Toggle movable side
    setState(_state == RedMove ? BlueMove : RedMove);
}

void ChessGame::setState(State state) {
    emit stateDidChange(state, _state);
    _state = state;
}
