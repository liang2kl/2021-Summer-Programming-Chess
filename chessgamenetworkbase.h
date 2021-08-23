#ifndef CHESSGAMENETWORKBASE_H
#define CHESSGAMENETWORKBASE_H

#include <QTcpSocket>
#include "chesspoint.h"

class ChessGameNetworkBase : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;

protected slots:
    void socketDidReceivedData();

protected:
    enum DataType { Flip = 200, Move, Chessboard };

    virtual void handleReceivedData(QByteArray buffer);
    void connectToSocket(QTcpSocket *socket);
    void sendBytes(QByteArray bytes);

signals:
    void didReceiveFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void didReceiveMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);
    void didConnectToHost();

//    void diFailToSendData(int index);
public:
    void sendFlipChessData(const ChessPoint &pos, qint32 operationIndex);
    void sendMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex);

};

#endif // CHESSGAMENETWORKBASE_H
