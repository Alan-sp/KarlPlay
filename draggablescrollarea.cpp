#include "draggablescrollarea.h"
#include <QScrollBar>

DraggableScrollArea::DraggableScrollArea(QWidget *parent)
    : QScrollArea(parent), dragging(false), overscrollX(0), overscrollY(0) {
    setMouseTracking(true);
}

void DraggableScrollArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastPos = event->pos();
        setCursor(Qt::ClosedHandCursor);  // 改变光标样式以指示可拖动
    }
    QScrollArea::mousePressEvent(event);
}

void DraggableScrollArea::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        int dx = event->pos().x() - lastPos.x();
        int dy = event->pos().y() - lastPos.y();

        // 水平滚动条
        int newX = horizontalScrollBar()->value() - dx;
        if (newX < horizontalScrollBar()->minimum()) {
            overscrollX = newX - horizontalScrollBar()->minimum();
            newX = horizontalScrollBar()->minimum() - overscrollX / 2;  // 模拟超出范围的拖动
        } else if (newX > horizontalScrollBar()->maximum()) {
            overscrollX = newX - horizontalScrollBar()->maximum();
            newX = horizontalScrollBar()->maximum() + overscrollX / 2;  // 模拟超出范围的拖动
        } else {
            overscrollX = 0;
        }
        horizontalScrollBar()->setValue(newX);

        // 垂直滚动条
        int newY = verticalScrollBar()->value() - dy;
        if (newY < verticalScrollBar()->minimum()) {
            overscrollY = newY - verticalScrollBar()->minimum();
            newY = verticalScrollBar()->minimum() - overscrollY / 2;  // 模拟超出范围的拖动
        } else if (newY > verticalScrollBar()->maximum()) {
            overscrollY = newY - verticalScrollBar()->maximum();
            newY = verticalScrollBar()->maximum() + overscrollY / 2;  // 模拟超出范围的拖动
        } else {
            overscrollY = 0;
        }
        verticalScrollBar()->setValue(newY);

        lastPos = event->pos();
    }
    QScrollArea::mouseMoveEvent(event);
}

void DraggableScrollArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        setCursor(Qt::ArrowCursor);  // 恢复光标样式

        // 水平回弹
        if (overscrollX != 0) {
            applyBounceBack(horizontalScrollBar(), overscrollX);
        }

        // 垂直回弹
        if (overscrollY != 0) {
            applyBounceBack(verticalScrollBar(), overscrollY);
        }
    }
    QScrollArea::mouseReleaseEvent(event);
}

void DraggableScrollArea::applyBounceBack(QScrollBar *scrollBar, int overshoot) {
    QPropertyAnimation *animation = new QPropertyAnimation(scrollBar, "value");
    animation->setDuration(300);
    animation->setStartValue(scrollBar->value());
    int endValue;
    if (overshoot < 0) {
        endValue = scrollBar->minimum();
    } else {
        endValue = scrollBar->maximum();
    }
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);
    animation->start();
}
