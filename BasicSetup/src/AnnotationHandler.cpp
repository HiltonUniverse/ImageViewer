#include "AnnotationHandler.h"
#include "AddAnnotationUndoRedo.h"
#include "RemoveAnnotationUndoRedo.h"
#include "AnnotationTranslationUndoRedo.h"
#include "DataManager.h"

#include <QCursor>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QPainterPath>

namespace
{
    //-----------------------------------
    auto& getDataManager()
    {
        return DataManager::instance();
    }

    //-----------------------------------
    bool containsClick(const QPainterPath & path, const QPointF & p, qreal width=6.0)
    {
        QPainterPathStroker stroker;
        stroker.setWidth(width);
        QPainterPath strokepath = stroker.createStroke(path);
        return strokepath.contains(p);
    }

    //-----------------------------------
    QPainterPath convertPointsToPainterPath(QVector<QPointF>& points)
    {
        QPainterPath path;
        //we first move our path to where the first point starts
        //otherwise it always starts at (0,0), drawing a line from (0,0) to the first point of points vector.
        path.moveTo(points.front());

        for(auto& p : points)
        {
            path.lineTo(p);
        }

        return path;
    }
}

//-----------------------------------
AnnotationHandler::AnnotationHandler()
    : m_current_state(AnnotationState::State::NONE)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons({Qt::LeftButton, Qt::RightButton});
    setFlags({ItemAcceptsInputMethod, ItemHasContents, ItemIsFocusScope});
    setupConnection();
}

//-----------------------------------
void AnnotationHandler::setupConnection()
{
    auto& data_manager = getDataManager();

    connect(&data_manager, &DataManagerImpl::activeImageChanged, this, &AnnotationHandler::handleActiveImageChanged);
    connect(&data_manager, &DataManagerImpl::annotationAdded, this, &AnnotationHandler::handleAnnotationAdded);
    connect(&data_manager, &DataManagerImpl::annotationRemoved, this, &AnnotationHandler::handleAnnotationRemoved);
}

//-----------------------------------
void AnnotationHandler::handleActiveImageChanged(std::shared_ptr<::Image> image)
{
    m_annotations.clear();

    for(auto& annotation : image->getAnnotations())
    {
        annotation->attach(this);
        m_annotations.push_back(annotation);
    }

    update();
}

//-----------------------------------
void AnnotationHandler::handleAnnotationAdded(std::shared_ptr<Annotation> annotation)
{
    annotation->attach(this);
    m_annotations.push_back(annotation);
    update();
}

//-----------------------------------
void AnnotationHandler::handleAnnotationRemoved(std::shared_ptr<Annotation> annotation)
{
    annotation->detach(this);
    m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), annotation), m_annotations.end());
    update();
}

//-----------------------------------
void AnnotationHandler::changed(Annotation *type, const AnnotationEvent::EventType &evenType)
{
    switch(evenType)
    {
        case AnnotationEvent::EventType::ANNOTATION_SELECTED:
        {
            if(type->isSelected())
            {
                setFocus(true);
            }
            update();
            return;
        }

        case AnnotationEvent::EventType::ANNOTATION_HOVERED:
        case AnnotationEvent::EventType::ANNOTATION_PATH_UPDATED:
        {
            update();
            return;
        }
    }

    Q_UNREACHABLE();
}

//-----------------------------------
void AnnotationHandler::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw the previous path
    for (const auto& annotation : m_annotations)
    {
        const auto& path = annotation->getPainterPath();
        if (path.isEmpty())
        {
            continue;
        }

        //this is the highlighted boundary area when an item is selected
        if(annotation->isSelected())
        {
            QPen borderPen((QColor(Qt::green)));
            borderPen.setStyle(Qt::DashLine);

            QPainterPathStroker stroker;
            stroker.setWidth(7);
            QPainterPath border_path= stroker.createStroke(path);

            painter->setPen(borderPen);
            painter->drawPath(border_path);
        }
        else if(annotation->isHovered())
        {
            QPen borderPen((QColor(Qt::white)));
            borderPen.setStyle(Qt::DashLine);

            QPainterPathStroker stroker;
            stroker.setWidth(7);
            QPainterPath hover_path = stroker.createStroke(annotation->getPainterPath());

            painter->setPen(borderPen);
            painter->drawPath(hover_path);
        }

        QPen pathPen(QColor(annotation->getColor()),5);
        painter->setPen(pathPen);

        painter->drawPath(path);
    }

    painter->setPen(QPen(QColor(m_pen_color), 5));
    //This is to instantly draw the lines
    for (int i = 1; i < m_current_points.size(); ++i)
    {
        QPointF first = m_current_points[i - 1];
        QPointF second = m_current_points[i];

        painter->drawLine(first, second);
    }
}


//-----------------------------------
void AnnotationHandler::handleAnnotationSelection(QMouseEvent *event)
{
    for(auto& annotation : m_annotations)
    {
        //if we never enter this if case, the licked mouse position does not have an annotation under it
        if(containsClick(annotation->getPainterPath(), event->pos()))
        {
            setFocus(true); //so we handle key events to delete annotations
            annotation->setSelected(true);
            m_initial_position = event->pos(); //This is the initial translation point if dragging happens
            m_original_path = annotation->getPainterPath();
            handleAnnotationStateChanged(AnnotationState::State::SELECTED);
        }
        else
        {
            annotation->setSelected(false);
        }
    }
}

