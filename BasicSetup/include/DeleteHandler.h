#pragma once

#include "DataManager.h"

#include <QObject>
#include <QKeyEvent>

class DeleteHandler : public QObject
{
    Q_OBJECT
public:
    //-----------------------------------
    DeleteHandler(DataManagerImpl& dataManager,
                  QObject* parent = nullptr)
        : QObject(parent)
        , m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    ~DeleteHandler() override = default;

    //-----------------------------------
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        qWarning() << "HELLO WROLD!";
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            qDebug("Ate key press %d", keyEvent->key());
            emit deleteKeyPressed();
            return true;
        }
        // standard event processing
        return QObject::eventFilter(obj, event);
    }

signals:
    void deleteKeyPressed();

private:
    DataManagerImpl& m_datamanager;
};
