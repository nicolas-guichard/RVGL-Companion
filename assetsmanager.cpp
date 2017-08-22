#include "assetsmanager.h"

AssetsManager::AssetsManager(QObject *parent) : QObject(parent)
{
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

QVariantList AssetsManager::progresses() {
    return m_progresses;
}

void AssetsManager::setProgress(int index, qint64 a, qint64 b) {
    if (b > 0) {
        m_progresses[index] = (100.0*a/b);
        emit progressesChanged();
    }
}

void AssetsManager::installAsset(QString URL) {
    installAsset(QUrl(URL));
}

void AssetsManager::installAsset(QUrl URL) {
    int myIndex = m_downloads.length();
    m_downloads.append(new FileDownloader(URL, this));
    m_progresses.append(0);
    emit progressesChanged();
    connect(m_downloads[myIndex], &FileDownloader::downloaded, [=](QFile* file){ installAsset(myIndex, file); });
    connect(m_downloads[myIndex], &FileDownloader::progressChanged, [=](qint64 a, qint64 b){ setProgress(myIndex, a, b); });
}

void AssetsManager::installAsset(int index, QFile* file) {
    QProcess* sevenzip = new QProcess();
    sevenzip->setWorkingDirectory(settings.value("rvglDir").toString());
#ifdef Q_OS_WIN
    QFile::copy(":/7za.exe", settings.value("rvglDir").toString()+"\\7za.exe");
    sevenzip->start(settings.value("rvglDir").toString()+"\\7za.exe x -y "+file->fileName().replace('/', '\\'));
#else
    sevenzip->start("7za x -y "+file->fileName());
#endif
    connect(sevenzip, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](){ sevenzip->deleteLater(); m_downloads[index]->deleteLater(); });
}
