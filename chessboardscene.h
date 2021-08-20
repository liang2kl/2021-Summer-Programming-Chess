#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QResizeEvent>
#include <QGraphicsSceneMouseEvent>

#include "chessgamemanager.h"
#include "chessgraphicsitem.h"

class ChessboardScene : public QGraphicsScene {
    Q_OBJECT

private:
    ChessGameManager *manager;
    QVector<QGraphicsItem *> containerItems = QVector<QGraphicsItem *>(60);
    QVector<ChessGraphicsItem *> chessItems = QVector<ChessGraphicsItem *>(60);
    QVector<QGraphicsItem *> highlightItems;
    QGraphicsItem *selectedItem = nullptr;
private:
    // Initial drawing
    void drawScene();
    void drawLines(const QVector<QPoint>& cellCenters);
    void drawRects(const QVector<QPoint>& cellCenters);
    void drawCircles(const QVector<QPoint>& cellCenters);
    void drawBoardBackground(const QVector<QPoint> &cellCenters);

    void _drawRoad(const QLine &line);
    void _drawRailway(const QLine &line);

    // In-game drawing
    void drawHighlightItems();
    void drawSelectionIndicator();
    void drawChesses(const QVector<QPoint>& cellCenters);

    // Geometry
    const float CELL_WIDTH_RATIO = 2;
    const float HORIZONTAL_SPACING_RATIO = 0.8;
    const float VERTICAL_SPACING_RATIO = 0.5;
    const float CENTER_VERTICAL_SPACING_RATIO = 3;
    const float CIRCLE_RADIUS_RATIO = 0.65;

    QVector<QPoint> cellCenters;
    int __cellHeight;

    int cellHeight() { return __cellHeight; }
    int cellWidth() { return __cellHeight * CELL_WIDTH_RATIO; }
    int horizontalSpacing() { return __cellHeight * HORIZONTAL_SPACING_RATIO; }
    int verticalSpacing() { return __cellHeight * VERTICAL_SPACING_RATIO; }
    int centerVerticalSpacing() { return __cellHeight * CENTER_VERTICAL_SPACING_RATIO; }
    int circleRadius() { return __cellHeight * CIRCLE_RADIUS_RATIO; }

    QVector<QPoint> generateCellData();
    void setNewSize(const QSize &size);

    // Touch event
    void mousePressEvent(QGraphicsSceneMouseEvent *);

public slots:
    void resizeEvent(QResizeEvent * event);

    // Respond to model signals.
    void chessGameDidChangeState(ChessGame::State state);
    void chessGameDidFlipChess(const ChessPoint &pos);
    void chessGameDidMoveChess(const ChessPoint &source, const ChessPoint &dest);
    void chessGameDidRemoveChess(const ChessPoint &pos);

signals:
    void didFinishRender();

// State
private:
    // TODO: Update for multiusers.
    int __selectedIndex = -1;
    QVector<int> __destPoints;
    void setSelectedIndex(int i);
    void setDestPoints(const QVector<int>);
    bool isMovable();
    Chess::Side movingSide();

public:
    ChessboardScene(ChessGameManager *manager, int initialHeight = 600);
};

#endif // CHESSBOARDSCENE_H
