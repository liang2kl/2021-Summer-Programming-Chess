#include <algorithm>
#include <random>
#include <QDebug>

#include "chessgame.h"
#include "constants.h"

ChessGame::ChessGame() {
    randomize();
}

void ChessGame::randomize() {
    QVector<ChessInfo> chesses;

    for (int s = 0; s < 2; s++) {
        auto side = ChessInfo::Side(s);

        chesses.emplace_back(Chess::Type::Flag, side);
        chesses.emplace_back(Chess::Type::Commander, side);
        chesses.emplace_back(Chess::Type::ArmyCommander, side);

        for (int i = 0; i < 2; i++) {
            chesses.emplace_back(Chess::Type::DivisionCommander, side);
            chesses.emplace_back(Chess::Type::Brigadier, side);
            chesses.emplace_back(Chess::Type::RegimentalCommander, side);
            chesses.emplace_back(Chess::Type::BattalionCommander, side);
            chesses.emplace_back(Chess::Type::Bomb, side);
        }

        for (int i = 0; i < 3; i++) {
            chesses.emplace_back(Chess::Type::CompanyCommander, side);
            chesses.emplace_back(Chess::Type::PlatoonCommander, side);
            chesses.emplace_back(Chess::Type::Engineer, side);
            chesses.emplace_back(Chess::Type::Landmine, side);
        }

    }

    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(chesses), std::end(chesses), rng);

    for (int i = 0; i < 50; i++) {
        const int column = i % 5;
        const int row = i / 5;
        chesses[i].position = QPoint(row, column);
    }

    int campIndex1 = 0;
    int campIndex2 = 0;

    for (auto &chess : chesses) {
        const int index = chess.position.x() * 5 + chess.position.y();
        if (index == 11 || index == 13 || index == 17 || index == 21 || index == 23) {
            chess.position = QPoint(10, campIndex1);
            campIndex1++;
        } else if (index == 36 || index == 38 || index == 42 || index == 46 || index == 48) {
            chess.position = QPoint(11, campIndex2);
            campIndex2++;
        }

        qDebug() << chess.chess.name() << " " << chess.side << " " << chess.position;
    }

    this->chesses = chesses;
}
