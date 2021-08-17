#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "chessboardscene.h"

class ChessboardView : public QGraphicsView
{
    Q_OBJECT

private:
    ChessboardScene * chessboardScene;

private slots:
    void sceneDidFinishRender();
    void resizeEvent(QResizeEvent *);

signals:
    void didResize(QResizeEvent *);

public:
    ChessboardView();
};

#endif // CHESSBOARDVIEW_H
