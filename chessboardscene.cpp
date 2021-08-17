#include "chessboardscene.h"
#include "dropshadoweffect.h"
#include <QImage>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

ChessboardScene::ChessboardScene(QObject *parent) : QGraphicsScene(parent)
{
    drawScene();
}

void ChessboardScene::drawScene() {
    QVector<QPoint> points = generateCellData();

    drawBoardBackground(points);
    drawRoads(points);
    drawRects(points);
    drawCircles(points);

    emit didFinishRender();
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

        QGraphicsRectItem *item = this->addRect(
                    QRectF(cellOrigins[i].x(),
                           cellOrigins[i].y(),
                           cellWidth(),
                           cellHeight()),
                    QPen(QColor(0x495f68), cellHeight() / 8),
                    QColor(0x212325));

        item->setGraphicsEffect(new DropShadowEffect());
    }
}

void ChessboardScene::drawCircles(const QVector<QPoint> &cellOrigins) {
    QPixmap image(":/resources/chessboard/camp-placeholder.png");
    image = image.scaled(cellWidth(), cellHeight(), Qt::KeepAspectRatio);

    const float radius = cellHeight() * CIRCLE_RADIUS_RATIO;

    for (const QPoint& coordinate : CIRCLE_COORDINATES) {
        const int index = coordinate.x() * 5 + coordinate.y();
        const QPoint center = centerPointFromOrigin(cellOrigins[index]);

        auto *item = this->addEllipse(
                    center.x() - radius,
                    center.y() - radius,
                    radius * 2,
                    radius * 2,
                    QPen(QColor(0x3a5136), cellHeight() / 8),
                    QColor(0x212325));

        item->setGraphicsEffect(new DropShadowEffect());
    }
}

void ChessboardScene::drawBoardBackground(const QVector<QPoint> &cellOrigins) {
    QPixmap image(":/resources/chessboard/background.jpg");
    const int width = 5 * cellWidth() + 6 * horizontalSpacing();
    const int height = 12 * cellHeight() + 13 * verticalSpacing() + centerVerticalSpacing();
    image = image.scaled(width, height);
    this->addRect(0, 0, width, height, QPen(), QColor(0x1b1b1b));
//    this->addPixmap(image);

    const QRectF rect1(centerPointFromOrigin(cellOrigins[0]), centerPointFromOrigin(cellOrigins[29]));
    const QRectF rect2(centerPointFromOrigin(cellOrigins[30]), centerPointFromOrigin(cellOrigins[59]));

    QColor color = 0x272b2f;
    color.setAlphaF(0.5);
    this->addRect(rect1, QPen(), color);
    this->addRect(rect2, QPen(), color);
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
    auto *item = this->addLine(line,
            QPen(QColor(0x303f46), cellHeight() / 6));

    item->setGraphicsEffect(new DropShadowEffect());
}

void ChessboardScene::_drawRailway(const QLine &line) {

    auto *item1 = this->addLine(line,
            QPen(QColor(0x111), cellHeight() / 2.8));

    item1->setGraphicsEffect(new DropShadowEffect());

    auto *item2 = this->addLine(line,
            QPen(QColor(0x9d8647), cellHeight() / 8, Qt::PenStyle::DotLine));

    item2->setGraphicsEffect(new DropShadowEffect());
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
    if (abs(event->size().height() - event->oldSize().height()) < 2) { return; }
    float ratio = 12 + 12 * VERTICAL_SPACING_RATIO + CENTER_VERTICAL_SPACING_RATIO;
    int cellHeight = event->size().height() / ratio * 0.97;
    setCellHeight(cellHeight);

    this->clear();
    drawScene();
}
