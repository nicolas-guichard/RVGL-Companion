#include <QGuiApplication>
#include <singleapplication.h>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QUrl>
#include <QUrlQuery>
#include <QSettings>
#include <QIcon>
#include <QQuickStyle>
#include <QWindow>
#include "assetsmanager.h"
#include "rvgllauncher.h"

#ifdef Q_OS_WIN
#include <QSysInfo>
#else
#include <QFile>
#include <QTextStream>
#include <QProcess>
#endif

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, true);
    app.setApplicationName("RVGL Companion");
    app.setOrganizationName("Re-Volt.io");
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setApplicationVersion("1.1-RC2");
    app.setWindowIcon(QIcon("qrc:/icon.ico"));

    QSettings settings;

    #ifdef Q_OS_WIN
        if (QSysInfo::currentCpuArchitecture() == "x86_64" || QSysInfo::currentCpuArchitecture() == "ia64") {
            QSettings classes("HKEY_CURRENT_USER\\Software\\Classes\\rvgl",
                               QSettings::Registry64Format);
            classes.setValue(".", "rvgl scheme handler");
            QSettings protocol("HKEY_CURRENT_USER\\Software\\Classes\\rvgl\\URL Protocol",
                               QSettings::Registry64Format);
            protocol.setValue(".", "");
            QSettings command("HKEY_CURRENT_USER\\Software\\Classes\\rvgl\\shell\\open\\command",
                               QSettings::Registry64Format);
            command.setValue(".", "\""+QGuiApplication::applicationFilePath().replace('/', '\\')+"\" \"%1\"");
        }
        QSettings classes("HKEY_CURRENT_USER\\Software\\Classes\\rvgl",
                           QSettings::Registry32Format);
        classes.setValue(".", "rvgl scheme handler");
        QSettings protocol("HKEY_CURRENT_USER\\Software\\Classes\\rvgl\\URL Protocol",
                           QSettings::Registry32Format);
        protocol.setValue(".", "");
        QSettings command("HKEY_CURRENT_USER\\Software\\Classes\\rvgl\\shell\\open\\command",
                           QSettings::Registry32Format);
        command.setValue(".", "\""+QGuiApplication::applicationFilePath().replace('/', '\\')+"\" \"%1\"");
    #else
        QFile desktopFile(QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)+"/rvgl_companion.desktop");
        desktopFile.open(QIODevice::WriteOnly);
        QTextStream desktopTextFile(&desktopFile);
        desktopTextFile << "[Desktop Entry]\n"
                           "Version=1.0\n"
                           "Type=Application\n"
                           "Exec="+QGuiApplication::applicationFilePath()+" %u\n"
                           "Icon="+QGuiApplication::applicationDirPath()+"/icon.png\n"
                           "StartupNotify=true\n"
                           "Terminal=false\n"
                           "Categories=Utility;\n"
                           "MimeType=x-scheme-handler/rvgl\n"
                           "Name=RVGL Companion\n"
                           "Comment=Launch RVGL";
        desktopFile.close();
        QProcess desktopUpdater;
        desktopUpdater.start("update-desktop-database ~/.local/share/applications");
    #endif

    QCommandLineParser parser;
    parser.setApplicationDescription("RVGL companion app");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("uri", QCoreApplication::translate("main", "rvgl: URI indicating what operation to execute."), "[uri]");
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    qmlRegisterType<AssetsManager>("AssetsManager", 1, 0, "AssetsManager");
    qmlRegisterType<RVGLLauncher>("RVGLLauncher", 1, 0, "RVGLLauncher");

    QString func;
    QString option;

    if (args.length() > 0) {
        QUrl url(args.at(0));
        func = url.fileName();
        QUrlQuery query(url);
        if (func == "run" || func == "join") {
            RVGLLauncher* launcher = new RVGLLauncher();
            QString dir = settings.value("installs").toList()[0].toMap()["dir"].toString();
            QStringList launchOptions = settings.value("installs").toList()[0].toMap()["options"].toString().split(' ');
            QPair<QString, QString> optionPair;
            foreach (optionPair, query.queryItems()) {
                launchOptions << "-"+optionPair.first << optionPair.second;
            }
            if (func == "join") {
                launchOptions << "-lobby "+query.queryItemValue("IP");
            }
            QProcess* rvgl = launcher->launchRaw(dir, launchOptions);
            QObject::connect(rvgl, SIGNAL (destroyed()), &app, SLOT (quit()));
            return app.exec();
        } else if (func == "install_asset") {
            option = query.queryItemValue("URL");
        }
    }

    if (app.isSecondary()){
        app.sendMessage(func.toLatin1()+'\\'+option.toLatin1());
        return 0;
    }

    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());

    QQmlApplicationEngine engine;
    engine.load("qrc:/main.qml");
    engine.rootObjects().at(0)->setProperty("availableStyles", QStringList() << "Default" << "Material" << "Universal");
    engine.rootObjects().at(0)->setProperty("func", func);
    engine.rootObjects().at(0)->setProperty("option", option);

    QObject::connect(
        &app, SIGNAL (instanceStarted()),
        engine.rootObjects().at(0), SLOT (raise())
    );

    QObject::connect(
        &app, &SingleApplication::receivedMessage, [&](qint32 id, QByteArray message) {
            qDebug() << "Secondary instance" << id << "sent us" << message;
            engine.rootObjects().at(0)->setProperty("func", "");
            engine.rootObjects().at(0)->setProperty("func", message.split('\\')[0]);
            engine.rootObjects().at(0)->setProperty("option", "");
            engine.rootObjects().at(0)->setProperty("option", message.split('\\')[1]);
        }
    );

    return app.exec();
}
