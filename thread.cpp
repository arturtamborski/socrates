#include "thread.h"
#include "server.h"
#include "transcoder.h"
#include "mainwindow.h"

Thread::Thread(QObject *parent)
    : QThread(parent)
{
}

void Thread::setUrl(QString &url)
{
    m_url = url;
}

void Thread::run()
{
    Server server;
    Transcoder transcoder;

    connect(&transcoder, &Transcoder::error, this, &Thread::error);
    connect(&server,         &Server::error, this, &Thread::error);
    connect(&server,         &Server::frame, this, &Thread::frame);

    server.start();
    transcoder.start(m_url);

    exec();
}
