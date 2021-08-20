#include "chessgamemanager.h"

ChessGameManager::ChessGameManager(bool isServer) : isServer(isServer) {
    if (isServer) {
        server = new ChessGameNetworkServer();

    } else {
        client = new ChessGameNetworkClient();
    }
}

void ChessGameManager::flipChess(const ChessPoint &) {
    // TODO
}

void ChessGameManager::moveChess(const ChessPoint &, const ChessPoint &) {
    // TODO
}
