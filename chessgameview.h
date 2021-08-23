#ifndef CHESSGAMEVIEW_H
#define CHESSGAMEVIEW_H

#include <QWidget>
#include "chessboardview.h"
#include "chessgamepanelview.h"
#include "chessgamemanager.h"

class ChessGameView : public QWidget {
    Q_OBJECT

private:
    ChessboardView *chessboardView;
    ChessGamePanelView *panelView;
    ChessGameManager *manager;

    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
public:
    ChessGameView(ChessGameManager *manager, QWidget *parent = nullptr);

signals:

};

#endif // CHESSGAMEVIEW_H
