#pragma once

#include "Annotation.h"
#include "Image.h"

#include <QQuickPaintedItem>
#include <QPen>

struct AnnotationState : public QObject
{
    Q_OBJECT
public:
    enum class State
    {
        NONE,
        DRAWING,
        TRANSLATION_STARTED,
        TRANSLATION_ENDED,
        SELECTED,
        ADDING_TEXT
    };

    Q_ENUM(State)
};

class AnnotationHandler : public QQuickPaintedItem, public pattern::Observer<Annotation, AnnotationEvent::EventType>
{
    Q_OBJECT
public:

    AnnotationHandler();

    void paint(QPainter *painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    Q_INVOKABLE void setPenColor(const QString& penColor);
    Q_INVOKABLE void updateStatus(AnnotationState::State newState);

private:
    void setupConnection();
    void startAnnotationTranslation(QMouseEvent *event);
    void endAnnotationTranslation();
    void updateCursorShape();
    void handleAnnotationStateChanged(AnnotationState::State &&state);
    void handleAnnotationSelection(QMouseEvent* event);
    void setupToolTip(const QPointF& position);

    void handleActiveImageChanged(std::shared_ptr<::Image> image);
    void handleAnnotationAdded(std::shared_ptr<Annotation> annotation);
    void handleAnnotationRemoved(std::shared_ptr<Annotation> annotation);

    void changed(Annotation* type, const AnnotationEvent::EventType& evenType) override;

    QString m_pen_color;
    QPointF m_initial_position;
    QPainterPath m_original_path;
    QVector<QPointF> m_current_points;
    AnnotationState::State m_current_state;
    std::vector<std::shared_ptr<Annotation>> m_annotations;
};
