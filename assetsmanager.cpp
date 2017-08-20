#include "assetsmanager.h"

AssetsManager::AssetsManager(QObject *parent) : QObject(parent)
{
    m_progress = 0;
    m_downloads = 0;
    m_running_downloads = 0;
}

int AssetsManager::progress() {
    return m_progress;
}

int AssetsManager::runningDownloads() {
    return m_running_downloads;
}

#ifndef Q_OS_WIN
void AssetsManager::fixCases() {
    fixCases(QDir(settings.value("rvglDir").toString()));
}

void AssetsManager::fixCases(QDir directory) {
    directory.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    QFileInfoList list = directory.entryInfoList();
    QString fileName;
    for (int i = 0; i < list.size(); ++i) {
        if (list.at(i).isDir()) {
            fixCases(list.at(i).absoluteFilePath());
        }
        fileName = list.at(i).fileName();
        if (fileName.toLower() != fileName) {
            QFileInfo dest(directory.filePath(fileName.toLower()));
            if (dest.exists()){
                if (dest.isDir()) {
                    QDir(dest.absoluteFilePath()).removeRecursively();
                } else {
                    QFile(dest.absoluteFilePath()).remove();
                }
            }
            directory.rename(fileName, fileName.toLower());
        }
    }
}
#endif

void AssetsManager::setProgress(qint64 a, qint64 b) {
    if (b > 0) {
        m_progress = 100.0*a/b;
        emit progressChanged();
    }
}

void AssetsManager::setOverallProgress() {
    m_progress = 100-100.0*m_running_downloads/m_downloads;
    emit progressChanged();
}

void AssetsManager::downloadFinished() {
    m_running_downloads -= 1;
    setOverallProgress();
    emit runningDownloadsChanged();
}

void AssetsManager::installAsset(QString URL) {
    installAsset(QUrl(URL));
}

FileDownloader* AssetsManager::installAssetRaw(QUrl URL) {
    m_running_downloads += 1;
    emit runningDownloadsChanged();
    FileDownloader* download = new FileDownloader(URL, this);
    connect(download, SIGNAL (downloaded(QFile*)), this, SLOT (downloadFinished()));
    connect(download, SIGNAL (downloaded(QFile*)), this, SLOT (installAsset(QFile*)));
    return download;
}

void AssetsManager::installAsset(QUrl URL) {
    FileDownloader* download = installAssetRaw(URL);
    connect(download, SIGNAL (progressChanged(qint64, qint64)), this, SLOT (setProgress(qint64, qint64)));
}

void AssetsManager::installAsset(QFile* file) {
    file->close();
    QProcess sevenzip;
    sevenzip.setWorkingDirectory(settings.value("rvglDir").toString());
    #ifdef Q_OS_WIN
        QFile::copy(":/7za.exe", settings.value("rvglDir").toString()+"\\7za.exe");
        sevenzip.start(settings.value("rvglDir").toString()+"\\7za.exe x -y "+file->fileName().replace('/', '\\'));
    #else
        sevenzip.start("7za x -y "+file->fileName());
    #endif
    if (sevenzip.waitForFinished()) {
        #ifdef Q_OS_LINUX
            fixCases();
        #endif
        file->remove();
    }
}

void AssetsManager::installMonth() {
    m_progress = 0;
    emit progressChanged();
    FileDownloader* download = new FileDownloader(QUrl("https://www.revoltrace.net/month_tracks.php"), this);
    connect(download, SIGNAL (downloaded(QFile*)), this, SLOT (installMonth(QFile*)));
}

void AssetsManager::installMonth(QFile* file) {
    QList<QUrl> links;
    QUrl trackUrl;
    QString line;

    file->reset();

    while(!file->atEnd()) {
        line = file->readLine();
        if (line.contains("class=\"downloadTrack\"")) {
            links.append(QUrl(line.split('"')[1]));
        }
    }

    m_downloads = links.length();

    for (int i=0; i < links.length(); i++) {
        trackUrl = links[i];
        if (trackUrl.host() == "revoltzone.net") {
            QString trackID = trackUrl.path().split('/')[2];
            trackUrl.setUrl("http://revoltzone.net/sitescripts/dload.php?id="+trackID);
        }
        installAssetRaw(trackUrl);
    }
}
