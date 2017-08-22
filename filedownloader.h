#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QStandardPaths>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl, QObject *parent = 0);
    virtual ~FileDownloader();
    QFile file;

signals:
    void downloaded(QFile*);
    void progressChanged(qint64, qint64);

private slots:
    void noMoreData();
    void newData();

private:
    QNetworkAccessManager m_WebCtrl;
    QString m_filename;
    QNetworkReply* m_reply;
};

#endif // FILEDOWNLOADER_H
