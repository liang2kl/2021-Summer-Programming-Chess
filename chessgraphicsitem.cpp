#include "chessgraphicsitem.h"
#include "constants.h"
#include <QFont>
#include <QBrush>

ChessGraphicsItem::ChessGraphicsItem(const Chess *chess, const QSizeF &size, bool isBack)
    : _chess(chess), isBack(isBack) {
    auto font = QFont();
    font.setPointSizeF(size.height() * FONT_SIZE_RATIO);

    auto *frontRectItem = new QGraphicsRectItem(QRectF(QPointF(), size));
    frontRectItem->setBrush(chess->side() == Chess::Side::Blue ? Constant::blue : Constant::red);
    auto *frontTextItem = new QGraphicsTextItem(chess->name());
    frontTextItem->setFont(font);

    auto *frontItemGroup = new QGraphicsItemGroup();
    frontItemGroup->addToGroup(frontRectItem);
    frontItemGroup->addToGroup(frontTextItem);
    frontItem = frontItemGroup;

    auto *backRectItem = new QGraphicsRectItem(QRectF(QPointF(), size));
    backRectItem->setBrush(Qt::gray);
    auto *backTextItem = new QGraphicsTextItem("?");

    backTextItem->setFont(font);

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

void ChessGraphicsItem::animatedSetPos(const QPointF &pos, int duration) {

    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(duration);
    animation->setStartValue(this->pos());
    animation->setEndValue(pos);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
