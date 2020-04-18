#ifndef STREAMTRANSCODER_H
#define STREAMTRANSCODER_H

#include <QObject>
#include <QProcess>

class Transcoder : public QObject
{
    Q_OBJECT

public:
    Transcoder(QObject *parent = nullptr);
    ~Transcoder();

    bool start(QString &url, int fps, int speed = 1);
    void finish();
    bool isRunning();

public slots:
    void onStart();
    void onFinish(int code);

private:
    QProcess m_process;
    QStringList m_args;
};

#endif // STREAMTRANSCODER_H
