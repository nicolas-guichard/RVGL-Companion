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
    Q_PROPERTY(QVariantList progresses READ progresses NOTIFY progressesChanged)

public:
    explicit AssetsManager(QObject *parent = nullptr);
    Q_INVOKABLE void installAsset(QString);
#ifndef Q_OS_WIN
    Q_INVOKABLE void fixCases();
#endif
    QVariantList progresses();

signals:
    void progressesChanged();

private slots:
    void installAsset(int, QFile*);

private:
#ifndef Q_OS_WIN
    void fixCases(QDir);
#endif
    void setProgress(int, qint64, qint64);
    void installAsset(QUrl);
    QSettings settings;
    QVariantList m_progresses;
    QList<FileDownloader*> m_downloads;
};

#endif // ASSETSMANAGER_H
