#pragma once
#include "Annotation.h"

#include <QQuickPaintedItem>
#include <QPen>

class AnnotationHandler : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool can_draw WRITE setCanDraw CONSTANT)
public:
    AnnotationHandler();

    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setCanDraw(bool canDraw);
    void setupConnection();

    bool m_can_draw;
    QVector<QPointF> m_current_points;
    std::vector<std::shared_ptr<Annotation>> m_annotations;
};
