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


public:
    ChessGraphicsItem(const Chess *chess, const QSizeF &size, bool isBack = true);
    void toggleSide();
    const Chess *chess() { return _chess; }
};

#endif // CHESSGRAPHICSITEM_H
