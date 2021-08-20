#include "chessgamenetworkserver.h"
#include "constants.h"

ChessGameNetworkServer::ChessGameNetworkServer() {
    server = new QTcpServer();
}

void ChessGameNetworkServer::startListening() {
    server->listen(QHostAddress::Any, Constant::portNumber);
}

void ChessGameNetworkServer::serverDidInitiateNewConnection() {
    auto *socket = server->nextPendingConnection();
    connectToSocket(socket);
}

void ChessGameNetworkServer::sendChessboardData(QVector<Chess> data) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    stream << qint32(ChessGameNetworkBase::DataType::Chessboard);

    for (auto chess : data) {
        stream << chess;
    }

    sendBytes(bytes);
}
