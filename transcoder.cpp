#include <QDebug>
#include "transcoder.h"
#include "strings.h"

Transcoder::Transcoder(QObject *parent)
    : QObject(parent)
{
    connect(&m_process,	&QProcess::errorOccurred, this, &Transcoder::onError);
//    connect(&m_process,	&QProcess::finished,      this, &Transcoder::onFinish);
    connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this,         SLOT(onFinish(int, QProcess::ExitStatus)));
}

void Transcoder::start(QString &url)
{
    qDebug() << STR_TRANSCODER_STARTING;

    m_args.clear();

    // source
    m_args << "-i" << url;

    // logging
    m_args << "-loglevel" << "info";

    // scale
    m_args << "-s" << "960x540";

    // frame limit
    m_args << "-vf" << QString("fps=%1").arg(FPS);

    // overwrite output
    m_args << "-y";

    // remove audio track
    m_args << "-an";

    // go fast
    m_args << "-preset" << "ultrafast";
    m_args << "-tune" << "zerolatency";

    // drop frames
    m_args << "-vsync" << "drop";

    // output format, overwrite
    //m_args << "-f" << "mpegts";
    m_args << "-f" << "image2";
    m_args << "-update" << "1";

    // limit frames
    m_args << "-vframes" << "10";

    // output destination
    m_args << QString("tcp://127.0.0.1:%1").arg(PORT);

    m_process.start("ffmpeg", m_args);
    m_process.waitForStarted();
}

void Transcoder::finish()
{
    m_process.terminate();
    m_process.waitForFinished();
}

void Transcoder::printProcessOutput()
{
    auto lines = m_process.readAllStandardError().split('\n');
    for (auto &line : lines) {
        qDebug() << '\t' << line;
    }
}

void Transcoder::onFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    auto value = QVariant::fromValue(exitStatus).toString();
    qDebug() << STR_TRANSCODER << exitCode << " " << value;
    printProcessOutput();
}

void Transcoder::onError(QProcess::ProcessError e)
{
    auto value = QVariant::fromValue(e).toString();
    auto message = STR_TRANSCODER_FAILED " " + value;
    printProcessOutput();
    emit error(message);
}
