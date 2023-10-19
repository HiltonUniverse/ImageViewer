#include "AnnotationHandler.h"
#include "AddAnnotationUndoRedo.h"
#include "RemoveAnnotationUndoRedo.h"
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
}

//-----------------------------------
AnnotationHandler::AnnotationHandler()
    : m_can_draw(false)
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
            break;
        }
        case AnnotationEvent::EventType::ANNOTATION_HOVERED:
            break;
    }

    update();
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
void AnnotationHandler::mousePressEvent(QMouseEvent *event)
{
    //stop drawing action
    if (event->button() == Qt::RightButton)
    {
        setCanDraw(false);
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        m_current_points.append(event->position());

        for(auto& annotation : m_annotations)
        {
            if(containsClick(annotation->getPainterPath(), event->pos()))
            {
                annotation->setSelected(true);
                setFocus(true);
            }
            else
            {
                annotation->setSelected(false);
            }
        }
    }

    update();
}

//-----------------------------------
void AnnotationHandler::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_can_draw)
    {
        return;
    }

    m_current_points.append(event->position());
    update();
}

//-----------------------------------
void AnnotationHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_can_draw || m_current_points.size() == 1)
    {
        m_current_points.clear();
        return;
    }

    if(event->button() == Qt::LeftButton)
    {
        auto& data_manager = DataManager::instance();
        QPainterPath path;
        path.moveTo(m_current_points.front());
        for(auto& p : m_current_points)
        {
            path.lineTo(p);
        }

        auto new_annotation = std::make_shared<Annotation>(path,
                                                           data_manager.getActiveImage()->getId(),
                                                           m_pen_color);

        auto command = new AddAnnotationUndoRedo(new_annotation, data_manager);
        data_manager.m_undo_stack.push(command);

        m_current_points.clear();

        update();
    }
}

//-----------------------------------
void AnnotationHandler::hoverMoveEvent(QHoverEvent *event)
{
    for(auto& annotation : m_annotations)
    {
        if(containsClick(annotation->getPainterPath(), event->position()))
        {
            QPointF point = mapToGlobal(event->position());
            QPoint final_point = {static_cast<int>(point.x()), static_cast<int>(point.y())};
            QToolTip::showText(final_point, annotation->getId(),nullptr,QRect(), 1000);
            annotation->setHovered(true);
            update();
        }
        else
        {
            annotation->setHovered(false);
            update();
        }
    }
}

//-----------------------------------
void AnnotationHandler::dragMoveEvent(QDragMoveEvent *event)
{
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
void AnnotationHandler::setCanDraw(bool canDraw)
{
    m_can_draw = canDraw;

    if(m_can_draw)
    {
        setCursor(QCursor(Qt::CrossCursor));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}
