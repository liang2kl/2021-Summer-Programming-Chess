#ifndef CHESSGAMENETWORKBASE_H
#define CHESSGAMENETWORKBASE_H

#include <QTcpSocket>
#include "chesspoint.h"

class ChessGameNetworkBase : public QObject
{
    Q_OBJECT
protected slots:
    void socketDidReceivedData();
    void socketDidChangeState(QAbstractSocket::SocketState);

protected:
    enum DataType { Flip = 200, Move, Chessboard, Surrender, Start };

    QTcpSocket *socket;

    virtual void handleReceivedData(QByteArray buffer);
    void connectToSocket(QTcpSocket *socket);
    void sendBytes(QByteArray bytes);

signals:
    void didReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void didReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);
    void didReceiveStart();
    void didReceiveSurrender();
    void didConnectToHost();
    void didFailToConnectToHost();
    void didLostConnection();

public:
    virtual void disconnectFromHost() = 0;
    void sendFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void sendMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);
    void sendStart();
    void sendSurrender();
};

#endif // CHESSGAMENETWORKBASE_H
