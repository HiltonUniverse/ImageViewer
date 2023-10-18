#pragma once

#include "DataManager.h"

#include "Image.h"

#include "Serializable.h"

#include <QJsonObject>
#include <QJsonArray>

namespace
{
//-----------------------------------
std::vector<std::shared_ptr<Annotation>> convertArrayToAnnotation(QJsonArray&& jsonArray)
{
    if(jsonArray.isEmpty())
    {
        return {};
    }

    std::vector<std::shared_ptr<Annotation>> annotations;
    for(auto&& jsonObj : jsonArray)
    {
        const auto json_object = jsonObj.toObject();
        const QString& id = json_object["id"].toString();
        const QString& parent_id = json_object["parent_id"].toString();
        const QString& color = json_object["color"].toString();
        const auto& points_arry = json_object["points"].toArray();

        QVector<QPointF> points;
        for(auto point_array : points_arry)
        {
            auto point_obj = point_array.toObject();
            QPointF point;
            point.setX(point_obj["x"].toDouble());
            point.setY(point_obj["y"].toDouble());
            points.push_back(point);
        }

        QPainterPath painter_path;
        if(!points.empty())
        {
            painter_path.moveTo(points[0]);
            for(auto& pt : points)
            {
                painter_path.lineTo(pt);
            }
        }

        auto annotation = std::make_shared<Annotation>(painter_path, parent_id, color);
        annotation->setId(id);
        annotations.push_back(annotation);
    }

    return annotations;
}

//-----------------------------------
QJsonArray convertAnnotationToArray(std::vector<std::shared_ptr<Annotation>>& annotations)
{
    QJsonArray annotation_array;

    for(auto& annotation : annotations)
    {
        QJsonObject json_object;
        json_object["id"] = annotation->getId();
        json_object["parent_id"] = annotation->getParentId();
        json_object["color"] = annotation->getColor();

        auto painter_path = annotation->getPainterPath();
        QJsonArray points_array;
        for(int i = 0; i < painter_path.elementCount(); ++i)
        {
            const auto element = painter_path.elementAt(i);
            QJsonObject object;
            object["x"] = element.x;
            object["y"] = element.y;

            points_array.append(object);
        }

        json_object["points"] = points_array;

        annotation_array.append(json_object);
    }

    return annotation_array;
}
}

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
            const auto& annotations = convertArrayToAnnotation(json_object["annotations"].toArray());

            auto image = std::make_shared<Image>(name, path);
            image->setSelected(is_selected);
            image->setId(id);

            for(auto& annotation : annotations)
            {
                image->addAnnotation(annotation);
            }

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
            image_object["annotations"] = convertAnnotationToArray(image->getAnnotations());

            image_array.append(image_object);
        }

        QJsonDocument document(image_array);
        return document;
    }

private:
    DataManagerImpl& m_datamanger;
};
