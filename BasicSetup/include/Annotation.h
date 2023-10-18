#pragma once

#include <QString>
#include <QPainterPath>
#include <QVector>
#include <QGraphicsItem>

class Annotation
{
public:
    //-----------------------------------
    Annotation(const QPainterPath& path,
               const QString& parentId,
               const QString& color,
               QGraphicsItem* parent = nullptr)
        : m_painter_path(path)
        , m_parent_id(parentId)
        , m_color(color)
        , m_is_selected(false)
    {
        ++m_annotation_count;
        m_id = QString::number(m_annotation_count);
    }

    //-----------------------------------
    QString getNotification()
    {
        return "It's Annotation";
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
    QVector<QPointF> getPoints() const
    {
        return m_points;
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
    }

private:
    inline static int m_annotation_count = 0;

    QVector<QPointF> m_points;
    QPainterPath m_painter_path;
    QString m_id;
    QString m_parent_id;
    QString m_color;
    bool m_is_selected;
};
