#pragma once

#include "Annotation.h"
#include "DataObject.h"
#include "UniqueKeyGenerator.h"

#include <QString>

#include <vector>

class Image : public DataObject
{
public:
    //-----------------------------------
    Image(const QString& name, const QString& path)
        : m_path(path)
        , m_name(name)
        , m_id(UniqueKeyGenerator::getUniqueKey())
        , m_is_selected(false)
    {
    }

    //-----------------------------------
    ~Image() override = default;

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
    QString getName()
    {
        return m_name;
    }

    //-----------------------------------
    void setPath(const QString& path)
    {
        m_path = path;
    }

    //-----------------------------------
    QString getPath() const
    {
        return m_path;
    }

    //-----------------------------------
    void addAnnotation(std::shared_ptr<Annotation> annotation)
    {
        m_annotations.push_back(annotation);
    }

    //-----------------------------------
    void removeAnnotation(std::shared_ptr<Annotation> annotation)
    {
        m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), annotation), m_annotations.end());
    }

    //-----------------------------------
    std::vector<std::shared_ptr<Annotation>>& getAnnotations()
    {
        return m_annotations;
    }

    //-----------------------------------
    void setSelected(bool value)
    {        
        m_is_selected = value;
    }

    //-----------------------------------
    bool isSelected() const
    {
        return m_is_selected;
    }

private:
    QString m_path;
    QString m_name;
    QString m_id;
    bool m_is_selected;

    std::vector<std::shared_ptr<Annotation>> m_annotations;
};
