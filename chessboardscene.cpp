#include "chessboardscene.h"
#include <QImage>

ChessboardScene::ChessboardScene(QObject *parent) : QGraphicsScene(parent)
{
    drawScene();
}

void ChessboardScene::drawScene() {
    QVector<QPoint> points = generateCellData();

    drawBackgroundImage();
    drawRoads(points);
    drawRects(points);
    drawCircles(points);
}

void ChessboardScene::drawRects(const QVector<QPoint>& cellOrigins) {
    QPixmap image(":/resources/chessboard/station-placeholder.png");
    image = image.scaled(cellWidth(), cellHeight());

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

        QGraphicsPixmapItem *item = this->addPixmap(image);
        item->setOffset(cellOrigins[i]);
    }
}

void ChessboardScene::drawCircles(const QVector<QPoint> &cellOrigins) {
    QPixmap image(":/resources/chessboard/camp-placeholder.png");
    image = image.scaled(cellWidth(), cellHeight(), Qt::KeepAspectRatio);

    for (const QPoint& coordinate : CIRCLE_COORDINATES) {
        const int index = coordinate.x() * 5 + coordinate.y();
        QGraphicsPixmapItem *item = this->addPixmap(image);
        // FIXME: Assuming the resource aspect ratio as 1
        const int xOffset = (cellWidth() - cellHeight()) / 2;
        item->setOffset(cellOrigins[index].x() + xOffset, cellOrigins[index].y());
    }
}

void ChessboardScene::drawBackgroundImage() {
    QPixmap image(":/resources/chessboard/background.jpg");
    const int width = 5 * cellWidth() + 6 * horizontalSpacing();
    const int height = 12 * cellHeight() + 13 * verticalSpacing() + centerVerticalSpacing();
    image = image.scaled(width, height);
    this->addPixmap(image);
}

void ChessboardScene::drawRoads(const QVector<QPoint> &cellOrigins) {

    // Horizontal
    for (int i = 0; i < 12; i++) {
        const int base = i * 5;
        QLine line(centerPointFromOrigin(cellOrigins[base]),
                   centerPointFromOrigin(cellOrigins[base + 4]));

        if (i == 1 || i == 5 || i == 6 || i == 10) {
            _drawRailway(line);
        } else {
            _drawRoad(line);
        }

    }

    // Vertical
    for (int i = 0; i < 5; i++) {
        QLine line1(centerPointFromOrigin(cellOrigins[i]),
                   centerPointFromOrigin(cellOrigins[25 + i]));
        _drawRoad(line1);

        QLine line2(centerPointFromOrigin(cellOrigins[30 + i]),
                   centerPointFromOrigin(cellOrigins[55 + i]));
        _drawRoad(line2);

        if (i == 0 || i == 4) {
            QLine line(centerPointFromOrigin(cellOrigins[5 + i]),
                       centerPointFromOrigin(cellOrigins[50 + i]));
            _drawRailway(line);
        } else if (i == 2) {
            QLine line(centerPointFromOrigin(cellOrigins[27]),
                       centerPointFromOrigin(cellOrigins[32]));
            _drawRailway(line);
        }
    }

    // Inclined
    for (const auto &coordinate : CIRCLE_COORDINATES) {
        if (coordinate.y() == 2) { continue; }
        const int index1 = (coordinate.x() - 1) * 5 + coordinate.y() - 1;
        const int index2 = (coordinate.x() + 1) * 5 + coordinate.y() + 1;
        const int index3 = (coordinate.x() - 1) * 5 + coordinate.y() + 1;
        const int index4 = (coordinate.x() + 1) * 5 + coordinate.y() - 1;

        QLine line1(centerPointFromOrigin(cellOrigins[index1]),
                   centerPointFromOrigin(cellOrigins[index2]));
        _drawRoad(line1);

        QLine line2(centerPointFromOrigin(cellOrigins[index3]),
                   centerPointFromOrigin(cellOrigins[index4]));
        _drawRoad(line2);
    }
}

void ChessboardScene::_drawRoad(const QLine &line) {
    auto *item = this->addLine(
            line,
            QPen(QColor(0x495f68), cellHeight() / 6));
}

void ChessboardScene::_drawRailway(const QLine &line) {

    this->addLine(
            line,
            QPen(QColor(0x111), cellHeight() / 2.8));

    this->addLine(
            line,
            QPen(QColor(0x9d8647), cellHeight() / 8, Qt::PenStyle::DotLine));
}

QVector<QPoint> ChessboardScene::generateCellData() {
    QVector<QPoint> points;
    for (int row = 0; row < 12; row++) {
        for (int column = 0; column < 5; column++) {
            float x = horizontalSpacing() * (column + 1) + cellWidth() * column;
            float y = verticalSpacing() * (row + 1) + cellHeight() * row;
            if (row > 5) {
                y += centerVerticalSpacing();
            }
            points.append(QPoint(x, y));
        }
    }

    return points;
}

QPoint ChessboardScene::centerPointFromOrigin(const QPoint &origin) {
    return QPoint(origin.x() + cellWidth() / 2,
                  origin.y() + cellHeight() / 2);
}

void ChessboardScene::resizeEvent(QResizeEvent *event) {
    float ratio = 12 + 12 * VERTICAL_SPACING_RATIO + CENTER_VERTICAL_SPACING_RATIO;
    int cellHeight = event->size().height() / ratio;
    setCellHeight(cellHeight);

    // TODO: Update rather than redraw
    this->clear();
    drawScene();
}
