#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>

class ChessboardScene : public QGraphicsScene {
private:
    const int CELL_HEIGHT = 40;
    const int CELL_WIDTH = 2 * CELL_HEIGHT;
    const int HORIZONTAL_SPACING = CELL_WIDTH * 0.15;
    const int VERTICAL_SPACING = CELL_HEIGHT * 0.2;
    const int CENTER_VERTICAL_SPACING = 3 * CELL_HEIGHT;
    const QVector<QPoint> CIRCLE_COORDINATES = {
        QPoint(2, 1), QPoint(2, 3),
        QPoint(3, 2), QPoint(4, 1),
        QPoint(4, 3), QPoint(7, 1),
        QPoint(7, 3), QPoint(8, 2),
        QPoint(9, 1), QPoint(9, 3)
    };
    void drawScene();
    void drawRailway(const QVector<QPoint>& cellOrigins);
    void drawRects(const QVector<QPoint>& cellOrigins);
    void drawCircles(const QVector<QPoint>& cellOrigins);
    QVector<QPoint> generateCellData();
public:
    explicit ChessboardScene(QObject *parent = nullptr);
};

#endif // CHESSBOARDSCENE_H
