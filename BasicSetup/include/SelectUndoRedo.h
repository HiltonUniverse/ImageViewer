#pragma once
#include "Image.h"
#include "DataManager.h"

#include <QUndoCommand>

class SelectUndoRedo : public QUndoCommand
{
public:
    SelectUndoRedo( std::shared_ptr<Image> new_image
                   , DataManagerImpl& datamanager )
        : m_previous_image(nullptr)
        , m_new_image(new_image)
        , m_datamanager(datamanager)
    {
    }

    void undo() override
    {
        QUndoCommand::undo();
        m_datamanager.setActiveImage(m_previous_image);
    }

    void redo() override
    {
        QUndoCommand::redo();
        m_previous_image = m_datamanager.getActiveImage();
        m_datamanager.setActiveImage(m_new_image);
    }

private:
    std::shared_ptr<Image> m_previous_image;
    std::shared_ptr<Image> m_new_image;
    DataManagerImpl& m_datamanager;
};
