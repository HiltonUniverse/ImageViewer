#pragma once

#include <QString>
#include <QPointF>
#include <QVector>

class Annotation
{
public:
    //-----------------------------------
    Annotation(const QVector<QPointF>& points, const QString& parentId)
        : m_points(points)
        , m_parent_id(parentId)
    {
        ++m_annotation_count;
        m_id = QString::number(m_annotation_count);
    }

    //-----------------------------------
    ~Annotation() = default;

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
    QVector<QPointF> getPoints() const
    {
        return m_points;
    }

private:
    inline static int m_annotation_count = 0;

    QVector<QPointF> m_points;
    QString m_id;
    QString m_parent_id;
};
