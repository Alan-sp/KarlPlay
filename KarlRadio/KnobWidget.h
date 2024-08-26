#ifndef KNOBWIDGET_H
#define KNOBWIDGET_H

#include <QWidget>

class KnobWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KnobWidget(QWidget *parent = nullptr);

    double value() const { return m_value; }
    void setValue(double value);

signals:
    void valueChanged(double newValue);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    double m_value;
    double m_minValue;
    double m_maxValue;
    QPoint m_lastPos;

    void updateValueBasedOnMouse(QMouseEvent *event);
};

#endif // KNOBWIDGET_H
