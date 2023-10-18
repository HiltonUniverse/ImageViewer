#pragma once

#include "DataManager.h"
#include "Serializer.h"

#include <QObject>
#include <QDebug>

class LoadHandler : public QObject
{
    Q_OBJECT
public:
    //-----------------------------------
    LoadHandler(DataManagerImpl& dataManager)
        : m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    Q_INVOKABLE void start()
    {
        Serializer serializer(m_datamanager);
        serializer.deserialize();
    }

private:
    DataManagerImpl& m_datamanager;
};
