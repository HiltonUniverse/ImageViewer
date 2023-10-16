#pragma once

#include "Annotation.h"
#include "DataManager.h"

#include <QUndoCommand>

class AnnotationUndoRedo : public QUndoCommand
{
public:
    AnnotationUndoRedo( std::shared_ptr<Annotation> annotation
                        , DataManagerImpl& datamanager)
        : m_annotation(annotation)
        , m_datamanager(datamanager)
    {
    }

    //-----------------------------------
    void undo() override
    {
        QUndoCommand::undo();
        m_datamanager.removeAnnotation(m_annotation);
    }

    //-----------------------------------
    void redo() override
    {
        QUndoCommand::redo();
        m_datamanager.addAnnotation(m_annotation);
    }

private:
    std::shared_ptr<Annotation> m_annotation;
    DataManagerImpl& m_datamanager;
};
