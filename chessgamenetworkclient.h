#ifndef CHESSGAMENETWORKCLIENT_H
#define CHESSGAMENETWORKCLIENT_H

#include "chessgamenetworkbase.h"
#include "chess.h"

class ChessGameNetworkClient : public ChessGameNetworkBase {
    Q_OBJECT
private:
    QTcpSocket *socket;
    void handleReceivedData(QByteArray buffer);
public:
    ChessGameNetworkClient();
    void connectToHost(const QString &hostName);
    void disconnectFromHost();

signals:
    void didReceiveChessboardData(QVector<Chess> data);
};

#endif // CHESSGAMENETWORKCLIENT_H
