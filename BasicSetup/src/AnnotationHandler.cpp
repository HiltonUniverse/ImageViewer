#include "AnnotationHandler.h"
#include "AnnotationUndoRedo.h"
#include "DataManager.h"

#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

//-----------------------------------
AnnotationHandler::AnnotationHandler()
    : m_can_draw(false)
{
    setFlag(ItemAcceptsInputMethod, true);
    setupConnection();
}

//-----------------------------------
void AnnotationHandler::setupConnection()
{
    auto& data_manager = DataManager::instance();

    connect(&data_manager, &DataManagerImpl::activeImageChanged, [this](auto image)
    {
        m_annotations.clear();

        for(auto& annotation : image->getAnnotations())
        {
            m_annotations.push_back(annotation);
        }

        update();
    });

    connect(&data_manager, &DataManagerImpl::annotationAdded, [this](auto annotation)
    {
        m_annotations.push_back(annotation);
        update();
    });

    connect(&data_manager, &DataManagerImpl::annotationRemoved, [this](auto annotation)
    {
        m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), annotation), m_annotations.end());
        update();
    });
}

//-----------------------------------
void AnnotationHandler::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor("#000000"), 5));

    // Draw the previous path
    for (const auto& annotation : m_annotations)
    {
        const auto& path = annotation->getPoints();
        if (!path.isEmpty())
        {
            for (int i = 1; i < path.size(); ++i)
            {
                QPointF first = path[i - 1];
                QPointF second = path[i];
                painter->drawLine(first, second);
            }
        }
    }

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
    if(!m_can_draw)
    {
        return;
    }

    //stop drawing action
    if (event->button() == Qt::RightButton)
    {
        setCanDraw(false);
        setAcceptedMouseButtons({Qt::NoButton});
        return;
    }

    m_current_points.append(event->position()); // Start a new path
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
        auto new_annotation = std::make_shared<Annotation>(m_current_points, data_manager.getActiveImage()->getId());
        auto command = new AnnotationUndoRedo(new_annotation, data_manager);
        data_manager.m_undo_stack.push(command);

        m_current_points.clear();

        update();
    }
}

//-----------------------------------
void AnnotationHandler::setCanDraw(bool canDraw)
{
    m_can_draw = canDraw;

    setAcceptedMouseButtons({Qt::LeftButton, Qt::RightButton});

    if(m_can_draw)
    {
        setCursor(QCursor(Qt::CrossCursor));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}
