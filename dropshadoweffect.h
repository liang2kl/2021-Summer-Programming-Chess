#ifndef DROPSHADOWEFFECT_H
#define DROPSHADOWEFFECT_H

#include <QGraphicsDropShadowEffect>

class DropShadowEffect : public QGraphicsDropShadowEffect
{
public:
    DropShadowEffect(qreal dx = 3, qreal dy = 3, const QColor &color = 0x212527, float alpha = 0.5) {
        setOffset(dx, dy);
        QColor _color = color;
        _color.setAlphaF(alpha);
        setColor(_color);
    }
};

#endif // DROPSHADOWEFFECT_H
