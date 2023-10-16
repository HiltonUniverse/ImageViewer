#pragma once
#include "Annotation.h"

#include <QQuickPaintedItem>
#include <QPen>

class AnnotationHandler : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool can_draw WRITE setCanDraw CONSTANT)
    Q_PROPERTY(QString pen_color WRITE setPenColor CONSTANT)
public:
    AnnotationHandler();

    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setPenColor(const QString& penColor);
    void setCanDraw(bool canDraw);
    void setupConnection();

    bool m_can_draw;
    QString m_pen_color;
    QVector<QPointF> m_current_points;
    std::vector<std::shared_ptr<Annotation>> m_annotations;
};
