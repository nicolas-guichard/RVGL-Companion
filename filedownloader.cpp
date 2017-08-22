#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
    QObject(parent)
{
    QNetworkRequest request(url);
    file.setFileName(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/"+url.fileName());
    file.open(QIODevice::ReadWrite);
    m_reply = m_WebCtrl.get(request);
    connect(m_reply, SIGNAL (readyRead()), this, SLOT (newData()));
    connect(m_reply, SIGNAL (downloadProgress(qint64, qint64)), this, SIGNAL(progressChanged(qint64,qint64)));
    connect(m_reply, SIGNAL (finished()), this, SLOT (noMoreData()));
}

FileDownloader::~FileDownloader() {
    file.remove();
}

void FileDownloader::newData(){
    file.write(m_reply->readAll());
}

void FileDownloader::noMoreData(){
    file.close();
    m_reply->deleteLater();
    emit downloaded(&file);
}
