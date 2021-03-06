#include <algorithm>
#include <random>
#include <QDebug>

#include "chessgame.h"
#include "constants.h"

ChessGame::ChessGame() {
    QVector<int> indices;
    QMap<int, int> map;
    int index = 0;
    for (int row = 1; row < 11; row++) {
        for (int column = 0; column < 5; column++) {
            if (row != 1 && row != 10 && row != 5 && row != 6 && !(column == 0 || column == 4)) {
                continue;
            }

            indices.append(row * 5 + column);
            map[row * 5 + column] = index;

            index += 1;
        }
    }

    railwayIndices = indices;
    railwayIndexMap = map;

    updateTimer = new QTimer();
    updateTimer->setInterval(1000);
    updateTimer->callOnTimeout([this]() {
        this->currentSecond--;
        emit remainingTimeDidChange(this->currentSecond);

        if (this->currentSecond == 0) {
            this->updateTimeout();
        }
    });
}

void ChessGame::setChesses(QVector<Chess> chesses) {
    for (auto chess : chesses) {
        _chesses[chess.position()] = new Chess(chess);
    }
    emit didUpdateChesses();
}

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

    for (int i = 0; i < 60; i++) {
        if (canMoveChess(chess->position(), ChessPoint(i / 5, i % 5))) {
            points.append(i);
        }
    }

    return points;
}

