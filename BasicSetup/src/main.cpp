#include "StartupHandler.h"

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QApplication>

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    StartupHandler handler(engine);

    engine.load(QUrl(QStringLiteral("qrc:/Basic/src/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
