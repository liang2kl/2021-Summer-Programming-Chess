#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QResizeEvent>
#include <QGraphicsSceneMouseEvent>

#include "chessgame.h"

class ChessboardScene : public QGraphicsScene {
    Q_OBJECT

private:
    QVector<QGraphicsItem *> rectItems = QVector<QGraphicsItem *>(60);
    QVector<QGraphicsItem *> chessItems = QVector<QGraphicsItem *>(60);

private:
    // Drawing
    void drawScene();
    void drawLines(const QVector<QPoint>& cellCenters);
    void drawRects(const QVector<QPoint>& cellCenters);
    void drawCircles(const QVector<QPoint>& cellCenters);
    void drawBoardBackground(const QVector<QPoint> &cellCenters);

    void drawChesses(const QVector<QPoint>& cellCenters);

    void _drawRoad(const QLine &line);
    void _drawRailway(const QLine &line);

    // Geometry
    const float CELL_WIDTH_RATIO = 2;
    const float HORIZONTAL_SPACING_RATIO = 0.8;
    const float VERTICAL_SPACING_RATIO = 0.5;
    const float CENTER_VERTICAL_SPACING_RATIO = 3;
    const float CIRCLE_RADIUS_RATIO = 0.65;

    int _cellHeight;

    int cellHeight() {
        return _cellHeight;
    }
    int cellWidth() {
        return _cellHeight * CELL_WIDTH_RATIO;
    }
    int horizontalSpacing() {
        return _cellHeight * HORIZONTAL_SPACING_RATIO;
    }
    int verticalSpacing() {
        return _cellHeight * VERTICAL_SPACING_RATIO;
    }
    int centerVerticalSpacing() {
        return _cellHeight * CENTER_VERTICAL_SPACING_RATIO;
    }
    int circleRadius() {
        return _cellHeight * CIRCLE_RADIUS_RATIO;
    }

    QVector<QPoint> generateCellData();
    void setNewSize(const QSize &size);

    void mousePressEvent(QGraphicsSceneMouseEvent *);


public slots:
    void resizeEvent(QResizeEvent * event);

signals:
    void didFinishRender();

public:
    ChessboardScene(int initialHeight = 600);
};

#endif // CHESSBOARDSCENE_H
