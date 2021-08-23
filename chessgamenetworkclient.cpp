#include "chessgamenetworkclient.h"
#include "constants.h"

ChessGameNetworkClient::ChessGameNetworkClient() {
    socket = new QTcpSocket();
    connectToSocket(socket);
}

void ChessGameNetworkClient::connectToHost(const QString &hostName) {
    socket->connectToHost(hostName, Constant::portNumber);
    connect(socket, &QTcpSocket::connected,
            this, &ChessGameNetworkBase::didConnectToHost);
    connect(socket, &QTcpSocket::errorOccurred,
            this, &ChessGameNetworkBase::didFailToConnectToHost);
    connect(socket, &QTcpSocket::stateChanged,
           this, &ChessGameNetworkClient::socketDidChangeState);
    qDebug() << "Connecting";
}

void ChessGameNetworkClient::handleReceivedData(QByteArray buffer) {
    QByteArray copy = buffer;
    qint32 type;

    QDataStream stream(&buffer, QIODevice::ReadOnly);

    stream >> type;

    if (type != ChessGameNetworkBase::DataType::Chessboard) {
        return ChessGameNetworkBase::handleReceivedData(copy);
    }

    qint32 startIndex;
    stream >> startIndex;

    QVector<Chess> data;

    for (int i = 0; i < 50; i++) {
        Chess chess;
        stream >> chess;
        data.append(chess);
    }

    emit didReceiveChessboardData(data, startIndex);
}
