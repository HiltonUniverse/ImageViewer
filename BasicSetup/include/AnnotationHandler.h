#pragma once

#include "Annotation.h"
#include "Image.h"

#include <QQuickPaintedItem>
#include <QPen>

class AnnotationHandler : public QQuickPaintedItem, public pattern::Observer<Annotation, AnnotationEvent::EventType>
{
    Q_OBJECT
    Q_PROPERTY(bool can_draw WRITE setCanDraw CONSTANT)
    Q_PROPERTY(QString pen_color WRITE setPenColor CONSTANT)
public:
    enum class AnnotationState
    {
        NONE,
        DRAWING,
        TRANSLATION_STARTED,
        TRANSLATION_ENDED,
        SELECTED
    };

    AnnotationHandler();

    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setPenColor(const QString& penColor);
    void setCanDraw(bool canDraw);
    void setupConnection();
    void startAnnotationTranslation(QMouseEvent *event);
    void endAnnotationTranslation();
    void updateCursorShape();
    void handleAnnotationStateChanged(AnnotationState &&state);
    void handleAnnotationSelection(QMouseEvent* event);
    void setupToolTip(const QPointF& position);

    void handleActiveImageChanged(std::shared_ptr<::Image> image);
    void handleAnnotationAdded(std::shared_ptr<Annotation> annotation);
    void handleAnnotationRemoved(std::shared_ptr<Annotation> annotation);

    void changed(Annotation* type, const AnnotationEvent::EventType& evenType) override;

    bool m_can_draw;
    QString m_pen_color;
    QPointF m_initial_position;
    QVector<QPointF> m_current_points;
    AnnotationState m_current_state;
    std::vector<std::shared_ptr<Annotation>> m_annotations;
};
