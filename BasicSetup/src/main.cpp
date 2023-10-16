#include <QDebug>
#include <QQmlApplicationEngine>
#include <QApplication>
#include "StartupHandler.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //qmlRegisterType<AnnotationHandler>("com.example", 1, 0, "AnnotationHandler");

    QQmlApplicationEngine engine;
    StartupHandler handler(engine);

    engine.load(QUrl(QStringLiteral("qrc:/Basic/src/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
