#pragma once

#include "Annotation.h"

#include <QUndoCommand>
#include <QPainterPath>

class AnnotationTranslationUndoRedo : public QUndoCommand
{
public:
    AnnotationTranslationUndoRedo(const QPainterPath& originalPath
                                  ,std::shared_ptr<Annotation> annotation)
        : m_original_path(originalPath)
        , m_new_path(annotation->getPainterPath())
        , m_annotation(annotation)
    {
    }

    //-----------------------------------
    void undo() override
    {
        QUndoCommand::undo();
        m_annotation->setPainterPath(m_original_path);
    }

    //-----------------------------------
    void redo() override
    {
        QUndoCommand::redo();
        m_annotation->setPainterPath(m_new_path);
    }

private:
    QPainterPath m_original_path;
    QPainterPath m_new_path;
    std::shared_ptr<Annotation> m_annotation;
};
