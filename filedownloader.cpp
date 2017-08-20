#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
    QObject(parent)
{
    m_WebCtrl.setRedirectPolicy(QNetworkRequest::ManualRedirectPolicy);
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    m_url = url;
    m_reply = m_WebCtrl.get(request);
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(progressChanged(qint64,qint64)));
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    if (pReply->hasRawHeader("Location")) {
        m_url = pReply->rawHeader("Location");
        QNetworkRequest request(m_url);
        m_reply = m_WebCtrl.get(request);
        connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(progressChanged(qint64,qint64)));
    } else {
        QByteArray data = pReply->readAll();
        QString fileName = m_url.fileName();
        m_file.setFileName(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/"+fileName);
        m_file.open(QIODevice::ReadWrite);
        m_file.write(data);
        pReply->deleteLater();
        emit downloaded(&m_file);
    }
}
