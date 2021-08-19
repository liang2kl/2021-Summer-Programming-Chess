#include "chessgraphicsitem.h"
#include "constants.h"
#include <QFont>
#include <QBrush>

ChessGraphicsItem::ChessGraphicsItem(const Chess *chess, const QSizeF &size, bool isBack)
    : _chess(chess), isBack(isBack) {
    auto *frontRectItem = new QGraphicsRectItem(QRectF(QPointF(), size));
    frontRectItem->setBrush(chess->side() == Chess::Side::Blue ? Constant::blue : Constant::red);
    auto *frontTextItem = new QGraphicsTextItem(chess->name());

    auto *frontItemGroup = new QGraphicsItemGroup();
    frontItemGroup->addToGroup(frontRectItem);
    frontItemGroup->addToGroup(frontTextItem);
    frontItem = frontItemGroup;

    auto *backRectItem = new QGraphicsRectItem(QRectF(QPointF(), size));
    backRectItem->setBrush(Qt::gray);
    auto *backTextItem = new QGraphicsTextItem("?");

    auto *backItemGroup = new QGraphicsItemGroup();
    backItemGroup->addToGroup(backRectItem);
    backItemGroup->addToGroup(backTextItem);
    backItem = backItemGroup;

    addToGroup(backItemGroup);
    addToGroup(frontItemGroup);

    if (isBack) {
        frontItemGroup->setVisible(false);
    } else {
        backItemGroup->setVisible(false);
    }
}

void ChessGraphicsItem::toggleSide() {
    isBack = !isBack;
    if (isBack) {
        backItem->setVisible(true);
        frontItem->setVisible(false);
    } else {
        backItem->setVisible(false);
        frontItem->setVisible(true);
    }
}
