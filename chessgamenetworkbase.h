#ifndef CHESSGAMENETWORKBASE_H
#define CHESSGAMENETWORKBASE_H

#include <QTcpSocket>
#include "chesspoint.h"

class ChessGameNetworkBase : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;
    void sendBytes(QByteArray bytes);

protected slots:
    void socketDidReceivedData();

protected:
    enum DataType { Flip, Move, Chessboard };

    virtual void handleReceivedData(QByteArray buffer);
    void connectToSocket(QTcpSocket *socket);
    void sendFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void sendMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);

signals:
    void didReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void didReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);
    void diFailToSendData(int index);
};

#endif // CHESSGAMENETWORKBASE_H
