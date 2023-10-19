#pragma once

#include "ObserverPattern.h"
#include "AnnotationEvent.h"
#include "UniqueKeyGenerator.h"

#include <QString>
#include <QPainterPath>

class Annotation : public pattern::Observable<Annotation, AnnotationEvent::EventType>
{
public:
    //-----------------------------------
    Annotation(const QPainterPath& path,
               const QString& parentId,
               const QString& color)
        : m_painter_path(path)
        , m_id(UniqueKeyGenerator::getUniqueKey())
        , m_parent_id(parentId)
        , m_color(color)
        , m_is_selected(false)
        , m_is_hovered(false)
    {
    }

    //-----------------------------------
    QString getNotification()
    {
        return "Annotation";
    }

    //-----------------------------------
    QString getParentId()
    {
        return m_parent_id;
    }

    //-----------------------------------
    QString getId()
    {
        return m_id;
    }

    //-----------------------------------
    void setId(const QString& id)
    {
        m_id = id;
    }

    //-----------------------------------
    QPainterPath getPainterPath()
    {
        return m_painter_path;
    }

    //-----------------------------------
    void setColor(const QString& color)
    {
        m_color = color;
    }

    //-----------------------------------
    QString getColor() const
    {
        return m_color;
    }

    //-----------------------------------
    bool isSelected() const
    {
        return m_is_selected;
    }

    //-----------------------------------
    void setSelected(bool selected)
    {
        m_is_selected = selected;
        AnnotationEvent::EventType event = AnnotationEvent::EventType::ANNOTATION_SELECTED;
        notify(event);
    }

    //-----------------------------------
    void setHovered(bool hovered)
    {
        m_is_hovered = hovered;
        AnnotationEvent::EventType event = AnnotationEvent::EventType::ANNOTATION_HOVERED;
        notify(event);
    }

    //-----------------------------------
    bool isHovered() const
    {
        return m_is_hovered;
    }

private:
    QPainterPath m_painter_path;
    QString m_id;
    QString m_parent_id;
    QString m_color;
    bool m_is_selected;
    bool m_is_hovered;
};
