#ifndef CHESSGRAPHICSITEM_H
#define CHESSGRAPHICSITEM_H

#include <QGraphicsItemGroup>
#include "chess.h"

class ChessGraphicsItem : public QGraphicsItemGroup {
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
