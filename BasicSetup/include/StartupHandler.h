#pragma once

#include "DataManager.h"
#include "AnnotationModel.h"
#include "ImageModel.h"
#include "AnnotationHandler.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

class StartupHandler
{
public:
    //-----------------------------------
    StartupHandler(QQmlApplicationEngine& engine)
    {
        auto& data_manager = DataManager::instance();

        qmlRegisterType<AnnotationHandler>("com.mycompany.qmlcomponents", 1, 1, "AnnotationHandler");

        m_image_model = std::make_shared<ImageModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_image_model", m_image_model.get());

        m_annotation_model = std::make_shared<AnnotationModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_annotation_model", m_annotation_model.get());

        engine.rootContext()->setContextProperty("cpp_datamanager", &data_manager);
    }

private:
    std::shared_ptr<AnnotationModel> m_annotation_model;
    std::shared_ptr<ImageModel> m_image_model;
};
