#pragma once

#include "Serializable.h"
#include "DataManager.h"
#include "Annotation.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

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
        auto& images = m_datamanager.getImages();

    }

private:
    DataManagerImpl& m_datamanager;
};