//-----------------------------------
void AnnotationHandler::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        //stop drawing action
        handleAnnotationStateChanged(AnnotationState::State::NONE);
        return;
    }

    qWarning() << "Mouse point: " << event->position();

    if (event->button() == Qt::LeftButton)
    {
        handleAnnotationSelection(event);

        if(m_current_state == AnnotationState::State::DRAWING)
        {
            m_current_points.append(event->position());
        }
        update();
    }
}

//-----------------------------------
void AnnotationHandler::startAnnotationTranslation(QMouseEvent* event)
{
    for(auto& annotation : m_annotations)
    {
        if(annotation->isSelected())
        {
            handleAnnotationStateChanged(AnnotationState::State::TRANSLATION_STARTED);

            auto& painter_path = annotation->getPainterPath();
            const auto delta = event->position() - m_initial_position;
            painter_path.translate(delta);
            //we update the drag_initial_mouse_position after every update to prevent delta accumulation
            //Otherwise the delta will cause large translation
            m_initial_position = event->position();

            update();
        }
    }
}

//-----------------------------------
void AnnotationHandler::mouseMoveEvent(QMouseEvent *event)
{
    if(m_current_state == AnnotationState::State::DRAWING)
    {
        m_current_points.append(event->position());
    }
    else
    {
        startAnnotationTranslation(event); //possibly translating if not drawing
    }

    update();
}

//-----------------------------------
void AnnotationHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_current_state == AnnotationState::State::TRANSLATION_STARTED)
    {
        auto selected_annotation = getDataManager().getSelectedAnnotation();
        auto translation_undoredo = std::make_unique<AnnotationTranslationUndoRedo>(m_original_path
                                                                                    , selected_annotation);
        getDataManager().m_undo_stack.push(translation_undoredo.release());

        handleAnnotationStateChanged(AnnotationState::State::TRANSLATION_ENDED);
    }

    if(m_current_state == AnnotationState::State::DRAWING && event->button() == Qt::LeftButton)
    {
        //No point in drawing if there is just one point, it's basically noise
        if(m_current_points.size() == 1)
        {
            m_current_points.clear();
            return;
        }

        auto painter_path = convertPointsToPainterPath(m_current_points);

        auto& data_manager = getDataManager();
        auto new_annotation = std::make_shared<Annotation>(painter_path,
                                                           data_manager.getActiveImage()->getId(),
                                                           m_pen_color);

        auto command = new AddAnnotationUndoRedo(new_annotation, data_manager);
        data_manager.m_undo_stack.push(command);

        m_current_points.clear();

        update();
    }
}

//-----------------------------------
void AnnotationHandler::setupToolTip(const QPointF& position)
{
    for(auto& annotation : m_annotations)
    {
        if(containsClick(annotation->getPainterPath(), position))
        {
            const QPointF point = mapToGlobal(position);
            const QPoint final_point = {static_cast<int>(point.x()), static_cast<int>(point.y())};
            QToolTip::showText(final_point, annotation->getId(),nullptr,QRect(), 1000);
            annotation->setHovered(true);
            return;
        }
        else
        {
            annotation->setHovered(false);
        }
    }
}

//-----------------------------------
void AnnotationHandler::hoverMoveEvent(QHoverEvent *event)
{
    setupToolTip(event->position());
    update();
}

//-----------------------------------
void AnnotationHandler::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        for(auto& annotation : m_annotations)
        {
            if(annotation && annotation->isSelected())
            {
                auto& data_manager = getDataManager();
                auto remove_undo_command = std::make_unique<RemoveAnnotationUndoRedo>(annotation, data_manager);
                data_manager.m_undo_stack.push(remove_undo_command.release());
            }
        }
    }
}

//-----------------------------------
void AnnotationHandler::setPenColor(const QString &penColor)
{
    m_pen_color = penColor;
}

//-----------------------------------
void AnnotationHandler::updateCursorShape()
{
    switch(m_current_state)
    {
        case AnnotationState::State::NONE:
        case AnnotationState::State::SELECTED:
        case AnnotationState::State::TRANSLATION_ENDED:
        {
            setCursor(QCursor(Qt::ArrowCursor));
            return;
        }
        case AnnotationState::State::DRAWING:
        {
            setCursor(QCursor(Qt::CrossCursor));
            return;
        }
        case AnnotationState::State::TRANSLATION_STARTED:
        {
            setCursor(QCursor(Qt::SizeAllCursor));
            return;
        }
        case AnnotationState::State::ADDING_TEXT:
        {
            setCursor(QCursor(Qt::IBeamCursor));
            return;
        }
    }
}

//-----------------------------------
void AnnotationHandler::handleAnnotationStateChanged(AnnotationState::State&& state)
{
    m_current_state = state;
    updateCursorShape();
}

//-----------------------------------
void AnnotationHandler::updateStatus(AnnotationState::State newState)
{
    if(!getDataManager().getActiveImage())
    {
        qWarning() << "Upload/Select an image to start drawing!";
        return;
    }

    m_current_state = newState;

    updateCursorShape();
}
