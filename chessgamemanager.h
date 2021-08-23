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
    ChessGame const * game() { return _game; }

    void connectToServer(const QString &hostName);
    void startListening();

    void flipChess(const ChessPoint &pos);
    void moveChess(const ChessPoint &src, const ChessPoint &des);

private slots:
    // General
    void networkDidConnectToHost();
    void networkDidReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void networkDidReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);

    // Client
    void clientDidReceiveChessboardData(QVector<Chess> data, qint32 startIndex);

signals:
    void didConnectToHost();
};

#endif // CHESSGAMEMANAGER_H
