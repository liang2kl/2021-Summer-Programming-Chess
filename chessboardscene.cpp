#include "chessboardscene.h"
#include <iostream>

ChessboardScene::ChessboardScene(QObject *parent) : QGraphicsScene(parent)
{
    drawScene();
}

void ChessboardScene::drawScene() {
    QVector<QPoint> points = generateCellData();
    drawRects(points);
}

void ChessboardScene::drawRailway(const QVector<QPoint>& cellOrigins) {

}

void ChessboardScene::drawRects(const QVector<QPoint>& cellOrigins) {
    const int size = cellOrigins.size();
    for (int i = 0; i < size; i++) {
        bool skip = false;
        for (const QPoint& coordinate : CIRCLE_COORDINATES) {
            if (coordinate.x() * 5 + coordinate.y() == i) {
                skip = true;
                break;
            }
        }
        if (skip) { continue; }

        QGraphicsRectItem* item = this->addRect(QRect(cellOrigins[i], QSize(CELL_WIDTH, CELL_HEIGHT)));
        item->setBrush(Qt::red);
    }
}

QVector<QPoint> ChessboardScene::generateCellData() {
    QVector<QPoint> points;
    for (int row = 0; row < 12; row++) {
        for (int column = 0; column < 5; column++) {
            float x = HORIZONTAL_SPACING * (column + 1) + CELL_WIDTH * column;
            float y = VERTICAL_SPACING * (row + 1) + CELL_HEIGHT * row;
            if (row > 5) {
                y += CENTER_VERTICAL_SPACING;
            }
            points.append(QPoint(x, y));
        }
    }

    return points;
}
