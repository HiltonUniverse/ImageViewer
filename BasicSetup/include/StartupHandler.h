#pragma once

#include "DataManager.h"
#include "Image.h"
#include "Annotation.h"
#include "AnnotationModel.h"
#include "ImageModel.h"
#include "AnnotationHandler.h"

#include <QDir>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

class StartupHandler
{
public:
    //-----------------------------------
    StartupHandler(QQmlApplicationEngine& engine)
    {
        auto& data_manager = DataManager::instance();

        qmlRegisterType<AnnotationHandler>("com.mycompany.qmlcomponents", 1, 1, "AnnotationHandler");

        //browse files inside the path
        QDir directory("/Users/Hilton/Desktop/Images");
        QStringList name_list = directory.entryList(QDir::Files);

        for(auto &name : name_list)
        {
            const QString path = directory.absoluteFilePath(name).prepend("file://");
            auto image = std::make_shared<Image>();
            image->setPath(path);
            data_manager.addImage(image);
        }

        m_image_model = std::make_shared<ImageModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_image_model", m_image_model.get());

        m_dummy_model = std::make_shared<AnnotationModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_dummy_model", m_dummy_model.get());

        auto image = data_manager.getImages().front();
        data_manager.setActiveImage(image);

        engine.rootContext()->setContextProperty("cpp_datamanager", &data_manager);
    }

private:
    std::shared_ptr<AnnotationModel> m_dummy_model;
    std::shared_ptr<ImageModel> m_image_model;
};
