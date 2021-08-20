#include "chessboardscene.h"
#include "dropshadoweffect.h"
#include "constants.h"
#include <QImage>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

ChessboardScene::ChessboardScene(int initialHeight) {
    auto *game = ChessGame::shared;
    connect(game, &ChessGame::chessDidFlip, this, &ChessboardScene::chessGameDidFlipChess);
    connect(game, &ChessGame::chessDidMove, this, &ChessboardScene::chessGameDidMoveChess);
    connect(game, &ChessGame::chessDidRemoved, this, &ChessboardScene::chessGameDidRemoveChess);
    connect(game, &ChessGame::stateDidChange, this, &ChessboardScene::chessGameDidChangeState);
    setNewSize(QSize(0, initialHeight));
    drawScene();
}

// Drawing. Do not initiate drawings outside this section.
void ChessboardScene::drawScene() {
    cellCenters = generateCellData();

    drawBoardBackground(cellCenters);
    drawLines(cellCenters);
    drawRects(cellCenters);
    drawCircles(cellCenters);
    drawChesses(cellCenters);
    drawHighlightItems();
    drawSelectionIndicator();

    emit didFinishRender();
}

void ChessboardScene::drawChesses(const QVector<QPoint>& cellCenters) {
    const QVector<const Chess*> chesses = ChessGame::shared->chesses();

    for (const auto *chess : chesses) {
        if (chess) {
            auto *item = new ChessGraphicsItem(chess, QSizeF(cellWidth(), cellHeight()), !chess->isFlipped());
            const int i = chess->position();
            item->setPos(cellCenters[i].x() - cellWidth() / 2,
                         cellCenters[i].y() - cellHeight() / 2);

            addItem(item);
            chessItems[i] = item;
        }
    }
}

void ChessboardScene::drawRects(const QVector<QPoint>& cellCenters) {
    const int size = cellCenters.size();
    for (int i = 0; i < size; i++) {
        bool skip = false;
        for (const QPoint& coordinate : ChessPoint::CAMP_POINTS) {
            if (coordinate.x() * 5 + coordinate.y() == i) {
                skip = true;
                break;
            }
        }
        if (skip) { continue; }

        QGraphicsRectItem *item = this->addRect(
                    QRectF(cellCenters[i].x() - cellWidth() * 0.5,
                           cellCenters[i].y() - cellHeight() * 0.5,
                           cellWidth(),
                           cellHeight()),
                    QPen(Constant::cellStrokeColor, cellHeight() / 8),
                    Constant::cellFillColor);

        item->setGraphicsEffect(new DropShadowEffect());
        containerItems[i] = item;
    }
}

void ChessboardScene::drawCircles(const QVector<QPoint> &cellCenters) {
    const float radius = circleRadius();

    for (const QPoint& coordinate : ChessPoint::CAMP_POINTS) {
        const int index = coordinate.x() * 5 + coordinate.y();
        const QPoint center = cellCenters[index];

        auto *item = this->addEllipse(
                    center.x() - radius,
                    center.y() - radius,
                    radius * 2,
                    radius * 2,
                    QPen(Constant::circleStrokeColor, cellHeight() / 8),
                    Constant::cellFillColor);

        item->setGraphicsEffect(new DropShadowEffect());
        containerItems[index] = item;
    }
}

void ChessboardScene::drawBoardBackground(const QVector<QPoint> &cellCenters) {
    const int width = 5 * cellWidth() + 6 * horizontalSpacing();
    const int height = 12 * cellHeight() + 13 * verticalSpacing() + centerVerticalSpacing();

    this->addRect(0, 0, width, height, Constant::backgroundColor, Constant::backgroundColor);

    this->addRect(QRect(cellCenters[0], cellCenters[29]), QPen(), Constant::cellFillColor);
    this->addRect(QRect(cellCenters[30], cellCenters[59]), QPen(), Constant::cellFillColor);
}

