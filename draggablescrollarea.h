#ifndef DRAGGABLESCROLLAREA_H
#define DRAGGABLESCROLLAREA_H

#include <QScrollArea>
#include <QMouseEvent>
#include <QPropertyAnimation>

class DraggableScrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit DraggableScrollArea(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void applyBounceBack(QScrollBar *scrollBar, int overshoot);

    bool dragging;
    QPoint lastPos;
    int overscrollX;
    int overscrollY;
    int originalScrollX;
    int originalScrollY;
};

#endif // DRAGGABLESCROLLAREA_H
