#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "chessboardscene.h"
#include "chessgamemanager.h"

class ChessboardView : public QGraphicsView
{
    Q_OBJECT

private:
    ChessboardScene * chessboardScene;
    ChessGameManager *manager;

private slots:
    void sceneDidFinishRender();
    void resizeEvent(QResizeEvent *);

signals:
    void didResize(QResizeEvent *);

public:
    ChessboardView(ChessGameManager *manager, QWidget *parent = nullptr);
};

#endif // CHESSBOARDVIEW_H
