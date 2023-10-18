#pragma once

#include "ImageSerializer.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

class Serializer
{
public:
    Serializer(DataManagerImpl& dataManager)
        : m_datamanager(dataManager)
    {
    }

    //-----------------------------------
    void serialize()
    {
        //Image
        ImageSerializer image_serializer(m_datamanager);
        auto json_document = image_serializer.toJson();
        save(json_document, "ImageSerializable");
    }

    //-----------------------------------
    void deserialize()
    {
        //for Image
        ImageSerializer image_serializer(m_datamanager);
        auto json_document = read("ImageSerializable");
        image_serializer.fromJson(json_document);
    }

private:
    //-----------------------------------
    void save(const QJsonDocument& jsonDocument, const QString& fileName)
    {
        const QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QFile file(desktop_path+ "/" + fileName + ".json");

        if (file.open(QIODevice::WriteOnly))
        {
            // Write the JSON data to the file
            file.write(jsonDocument.toJson());

            // Close the file
            file.close();
        } else
        {
            qWarning("Failed to open the file for writing.");
        }
    }

    //-----------------------------------
    QJsonDocument read(const QString& fileName)
    {
        const QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QFile file(desktop_path+ "/" + fileName + ".json");

        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
            if (jsonDocument.isNull())
            {
                qWarning() << "Read file is empty!";
                return {};
            }

            return jsonDocument;
        }
        else
        {
            qWarning("Failed to open the file for reading.");
            return {};
        }
    }

    DataManagerImpl& m_datamanager;
};
