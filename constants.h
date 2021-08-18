#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGui>

namespace Constant {
    const QColor backgroundColor = QColor(0x1b1b1b);
    const QColor cellStrokeColor = QColor(0x3d505b);
    const QColor cellFillColor = QColor(0x212325);
    const QColor circleStrokeColor = QColor(0x3a5136);

    const QVector<QPoint> campCoordinates = {
        QPoint(2, 1), QPoint(2, 3),
        QPoint(3, 2), QPoint(4, 1),
        QPoint(4, 3), QPoint(7, 1),
        QPoint(7, 3), QPoint(8, 2),
        QPoint(9, 1), QPoint(9, 3)
    };
}

#endif // CONSTANTS_H
