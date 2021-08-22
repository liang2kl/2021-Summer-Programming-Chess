#include "chessgamemanager.h"

ChessGameManager::ChessGameManager(bool isServer) : isServer(isServer) {
    _game = new ChessGame();
    ChessGameNetworkBase *base;
    if (isServer) {
        _game->randomize();
        server = new ChessGameNetworkServer();
        server->startListening();
        connect(server, &ChessGameNetworkServer::didConnectToHost,
                this, &ChessGameManager::networkDidConnectToHost);
        base = server;
    } else {
        client = new ChessGameNetworkClient();
        base = client;
        connect(client, &ChessGameNetworkClient::didReceiveChessboardData,
                this, &ChessGameManager::clientDidReceiveChessboardData);
    }

    connect(base, &ChessGameNetworkBase::didReceiveFlipChessData,
            this, &ChessGameManager::networkDidReceiveFlipChessData);
    connect(base, &ChessGameNetworkBase::didReceiveMoveChessData,
            this, &ChessGameManager::networkDidReceiveMoveChessData);
}

void ChessGameManager::connectToServer(const QString &hostName) {
    assert(!isServer);
    client->connectToHost(hostName);
}

void ChessGameManager::flipChess(const ChessPoint &pos) {
    // TODO
    _game->flipChess(pos);
    if (isServer) {
        server->sendFlipChessData(pos, 0);
    } else {
        client->sendFlipChessData(pos, 0);
    }
}

void ChessGameManager::moveChess(const ChessPoint &src, const ChessPoint &dest) {
    // TODO
    _game->moveChess(src, dest);
    if (isServer) {
        server->sendMoveChessData(src, dest, 0);
    } else {
        client->sendMoveChessData(src, dest, 0);
    }

}

void ChessGameManager::networkDidConnectToHost() {
    emit didConnectToHost();
    QVector<Chess> chesses;
    for (auto *chess : _game->chesses()) {
        if (chess) {
            chesses.append(*chess);
        }
    }
    server->sendChessboardData(chesses);
}

void ChessGameManager::networkDidReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex) {
    // TODO: Index
    _game->flipChess(pos);
}

void ChessGameManager::networkDidReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex) {
    _game->moveChess(src, des);
}

void ChessGameManager::clientDidReceiveChessboardData(QVector<Chess> data) {
    _game->setChesses(data);
}
