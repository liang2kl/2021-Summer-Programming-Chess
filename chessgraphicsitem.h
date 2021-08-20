#ifndef CHESSGRAPHICSITEM_H
#define CHESSGRAPHICSITEM_H

#include <QGraphicsItemGroup>
#include "chess.h"

class ChessGraphicsItem : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
private:
    const Chess *_chess;
    bool isBack = true;
    QGraphicsItemGroup *frontItem;
    QGraphicsItemGroup *backItem;

    const float FONT_SIZE_RATIO = 0.4;

public:
    ChessGraphicsItem(const Chess *chess, const QSizeF &size, bool isBack = true);
    void toggleSide();
    const Chess *chess() { return _chess; }
    void animatedSetPos(const QPointF &pos, int duration = 250);
};

#endif // CHESSGRAPHICSITEM_H
