#pragma once

#include "Serializable.h"
#include "DataManager.h"
#include "Annotation.h"

#include <QDebug>

class AnnotationSerializable : public Serializable
{
public:
    //-----------------------------------
    AnnotationSerializable(DataManagerImpl& dataManager)
        : m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    ~AnnotationSerializable() override = default;

    //-----------------------------------
    void fromJson(const QJsonDocument& jsonDocument) override
    {
    }

    //-----------------------------------
    QJsonDocument toJson() override
    {
    }

private:
    DataManagerImpl& m_datamanager;
};
