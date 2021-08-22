#ifndef CHESSGAMENETWORKSERVER_H
#define CHESSGAMENETWORKSERVER_H

#include "chessgamenetworkbase.h"
#include "chess.h"
#include <QTcpServer>

class ChessGameNetworkServer : public ChessGameNetworkBase
{
    Q_OBJECT
private:
    QTcpServer *server;
public:
    ChessGameNetworkServer();
    void startListening();
    void sendChessboardData(QVector<Chess> data);

private slots:
    void serverDidInitiateNewConnection();

signals:
    void didConnectToHost();

};

#endif // CHESSGAMENETWORKSERVER_H
