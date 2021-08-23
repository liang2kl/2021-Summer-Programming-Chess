#include "chessgamemanager.h"

#include <random>
#include <QRandomGenerator>

ChessGameManager::ChessGameManager(bool isServer) : isServer(isServer) {
    _game = new ChessGame();
    _game->setIsServer(isServer);

    if (isServer) {
        _game->randomize();
        server = new ChessGameNetworkServer();
        server->startListening();
        connect(server, &ChessGameNetworkBase::didConnectToHost,
                this, &ChessGameManager::networkDidConnectToHost);
        network = server;
    } else {
        client = new ChessGameNetworkClient();
        network = client;
        connect(client, &ChessGameNetworkBase::didConnectToHost,
                this, &ChessGameManager::networkDidConnectToHost);
        connect(client, &ChessGameNetworkClient::didReceiveChessboardData,
                this, &ChessGameManager::clientDidReceiveChessboardData);
    }

    connect(network, &ChessGameNetworkBase::didReceiveFlipChessData,
            this, &ChessGameManager::networkDidReceiveFlipChessData);
    connect(network, &ChessGameNetworkBase::didReceiveMoveChessData,
            this, &ChessGameManager::networkDidReceiveMoveChessData);
}

void ChessGameManager::connectToServer(const QString &hostName) {
    assert(!isServer);
    client->connectToHost(hostName);
}

void ChessGameManager::flipChess(const ChessPoint &pos) {
    // TODO
    assert(_game->canAct());
    _game->flipChess(pos);
    network->sendFlipChessData(pos, _game->index());
}

void ChessGameManager::moveChess(const ChessPoint &src, const ChessPoint &dest) {
    // TODO
    assert(_game->canAct());
    _game->moveChess(src, dest);
    network->sendMoveChessData(src, dest, _game->index());
}

void ChessGameManager::surrender() {
    _game->surrender(false);
    network->sendSurrender();
}

void ChessGameManager::networkDidConnectToHost() {
    emit didConnectToHost();

    if (isServer) {
        QVector<Chess> chesses;
        for (auto *chess : _game->chesses()) {
            if (chess) {
                chesses.append(*chess);
            }
        }

        auto generator = QRandomGenerator();
        auto rd = std::random_device {};
        generator.seed(rd());
        qint32 startIndex = generator.bounded(0, 20);
        _game->setStartIndex(startIndex);

        server->sendChessboardData(chesses, startIndex);
    }
}

void ChessGameManager::networkDidReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex) {
    // TODO: Index
    assert(operationIndex == _game->index() + 1);
    _game->flipChess(pos);
}

void ChessGameManager::networkDidReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex) {
    assert(operationIndex == _game->index() + 1);
    _game->moveChess(src, des);
}

void ChessGameManager::networkDidReceiveSurrender() {
    _game->surrender(true);
}

void ChessGameManager::clientDidReceiveChessboardData(QVector<Chess> data, qint32 startIndex) {
    _game->setStartIndex(startIndex);
    _game->setChesses(data);
}
