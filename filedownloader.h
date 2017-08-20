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

 signals:
  void downloaded(QFile*);
  void progressChanged(qint64, qint64);

 private slots:
  void fileDownloaded(QNetworkReply*);

 private:
  QNetworkAccessManager m_WebCtrl;
  QUrl m_url;
  QFile m_file;
  float m_progress;
  QNetworkReply* m_reply;
};

#endif // FILEDOWNLOADER_H
