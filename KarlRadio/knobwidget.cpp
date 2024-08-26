#include "KnobWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>

KnobWidget::KnobWidget(QWidget *parent)
    : QWidget(parent), m_value(1000.0), m_minValue(880.0), m_maxValue(1080.0)
{
    setMinimumSize(100, 100);
}

void KnobWidget::setValue(double value)
{
    if (value >= m_minValue && value <= m_maxValue) {
        m_value = value;
//        qDebug()<<"valuechanged";
        emit valueChanged(m_value);
//        qDebug()<<"valuechanged1";
        update();
    }
}

void KnobWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
    painter.setWindow(-50, -50, 100, 100);

    // Draw the knob
    painter.setBrush(Qt::lightGray);
    painter.drawEllipse(-45, -45, 90, 90);

    // Draw the pointer
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGray);
    painter.save();
    painter.rotate((m_value - m_minValue) * 270.0 / (m_maxValue - m_minValue) - 135);
    painter.drawRect(-5, -40, 10, 30);
    painter.restore();
}

void KnobWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
//    qDebug()<<"mousepressed";
}

void KnobWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateValueBasedOnMouse(event);
//    qDebug()<<"mousemoved";
}

void KnobWidget::mouseReleaseEvent(QMouseEvent *event)
{
    updateValueBasedOnMouse(event);
}

void KnobWidget::updateValueBasedOnMouse(QMouseEvent *event)
{
    double deltaX = event->x() - m_lastPos.x();
    double deltaValue = (deltaX * (m_maxValue - m_minValue)) / width();

    qDebug()<<deltaValue;

    setValue(m_value + deltaValue);
    m_lastPos = event->pos();
}
