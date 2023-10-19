#pragma once

#include "Annotation.h"
#include "DataManager.h"

#include <QUndoCommand>

class RemoveAnnotationUndoRedo : public QUndoCommand
{
public:
    RemoveAnnotationUndoRedo( std::shared_ptr<Annotation> annotation
                              , DataManagerImpl& datamanager)
        : m_annotation(annotation)
        , m_datamanager(datamanager)
    {
    }

    //-----------------------------------
    void undo() override
    {
        QUndoCommand::undo();

        m_datamanager.addAnnotation(m_annotation);
    }

    //-----------------------------------
    void redo() override
    {
        QUndoCommand::redo();

        m_annotation->setHovered(false);
        m_annotation->setSelected(false);
        m_datamanager.removeAnnotation(m_annotation);
    }

private:
    std::shared_ptr<Annotation> m_annotation;
    DataManagerImpl& m_datamanager;
};