bool ChessGame::canMoveChess(const ChessPoint &source, const ChessPoint &dest) const {
    auto sourceChess = _chesses[source];
    auto destChess = _chesses[dest];

    assert(sourceChess != nullptr);

    if (!sourceChess->isMovable()) {
        return false;
    }

    if (!destChess) {
        if (source.isOnRailway() && dest.isOnRailway()) {
            auto g = railwayGraph(source, dest);
            return allowingPointOnRailwayMoveTo(source, dest, sourceChess->type() == Chess::Type::Engineer, g);
        } else {
            // Only allowed when they are around, if one chess is not on railway
            return sourceChess->allowingMoveTo(dest);
        }
    }

    if (!destChess->isFlipped()) {
        return false;
    }

    if (sourceChess->side() == destChess->side()) { return false; }

    if (dest.isCamp()) { return false; }

    // Special care for Flags.
    if (destChess->type() == Chess::Type::Flag) {
        if (!isLandmineCleared(destChess->side())) {
            return false;
        }
    }

    if (!sourceChess->allowingMoveTo(dest)) {
        return false;
    }

    // It takes special care for chesses that
    // can move over more than one slot.
    if (source.isOnRailway() && dest.isOnRailway()) {
        auto g = railwayGraph(source, dest);
        if (!allowingPointOnRailwayMoveTo(source, dest, sourceChess->type() == Chess::Type::Engineer, g)) {
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
// The dest point must be included in the graph.
bool ChessGame::allowingPointOnRailwayMoveTo(const ChessPoint &source, const ChessPoint &dest, bool isEngineer, const Graph &railwayGraph) const {

    if (source.x() == dest.x() && allowingHorizontallyMoveTo(source, dest)) {
        return true;
    }

    if (source.y() == dest.y() && allowingVerticallyMoveTo(source, dest)) {
        return true;
    }

    if (!isEngineer) { return false; }

    // Engineer scenario

    return railwayGraph.isConnected(railwayIndexMap[source], railwayIndexMap[dest]);
}


bool ChessGame::allowingVerticallyMoveTo(const ChessPoint &source, const ChessPoint &dest) const {
    assert(source.y() == dest.y());

    const int column = source.y();
    if (column != 0 && column != 4 &&
            !(source == ChessPoint(5, 2) && dest == ChessPoint(6, 2)) &&
            !(source == ChessPoint(6, 2) && dest == ChessPoint(5, 2))
            ) {
        return false;
    }
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

    const int row = source.x();
    if (row != 1 && row != 5 && row != 6 && row != 10) { return false; }

    auto minY = std::min(source.y(), dest.y());
    auto maxY = std::max(source.y(), dest.y());

    for (int column = minY + 1; column < maxY; column++) {
        if (_chesses[source.x() * 5 + column]) {
            return false;
        }
    }
    return true;
}

bool ChessGame::isLandmineCleared(Chess::Side side) const {
    for (auto *chess : _chesses) {
        if (chess &&
            chess->side() == side &&
            chess->type() == Chess::Type::Landmine) {
            return false;
        }
    }

    return true;
}

bool ChessGame::chessCanChangePosition(const Chess *chess) {
    if (!chess->isMovable()) { return false; }

    for (int i = 0; i < 60; i++) {
        if (canMoveChess(chess->position(), i)) {
            return true;
        }
    }
    return false;
}

Graph ChessGame::railwayGraph(int startIndex, int endIndex) const {
    Graph g = Graph(32);

    // Remove start vertex.
    auto chesses = this->_chesses;
    chesses[startIndex] = nullptr;
    chesses[endIndex] = nullptr;

    auto fullRow = [](int r) -> bool {
        return r == 1 || r == 5 || r == 6 || r == 10;
    };

    auto offsetsByPreviousRow = [fullRow](int row, int column) -> int {
        assert(column == 0 || column == 4);
        return column == 0 ?
                    (fullRow(row - 1) ? 5 : 2) :
                    (fullRow(row) ? 5 : 2);
    };

    int i = -1;

    for (int row = 1; row < 11; row++) {
        for (int column = 0; column < 5; column++) {
            if (row != 1 && row != 10 && row != 5 && row != 6 && !(column == 0 || column == 4)) {
                continue;
            }
            i += 1;

            int index = row * 5 + column;
            // Occupied
            if (chesses[index]) { continue; }

            if (fullRow(row)) {
                if (column != 4 && !chesses[index + 1]) {
                    g.addEdge(i, i + 1);
                }
                if (column != 0 && !chesses[index - 1]) {
                    g.addEdge(i, i - 1);
                }
            }

            if (column == 0 || column == 4) {
                if (row != 10 && !chesses[index + 5]) {
                    g.addEdge(i, i + offsetsByPreviousRow(row + 1, column));
                }
                if (row != 1 && !chesses[index - 5]) {
                    g.addEdge(i, i - offsetsByPreviousRow(row, column));
                }
            }

        }
    }

    if (!chesses[27] && !chesses[32]) {
        g.addEdge(13, 18);
        g.addEdge(18, 13);
    }

    return g;
}

// Playing

void ChessGame::flipChess(const ChessPoint &pos) {
    auto *chess = _chesses[pos];
    chess->flip();
    emit chessDidFlip(pos);
    increaseIndex();
    updateFlipState(chess->side());
}

void ChessGame::moveChess(const ChessPoint &source, const ChessPoint &dest) {
    assert(canMoveChess(source, dest));
    assert((_state == BlueMove && _chesses[source]->side() == Chess::Side::Blue) ||
            (_state == RedMove && _chesses[source]->side() == Chess::Side::Red));

    auto *srcChess = _chesses[source];
    auto *desChess = _chesses[dest];

    if (!desChess) {
        srcChess->setPosition(dest);
        _chesses[dest] = _chesses[source];
        _chesses[source] = nullptr;
        emit chessDidMove(source, dest);
    } else {
        auto result = srcChess->encounter(*desChess);

        if (result == Chess::EncounterResult::Success) {
            srcChess->setPosition(dest);
            _chesses[dest] = _chesses[source];
            _chesses[source] = nullptr;

            emit chessDidRemoved(dest);
            emit chessDidMove(source, dest);
        } else {
            _chesses[dest] = nullptr;
            _chesses[source] = nullptr;

            emit chessDidRemoved(dest);
            emit chessDidRemoved(source);
        }
    }
    increaseIndex();
    updateResultState();
}

void ChessGame::surrender(bool isOpposite) {
    assert(canSurrender());
    setState(isOpposite ? ThisWin : ThatWin);
}

void ChessGame::start() {
    started = true;
    emit didStarted();
    startTimer();
}

bool ChessGame::canAct() const {
    if (!started) { return false; }
    if (_state == BlueWin || _state == RedWin ||
            _state == ThisWin || _state == ThatWin) {
        return false;
    }

    return (__index % 2) ^ isServer;
}

void ChessGame::startTimer() {
    currentSecond = 20;
    updateTimer->start();
    emit remainingTimeDidChange(20);
}

void ChessGame::stopTimer() {
    updateTimer->stop();
}

void ChessGame::setStartIndex(qint32 index) {
    startIndex = index;
    __index = startIndex;
    emit indexDidChange();
}

void ChessGame::increaseIndex() {
    __index += 1;
    stopTimer();
    emit indexDidChange();
    startTimer();
}

void ChessGame::updateTimeout() {
    if (canAct()) {
        thisTimeoutCount++;
        emit thisPlayerDidTimeout(thisTimeoutCount);
        if (thisTimeoutCount == 3) {
            setState(ThatWin);
            return;
        }
    } else {
        anotherTimeoutCount++;
        emit anotherPlayerDidTimeout(anotherTimeoutCount);
        if (anotherTimeoutCount == 3) {
            setState(ThisWin);
            return;
        }
    }
    increaseIndex();

    if (_state != Flip) {
        setState(_state == RedMove ? BlueMove : RedMove);
    }
}

// Game state

void ChessGame::updateFlipState(Chess::Side side) {
    if (_state == Flip) {

        if (secondLatestFlippedSide == side) {
            setState(side == Chess::Side::Red ? BlueMove : RedMove);
        } else {
            secondLatestFlippedSide = lastFlippedSide;
            lastFlippedSide = side;
        }

    } else {
        setState(_state == RedMove ? BlueMove : RedMove);
    }
}

void ChessGame::updateResultState() {
    assert(_state == BlueMove || _state == RedMove);
    // Flags
    bool redFlagExist = false;
    bool blueFlagExist = false;
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

    // Movable Logic
    bool canRedMove = false;
    bool canBlueMove = false;
    for (auto *chess : _chesses) {
        if (!chess) { continue; }
        if (!chess->isFlipped()) {
            canRedMove = true;
            canBlueMove = true;
            break;
        }
        if (canRedMove && chess->side() == Chess::Side::Red) {
            continue;
        }
        if (canBlueMove && chess->side() == Chess::Side::Blue) {
            continue;
        }
        if (chessCanChangePosition(chess)) {
            if (chess->side() == Chess::Side::Red) {
                canRedMove = true;
            } else {
                canBlueMove = true;
            }
        }
    }

    if (!canRedMove) {
        setState(BlueWin);
        return;
    } else if (!canBlueMove) {
        setState(RedWin);
        return;
    }

    // Toggle movable side
    setState(_state == RedMove ? BlueMove : RedMove);
}

void ChessGame::setState(State state) {
    if (state == BlueWin || state == RedWin ||
            state == ThisWin || state == ThatWin) {
        stopTimer();
    }
    emit stateDidChange(state, _state);
    _state = state;
}
