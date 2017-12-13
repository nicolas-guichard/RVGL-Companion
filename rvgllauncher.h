#ifndef RVGLLAUNCHER_H
#define RVGLLAUNCHER_H

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QThread>
#include "portmanager.h"

class RVGLLauncher : public QObject
{
    Q_OBJECT
public:
    explicit RVGLLauncher(QObject *parent = nullptr);
    virtual ~RVGLLauncher();
    Q_INVOKABLE void launch(QString, QStringList);
    QProcess* launchRaw(QString, QStringList);

signals:
    void discover();
    void openPort();
    void closePort();

private:
    QThread portManagerThread;
};

#endif // RVGLLAUNCHER_H
