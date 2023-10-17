#pragma once

#include "DataManager.h"
#include "ResetAnnotationHandler.h"
#include "ResetAnnotationUndoRedo.h"

#include <QDebug>
#include <QObject>

class ResetAnnotationHandler : public QObject
{
    Q_OBJECT
public:
    ResetAnnotationHandler(DataManagerImpl& dataManager)
        : m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    ~ResetAnnotationHandler() override = default;

    //-----------------------------------
    Q_INVOKABLE void start()
    {
        auto current_annotations = m_datamanager.getActiveImage()->getAnnotations();
        auto reset_annotation_undo_redo = std::make_unique<ResetAnnotationUndoRedo>(m_datamanager
                                                                                    , current_annotations);
        m_datamanager.m_undo_stack.push(reset_annotation_undo_redo.release());
    }

private:
    DataManagerImpl& m_datamanager;
};
