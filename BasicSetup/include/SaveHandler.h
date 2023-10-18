#pragma once

#include "DataManager.h"
#include "Serializer.h"

#include <QObject>
#include <QDebug>

class SaveHandler : public QObject
{
    Q_OBJECT
public:
    //-----------------------------------
    SaveHandler(DataManagerImpl& dataManager)
        : m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    Q_INVOKABLE void start()
    {
        Serializer serializer(m_datamanager);
        serializer.serialize();
    }

private:
    DataManagerImpl& m_datamanager;
};
