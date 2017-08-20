#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QUrl>
#include <QUrlQuery>
#include <QSettings>
#include <QIcon>
#include <QQuickStyle>
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
    QGuiApplication app(argc, argv);
    app.setApplicationName("RVGL Companion");
    app.setOrganizationName("Re-Volt.io");
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setApplicationVersion("1.0RC");
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
                           "Icon="+QGuiApplication::applicationDirPath()+"/images/icon.png\n"
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
            QStringList launchOptions = settings.value("rvglDefaultOptions", "").toString().split(' ');
            QPair<QString, QString> option;
            foreach (option, query.queryItems()) {
                launchOptions << "-"+option.first << option.second;
            }
            if (func == "join") {
                launchOptions << " -lobby "+query.queryItemValue("IP");
            }
            return RVGLLauncher().launch(launchOptions);
        } else if (func == "install_asset") {
            option = query.queryItemValue("URL");
        }
    }

    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());

    QQmlApplicationEngine engine;
    engine.load("qrc:/main.qml");
    engine.rootObjects().at(0)->setProperty("availableStyles", QQuickStyle::availableStyles());
    engine.rootObjects().at(0)->setProperty("func", func);
    engine.rootObjects().at(0)->setProperty("option", option);

    return app.exec();
}
