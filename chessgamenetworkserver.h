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
    void stopListening();
    void sendChessboardData(QVector<Chess> data, qint32 startIndex);
    void disconnectFromHost();
private slots:
    void serverDidInitiateNewConnection();
};

#endif // CHESSGAMENETWORKSERVER_H
