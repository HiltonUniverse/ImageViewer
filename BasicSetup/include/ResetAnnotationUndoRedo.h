#pragma once

#include "DataManager.h"

#include <QUndoCommand>

class ResetAnnotationUndoRedo : public QUndoCommand
{
public:
    //-----------------------------------
    ResetAnnotationUndoRedo(DataManagerImpl& dataManager
                            , std::vector<std::shared_ptr<Annotation>> currentAnnotations)
        : m_datamanager(dataManager)
        , m_current_annotations(currentAnnotations)
    {
    }

    //-----------------------------------
    ~ResetAnnotationUndoRedo() override = default;

    //-----------------------------------
    void undo() override
    {
        QUndoCommand::undo();
        for(auto& annotation : m_current_annotations)
        {
            m_datamanager.addAnnotation(annotation);
        }
    }

    //-----------------------------------
    void redo() override
    {
        QUndoCommand::redo();
        for(auto& annotation : m_current_annotations)
        {
            m_datamanager.removeAnnotation(annotation);
        }
    }

private:
    DataManagerImpl& m_datamanager;
    std::vector<std::shared_ptr<Annotation>> m_current_annotations;
};
