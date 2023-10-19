#pragma once
#include "Image.h"
#include "SingletonPattern.h"

#include <QObject>
#include <QUndoStack>

#include <vector>

class DataManagerImpl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ imagePath NOTIFY activeImageChanged)
public:
    //-----------------------------------
    void setActiveImage(std::shared_ptr<Image> image)
    {
        if(image == m_active_image)
        {
            return;
        }

        emit activeImageAboutToChanged(m_active_image);

        if(m_active_image)
        {
            m_active_image->setSelected(false);
        }

        m_active_image = image;
        m_active_image->setSelected(true);

        emit activeImageChanged(image);
    }

    //-----------------------------------
    void addImage(std::shared_ptr<Image> image)
    {
        m_images.push_back(image);

        if(m_images.size() == 1)
        {
            setActiveImage(image);
        }

        emit imageAdded(image);
    }

    //-----------------------------------
    void removeImages()
    {
        m_images.clear();
    }

    //-----------------------------------
    void addAnnotation(std::shared_ptr<Annotation> annotation)
    {
        if(m_active_image)
        {
            m_active_image->addAnnotation(annotation);
            emit annotationAdded(annotation);
        }
    }

    //-----------------------------------
    void removeAnnotation(std::shared_ptr<Annotation> annotation)
    {
        if(m_active_image)
        {
            m_active_image->removeAnnotation(annotation);
            emit annotationRemoved(annotation);
        }
    }

    //-----------------------------------
    std::shared_ptr<Annotation> getSelectedAnnotation() const
    {
        if(!m_active_image)
        {
            return nullptr;
        }

        for(auto& annotation : m_active_image->getAnnotations())
        {
            if(annotation->isSelected())
            {
                return annotation;
            }
        }

        return nullptr;
    }

    //-----------------------------------
    std::vector<std::shared_ptr<Image>>& getImages()
    {
        return m_images;
    }

    //-----------------------------------
    std::shared_ptr<Image> getActiveImage()
    {
        return m_active_image;
    }

    //-----------------------------------
    QString imagePath() const
    {
        if(m_active_image)
        {
            return m_active_image->getPath();
        }

        return "";
    }

    //-----------------------------------
    Q_INVOKABLE void undo()
    {
        if(m_undo_stack.count() > 0)
        {
            m_undo_stack.undo();
        }
    }

    //-----------------------------------
    Q_INVOKABLE void redo()
    {
        if(m_undo_stack.count() > 0)
        {
            m_undo_stack.redo();
        }
    }

    QUndoStack m_undo_stack;

signals:
    void activeImageAboutToChanged(std::shared_ptr<Image> image);
    void activeImageChanged(std::shared_ptr<Image> image);
    void annotationAdded(std::shared_ptr<Annotation> annotation);
    void annotationRemoved(std::shared_ptr<Annotation> annotation);
    void imageAdded(std::shared_ptr<Image> image);

private:
    std::vector<std::shared_ptr<Image>> m_images;

    std::shared_ptr<Image> m_active_image;
};

template class SingletonPattern<DataManagerImpl>;

using DataManager = SingletonPattern<DataManagerImpl>;
