#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtQuick>
#include <QQuickStyle>

#include "ledcolors.h"
#include "lastcolors.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Vladest Software"));
    app.setApplicationName(QStringLiteral("LEDMatrixEditor"));
    app.setApplicationVersion("0.0.1");

    qDebug() << QQuickStyle::availableStyles();
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine* engine = new QQmlApplicationEngine;

    LEDColors* ledColors = new LEDColors;
    LastColors* lastColors = new LastColors;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine->rootContext()->setContextProperty("colorModel", ledColors);
    engine->rootContext()->setContextProperty("lastColorsModel", lastColors);
    engine->load(url);

    int ret = app.exec();

    delete engine;
    delete lastColors;
    delete ledColors;
    return ret;
}
