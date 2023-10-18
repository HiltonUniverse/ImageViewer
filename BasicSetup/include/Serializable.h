#pragma once

#include "DataObject.h"

#include <QJsonDocument>


class Serializable
{
public:
    //-----------------------------------
    Serializable() = default;

    //-----------------------------------
    virtual ~Serializable() = default;


protected:
    virtual void fromJson(const QJsonDocument& jsonDocument) = 0;
    virtual QJsonDocument toJson() = 0;
};
