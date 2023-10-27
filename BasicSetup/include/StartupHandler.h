#pragma once

#include "DataManager.h"
#include "AnnotationModel.h"
#include "ImageModel.h"
#include "AnnotationHandler.h"
#include "ResetAnnotationHandler.h"
#include "SaveHandler.h"
#include "LoadHandler.h"
#include "ImageProcessingHandler.h"
#include "ImageProvider.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QObject>

class StartupHandler : public QObject
{
    Q_OBJECT
public:
    //-----------------------------------
    StartupHandler(QQmlApplicationEngine& engine)
    {
        auto& data_manager = DataManager::instance();

        qmlRegisterType<AnnotationHandler>("com.mycompany.qmlcomponents", 1, 1, "AnnotationHandler");
        //expose enum to qml so we can easily set the AnnotationState
        qmlRegisterUncreatableType<AnnotationState>("com.mycompany.qmlcomponents", 1, 1, "AnnotationState", "Cannot create AnnotationState in QML");

        m_image_model = std::make_shared<ImageModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_image_model", m_image_model.get());

        //Image provider
        auto image_provider = m_image_model->getImageProvider();
        engine.addImageProvider("cpp_image_provider", image_provider);

        m_annotation_model = std::make_shared<AnnotationModel>(data_manager);
        engine.rootContext()->setContextProperty("cpp_annotation_model", m_annotation_model.get());

        engine.rootContext()->setContextProperty("cpp_datamanager", &data_manager);

        m_reset_annotation_handler = std::make_shared<ResetAnnotationHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_reset_annotation_handler", m_reset_annotation_handler.get());

        m_save_handler = std::make_shared<SaveHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_save_handler", m_save_handler.get());

        m_load_handler = std::make_shared<LoadHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_load_handler", m_load_handler.get());

        m_image_processing_handler = std::make_shared<ImageProcessingHandler>(data_manager);
        engine.rootContext()->setContextProperty("cpp_image_processing_handler", m_image_processing_handler.get());
    }

private:
    std::shared_ptr<AnnotationModel> m_annotation_model;
    std::shared_ptr<ImageModel> m_image_model;
    std::shared_ptr<ResetAnnotationHandler> m_reset_annotation_handler;
    std::shared_ptr<SaveHandler> m_save_handler;
    std::shared_ptr<LoadHandler> m_load_handler;
    std::shared_ptr<ImageProcessingHandler> m_image_processing_handler;
};
