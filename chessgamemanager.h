#ifndef CHESSGAMEMANAGER_H
#define CHESSGAMEMANAGER_H

#include <QObject>
#include "chessgamenetworkclient.h"
#include "chessgamenetworkserver.h"
#include "chessgame.h"

class ChessGameManager : public QObject
{
    Q_OBJECT
private:
    bool isServer;
    ChessGameNetworkClient *client;
    ChessGameNetworkServer *server;
    ChessGame *_game;

public:
    ChessGameManager(bool isServer);
    void flipChess(const ChessPoint &pos);
    void moveChess(const ChessPoint &src, const ChessPoint &des);
    ChessGame const * game() { return _game; }

signals:

};

#endif // CHESSGAMEMANAGER_H
