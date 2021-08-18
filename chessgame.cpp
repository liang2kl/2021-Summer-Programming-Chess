#include <algorithm>
#include <random>
#include <QDebug>

#include "chessgame.h"
#include "constants.h"

ChessGame::ChessGame() {
    randomize();
}

const ChessGame *ChessGame::shared = new ChessGame();

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


    for (auto chess : this->_chesses) {
        if (chess) {
            qDebug() << chess->name() << " " << chess->side() << " " << chess->position();
        }
    }
}

bool ChessGame::canMoveChess(const ChessPoint &source, const ChessPoint &dest) {
    auto sourceChess = _chesses[indexOfPoint(source)];
    auto destChess = _chesses[indexOfPoint(dest)];

    assert(sourceChess != nullptr);

    if (!sourceChess->isMovable()) {
        return false;
    }

    if (!destChess) {
        return true;
    }

    if (!sourceChess->allowingMoveTo(dest)) {
        return false;
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
