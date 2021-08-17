#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QResizeEvent>

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
    void drawRoads(const QVector<QPoint>& cellOrigins);
    void drawRects(const QVector<QPoint>& cellOrigins);
    void drawCircles(const QVector<QPoint>& cellOrigins);
    void drawBackgroundImage();

    void _drawRoad(const QLine &line);
    void _drawRailway(const QLine &line);

    // Geometry
    const float CELL_WIDTH_RATIO = 2;
    const float HORIZONTAL_SPACING_RATIO = 0.8;
    const float VERTICAL_SPACING_RATIO = 0.5;
    const float CENTER_VERTICAL_SPACING_RATIO = 3;
    int _cellHeight = 50;
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
    QPoint centerPointFromOrigin(const QPoint &origin);

public slots:
    void resizeEvent(QResizeEvent * event);

public:
    explicit ChessboardScene(QObject *parent = nullptr);
};

#endif // CHESSBOARDSCENE_H
