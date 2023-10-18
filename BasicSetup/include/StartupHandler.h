#pragma once

#include "DataManager.h"
#include "AnnotationModel.h"
#include "ImageModel.h"
#include "AnnotationHandler.h"
#include "ResetAnnotationHandler.h"
#include "SaveHandler.h"
#include "LoadHandler.h"

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

        m_reset_annotation_handler = std::make_shared<ResetAnnotationHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_reset_annotation_handler", m_reset_annotation_handler.get());

        m_save_handler = std::make_shared<SaveHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_save_handler", m_save_handler.get());

        m_load_handler = std::make_shared<LoadHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_load_handler", m_load_handler.get());
    }

private:
    std::shared_ptr<AnnotationModel> m_annotation_model;
    std::shared_ptr<ImageModel> m_image_model;
    std::shared_ptr<ResetAnnotationHandler> m_reset_annotation_handler;
    std::shared_ptr<SaveHandler> m_save_handler;
    std::shared_ptr<LoadHandler> m_load_handler;
};
