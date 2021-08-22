#include "chessgamenetworkclient.h"
#include "constants.h"

ChessGameNetworkClient::ChessGameNetworkClient() {
    socket = new QTcpSocket();
    connectToSocket(socket);
}

void ChessGameNetworkClient::connectToHost(const QString &hostName) {
    socket->connectToHost(hostName, Constant::portNumber);
    qDebug() << "Connecting to" << hostName;
}

void ChessGameNetworkClient::disconnectFromHost() {
    socket->disconnectFromHost();
}

void ChessGameNetworkClient::handleReceivedData(QByteArray buffer) {
    QByteArray copy = buffer;
    qint32 size;
    ChessGameNetworkClient::DataType type;

    QDataStream stream(&buffer, QIODevice::ReadOnly);

    stream >> size >> type;

    if (type == ChessGameNetworkClient::DataType::Flip ||
            type == ChessGameNetworkClient::DataType::Move) {
        return ChessGameNetworkBase::handleReceivedData(copy);
    }

    QVector<Chess> data;

    for (int i = 0; i < 50; i++) {
        Chess chess;
        stream >> chess;
        data.append(chess);
    }

    emit didReceiveChessboardData(data);
}
