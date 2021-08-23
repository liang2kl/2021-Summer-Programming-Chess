#include "chessgamenetworkserver.h"
#include "constants.h"

ChessGameNetworkServer::ChessGameNetworkServer() {
    server = new QTcpServer();
}

void ChessGameNetworkServer::startListening() {
    server->listen(QHostAddress::Any, Constant::portNumber);
    connect(server, &QTcpServer::newConnection, this, &ChessGameNetworkServer::serverDidInitiateNewConnection);
}

void ChessGameNetworkServer::stopListening() {
    server->close();
}

void ChessGameNetworkServer::serverDidInitiateNewConnection() {
    auto *socket = server->nextPendingConnection();
    connectToSocket(socket);
    connect(socket, &QTcpSocket::stateChanged,
           this, &ChessGameNetworkServer::socketDidChangeState);
    emit didConnectToHost();
}

void ChessGameNetworkServer::sendChessboardData(QVector<Chess> data, qint32 startIndex) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    stream << qint32(ChessGameNetworkBase::DataType::Chessboard);

    stream << startIndex;

    for (auto chess : data) {
        stream << chess;
        qDebug() << chess.position();
    }

    sendBytes(bytes);
}
