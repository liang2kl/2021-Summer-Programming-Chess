#ifndef CHESSGAMEVIEW_H
#define CHESSGAMEVIEW_H

#include <QWidget>
#include "chessboardview.h"
#include "chessgamepanelview.h"

class ChessGameView : public QWidget {
    Q_OBJECT

private:
    ChessboardView *chessboardView;
    ChessGamePanelView *panelView;

    void resizeEvent(QResizeEvent *);
public:
    explicit ChessGameView(QWidget *parent = nullptr);

signals:

};

#endif // CHESSGAMEVIEW_H
