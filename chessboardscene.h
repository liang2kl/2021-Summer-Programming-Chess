#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QResizeEvent>
#include <QGraphicsSceneMouseEvent>

class ChessboardScene : public QGraphicsScene {
    Q_OBJECT

private:
    const QVector<QPoint> CIRCLE_COORDINATES = {
        QPoint(2, 1), QPoint(2, 3),
        QPoint(3, 2), QPoint(4, 1),
        QPoint(4, 3), QPoint(7, 1),
        QPoint(7, 3), QPoint(8, 2),
        QPoint(9, 1), QPoint(9, 3)
    };

    // Drawing
    void drawScene();
    void drawRoads(const QVector<QPoint>& cellCenters);
    void drawRects(const QVector<QPoint>& cellCenters);
    void drawCircles(const QVector<QPoint>& cellCenters);
    void drawBoardBackground(const QVector<QPoint> &cellCenters);

    void _drawRoad(const QLine &line);
    void _drawRailway(const QLine &line);

    // Geometry
    const float CELL_WIDTH_RATIO = 2;
    const float HORIZONTAL_SPACING_RATIO = 0.8;
    const float VERTICAL_SPACING_RATIO = 0.5;
    const float CENTER_VERTICAL_SPACING_RATIO = 3;
    const float CIRCLE_RADIUS_RATIO = 0.65;

    int _cellHeight;
    void setCellHeight(int height) { _cellHeight = height; }
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
