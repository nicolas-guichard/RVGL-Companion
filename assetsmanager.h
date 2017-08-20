#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <QObject>
#include <QUrl>
#include <QSettings>
#include <QProcess>
#include "filedownloader.h"

#ifndef Q_OS_WIN
#include <QDir>
#endif

class AssetsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int runningDownloads READ runningDownloads NOTIFY runningDownloadsChanged)

public:
    explicit AssetsManager(QObject *parent = nullptr);
    Q_INVOKABLE void installAsset(QString);
    Q_INVOKABLE void installMonth();
#ifndef Q_OS_WIN
    Q_INVOKABLE void fixCases();
#endif
    int progress();
    int runningDownloads();

signals:
    void progressChanged();
    void runningDownloadsChanged();

private slots:
    void installAsset(QFile*);
    void installMonth(QFile*);
    void setProgress(qint64, qint64);
    void setOverallProgress();
    void downloadFinished();

private:
#ifndef Q_OS_WIN
    void fixCases(QDir);
#endif
    void installAsset(QUrl);
    FileDownloader* installAssetRaw(QUrl);
    QSettings settings;
    int m_progress;
    int m_running_downloads;
    int m_downloads;
};

#endif // ASSETSMANAGER_H
