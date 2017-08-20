#ifndef RVGLLAUNCHER_H
#define RVGLLAUNCHER_H

#include <QObject>
#include <QSettings>
#include <QProcess>

class RVGLLauncher : public QObject
{
    Q_OBJECT
public:
    explicit RVGLLauncher(QObject *parent = nullptr);
    Q_INVOKABLE int launch(QStringList);

signals:

public slots:

private:
    QSettings settings;
};

#endif // RVGLLAUNCHER_H
