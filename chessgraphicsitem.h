#ifndef CHESSGRAPHICSITEM_H
#define CHESSGRAPHICSITEM_H

#include <QGraphicsItemGroup>
#include "chess.h"

class ChessGraphicsItem : public QGraphicsItemGroup {
private:
    bool isBack = true;
    QGraphicsItemGroup *frontItem;
    QGraphicsItemGroup *backItem;
public:
    ChessGraphicsItem(const Chess *chess, const QSizeF &size, bool isBack = true);
    void toggleSide();
};

#endif // CHESSGRAPHICSITEM_H