void ChessboardScene::drawLines(const QVector<QPoint> &cellCenters) {

    // Horizontal
    for (int i = 0; i < 12; i++) {
        const int base = i * 5;
        QLine line(cellCenters[base],
                   cellCenters[base + 4]);

        if (i == 1 || i == 5 || i == 6 || i == 10) {
            _drawRailway(line);
        } else {
            _drawRoad(line);
        }

    }

    // Vertical
    for (int i = 0; i < 5; i++) {
        QLine line1(cellCenters[i], cellCenters[25 + i]);
        _drawRoad(line1);

        QLine line2(cellCenters[30 + i], cellCenters[55 + i]);
        _drawRoad(line2);

        if (i == 0 || i == 4) {
            QLine line(cellCenters[5 + i], cellCenters[50 + i]);
            _drawRailway(line);
        } else if (i == 2) {
            QLine line(cellCenters[27], cellCenters[32]);
            _drawRailway(line);
        }
    }

    // Inclined
    for (const auto &coordinate : ChessPoint::CAMP_POINTS) {
        if (coordinate.y() == 2) { continue; }
        const int index1 = (coordinate.x() - 1) * 5 + coordinate.y() - 1;
        const int index2 = (coordinate.x() + 1) * 5 + coordinate.y() + 1;
        const int index3 = (coordinate.x() - 1) * 5 + coordinate.y() + 1;
        const int index4 = (coordinate.x() + 1) * 5 + coordinate.y() - 1;

        QLine line1(cellCenters[index1], cellCenters[index2]);
        _drawRoad(line1);

        QLine line2(cellCenters[index3], cellCenters[index4]);
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

void ChessboardScene::drawHighlightItems() {
    for (auto point : __destPoints) {
        QPoint center = cellCenters[point];

        QGraphicsItem *newItem;
        if (ChessPoint(point).isCamp()) {
            const float radius = circleRadius();

            newItem = addEllipse(
                        center.x() - radius,
                        center.y() - radius,
                        radius * 2,
                        radius * 2,
                        QPen(Constant::yellow, cellHeight() / 10),
                        Qt::NoBrush);
        } else {
            newItem = addRect(
                        center.x() - cellWidth() / 2,
                        center.y() - cellHeight() / 2,
                        cellWidth(),
                        cellHeight(),
                        QPen(Constant::yellow, cellHeight() / 10),
                        Qt::NoBrush);
        }

        highlightItems.append(newItem);
    }
}

void ChessboardScene::drawSelectionIndicator() {
    if (__selectedIndex != -1) {
        QPoint center = cellCenters[__selectedIndex];

        auto color = Constant::yellow;
        color.setAlphaF(0.7);

        auto *item = addRect(
                    center.x() - cellWidth() / 2,
                    center.y() - cellHeight() / 2,
                    cellWidth(),
                    cellHeight(),
                    Qt::NoPen,
                    color);

        selectedItem = item;
    }
}
// END DRAWING SECTION

QVector<QPoint> ChessboardScene::generateCellData() {
    QVector<QPoint> points;
    for (int row = 0; row < 12; row++) {
        for (int column = 0; column < 5; column++) {
            float x = horizontalSpacing() * (column + 1) + cellWidth() * (column + 0.5);
            float y = verticalSpacing() * (row + 1) + cellHeight() * (row + 0.5);
            if (row > 5) {
                y += centerVerticalSpacing();
            }
            points.append(QPoint(x, y));
        }
    }

    return points;
}

void ChessboardScene::resizeEvent(QResizeEvent *event) {
    if (abs(event->size().height() - event->oldSize().height()) < 2) { return; }
    setNewSize(event->size());

    this->clear();
    chessItems = QVector<ChessGraphicsItem *>(60);
    containerItems = QVector<QGraphicsItem *>(60);
    highlightItems = {};

    drawScene();
}

void ChessboardScene::setNewSize(const QSize &size) {
    float ratio = 12 + 12 * VERTICAL_SPACING_RATIO + CENTER_VERTICAL_SPACING_RATIO;
    __cellHeight = size.height() / ratio * 0.97;
}

// No UI modification here! Just interacting with the model.
void ChessboardScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    for (int i = 0; i < containerItems.size(); i++) {
        auto *item = containerItems[i];

        if (item->contains(event->scenePos())) {
            auto *chessItem = chessItems[i];

            if (chessItem) {
                if (!chessItem->chess()->isFlipped() && __selectedIndex == -1) {
                    ChessGame::shared->flipChess(i);
                    setSelectedIndex(-1);
                } else if (!isMovable()) {
                    break;
                } else if (__selectedIndex == -1 &&
                           movingSide() == chessItem->chess()->side() &&
                           chessItem->chess()->isMovable()) {
                    setSelectedIndex(i);
                } else if (__selectedIndex == i) {
                    setSelectedIndex(-1);
                } else if (__destPoints.contains(i)) {
                    ChessGame::shared->moveChess(__selectedIndex, i);
                    setSelectedIndex(-1);
                } // Else, do nothing.
            } else {
                if (__destPoints.contains(i)) {
                    ChessGame::shared->moveChess(__selectedIndex, i);
                    setSelectedIndex(-1);
                } // Else, do nothing.
            }
            break;
        }

    }

}

// UI updates whose states are owned by the view.
void ChessboardScene::setSelectedIndex(int i) {
    qDebug() << "Selected Item:" << i << ChessPoint(i);
    __selectedIndex = i;

    if (selectedItem) {
        if (selectedItem->scene() == this) {
            removeItem(selectedItem);
        }
    }

    if (i == -1) {
        setDestPoints({});
        return;
    }

    drawSelectionIndicator();

    auto chesses = ChessGame::shared->chesses();
    auto points = ChessGame::shared->availablePointsFor(chesses[i]);
    setDestPoints(points);
}

void ChessboardScene::setDestPoints(const QVector<int> points) {
    // Clear previous items.
    for (auto *item : highlightItems) {
        removeItem(item);
        delete item;
    }
    highlightItems = {};

    __destPoints = points;

    drawHighlightItems();
}

// Respond to signals from the game model. The only place where
// UI could be modified.
void ChessboardScene::chessGameDidFlipChess(const ChessPoint &pos) {
    qDebug() << "Flipped Chess" << pos;
    chessItems[pos]->toggleSide();
}

void ChessboardScene::chessGameDidMoveChess(const ChessPoint &source, const ChessPoint &dest) {
    qDebug() << "Move Chess from" << source << "to" << dest;


    chessItems[source]->animatedSetPos(
                QPointF(cellCenters[dest].x() - cellWidth() / 2,
                cellCenters[dest].y() - cellHeight() / 2)
                );

    auto *item = chessItems[source];
    chessItems[source] = chessItems[dest];
    chessItems[dest] = item;
}

void ChessboardScene::chessGameDidRemoveChess(const ChessPoint &pos) {
    removeItem(chessItems[pos]);
    delete chessItems[pos];
    chessItems[pos] = nullptr;
}

void ChessboardScene::chessGameDidChangeState(ChessGame::State state) {
    qDebug() << "Change state to" << state;
}

// State
bool ChessboardScene::isMovable() {
    auto state = ChessGame::shared->state();
    return state == ChessGame::State::RedMove ||
               state == ChessGame::State::BlueMove;
}

Chess::Side ChessboardScene::movingSide() {
    assert(isMovable());

    return ChessGame::shared->state() == ChessGame::State::RedMove ?
                Chess::Side::Red : Chess::Side::Blue;
}
