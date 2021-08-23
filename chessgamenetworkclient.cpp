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
    qDebug() << "Connecting";
}

void ChessGameNetworkClient::disconnectFromHost() {
    socket->disconnectFromHost();
}

void ChessGameNetworkClient::handleReceivedData(QByteArray buffer) {
    QByteArray copy = buffer;
    qint32 type;

    QDataStream stream(&buffer, QIODevice::ReadOnly);

    stream >> type;

    if (type == ChessGameNetworkClient::DataType::Flip ||
            type == ChessGameNetworkClient::DataType::Move) {
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
