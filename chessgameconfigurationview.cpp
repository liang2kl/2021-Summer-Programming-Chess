#include "chessgameconfigurationview.h"
#include "chessgameview.h"
#include <QDebug>
#include <QLabel>
#include <QNetworkInterface>
#include <QRegularExpressionValidator>

ChessGameConfigurationView::ChessGameConfigurationView(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    auto *topGroupBox = new QGroupBox();

    layout->addWidget(topGroupBox);

    auto *serverRadioButton = new QRadioButton();
    auto *clientRadioButton = new QRadioButton();

    QString hostString = "";
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             hostString = address.toString();
    }

    serverRadioButton->setText("服务端");
    clientRadioButton->setText("客户端");
    serverRadioButton->setChecked(true);

    connect(serverRadioButton, &QRadioButton::clicked, this, &ChessGameConfigurationView::didSetServer);
    connect(clientRadioButton, &QRadioButton::clicked, this, &ChessGameConfigurationView::didSetClient);

    addressTextField = new QLineEdit();
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex("^" + ipRange
                            + "(\\." + ipRange + ")"
                            + "(\\." + ipRange + ")"
                            + "(\\." + ipRange + ")$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    addressTextField->setValidator(ipValidator);

    auto *clientHLayout = new QHBoxLayout();
    clientHLayout->addWidget(clientRadioButton);
    clientHLayout->addWidget(addressTextField);

    auto *serverHLayout = new QHBoxLayout();
    serverHLayout->addWidget(serverRadioButton);
    serverHLayout->addWidget(new QLabel(hostString));
    serverHLayout->addStretch();

    auto *topVBox = new QVBoxLayout(topGroupBox);
    topVBox->addLayout(serverHLayout);
    topVBox->addLayout(clientHLayout);

    confirmationButton = new QPushButton("确认");
    topVBox->addWidget(confirmationButton);
    connect(confirmationButton, &QPushButton::clicked, this, &ChessGameConfigurationView::confirm);

}

void ChessGameConfigurationView::didSetClient() {
    addressTextField->setEnabled(true);
}

void ChessGameConfigurationView::didSetServer() {
    addressTextField->setEnabled(false);
}

void ChessGameConfigurationView::confirm() {
    setEnabled(false);
    manager = new ChessGameManager(__isServer);

    if (!__isServer) {
        auto *view = new ChessGameView(manager);
        view->show();
        close();
    } else {
        connect(manager, &ChessGameManager::didConnectToHost,
                this, &ChessGameConfigurationView::serverDidConnectToHost);
    }
}

void ChessGameConfigurationView::serverDidConnectToHost() {
    auto *view = new ChessGameView(manager);
    view->show();
    close();
}
