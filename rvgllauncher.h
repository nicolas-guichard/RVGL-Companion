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
    Q_INVOKABLE void launch(QString, QStringList);

signals:

public slots:

private:
};

#endif // RVGLLAUNCHER_H
