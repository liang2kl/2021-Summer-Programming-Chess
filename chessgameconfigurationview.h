#ifndef CHESSGAMECONFIGURATIONVIEW_H
#define CHESSGAMECONFIGURATIONVIEW_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include "chessgamemanager.h"

class ChessGameConfigurationView : public QWidget
{
    Q_OBJECT
private:
    bool __isServer = true;
    QLineEdit *addressTextField;
    QPushButton *confirmationButton;
    ChessGameManager *manager;

public:
    explicit ChessGameConfigurationView(QWidget *parent = nullptr);

private slots:
    void didSetClient();
    void didSetServer();
    void confirm();
    void serverDidConnectToHost();
};

#endif // CHESSGAMECONFIGURATIONVIEW_H
