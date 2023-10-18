#pragma once

#include "DataManager.h"

#include "Image.h"

#include "Serializable.h"

#include <QJsonObject>
#include <QJsonArray>

class ImageSerializer : public Serializable
{
public:
    //-----------------------------------
    ImageSerializer(DataManagerImpl& dataManager)
        : m_datamanger(dataManager)
    {
    }

    //-----------------------------------
    ~ImageSerializer() override = default;

    //-----------------------------------
    void fromJson(const QJsonDocument& jsonDocument) override
    {
        if(jsonDocument.isNull())
        {
            return;
        }

        QJsonArray jsonArray = jsonDocument.array();
        for(auto&& jsonObj : jsonArray)
        {
            const auto json_object = jsonObj.toObject();
            const QString name = json_object["name"].toString();
            const QString path = json_object["path"].toString();
            const QString id = json_object["id"].toString();
            const bool is_selected = json_object["is_selected"].toBool();

            auto image = std::make_shared<Image>(name, path);
            image->setSelected(is_selected);
            image->setId(id);

            m_datamanger.addImage(image);
            if(is_selected)
            {
                m_datamanger.setActiveImage(image);
            }
        }
    }

    //-----------------------------------
    QJsonDocument toJson() override
    {
        auto images = m_datamanger.getImages();
        if(images.empty())
        {
            return {};
        }

        QJsonArray image_array;
        for(auto& image : images)
        {
            QJsonObject image_object;
            image_object["name"] = image->getName();
            image_object["path"] = image->getPath();
            image_object["id"] = image->getId();
            image_object["is_selected"] = image->isSelected();

            image_array.append(image_object);
        }

        QJsonDocument document(image_array);
        return document;
    }

private:
    DataManagerImpl& m_datamanger;
};
