#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <QObject>
#include <QProcess>

class Transcoder : public QObject
{
    Q_OBJECT

public:
    Transcoder(QObject *parent = nullptr);

    void start(QString &url);
    void finish();

signals:
    void error(QString message);

private:
    void printProcessOutput(void);

public slots:
    void onError(QProcess::ProcessError error);
    void onFinish(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess m_process;
    QStringList m_args;
};

#endif // TRANSCODER_H
