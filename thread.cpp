#include "mainwindow.h"
#include "thread.h"

Thread::Thread(QObject *parent)
	: QThread(parent)
	, m_server(new Server(parent))
{
	connect(this, &QThread::started, this, &Thread::onStart);
	connect(this, &QThread::finished, this, &Thread::onFinish);
	connect(this, &QThread::finished, &m_transcoder, &Transcoder::finish);
	connect(m_server,	SIGNAL(error(QTcpSocket::SocketError)),
		parent,		SLOT(onServerError(QTcpSocket::SocketError)));
}

Thread::~Thread()
{
	delete m_server;
}

void Thread::setUrl(QString &&url)
{
	m_url = url;
}

void Thread::onStart()
{
	if (!m_server->listen(QHostAddress::LocalHost, 2563)) {
		qDebug() << "Error listening!";
		emit error(m_server->serverError());
		exit();
	}
	qDebug() << "Listening...";

	if (!m_transcoder.start(m_url, FPS, 1)) {
		qDebug() << "Error transcoding!";
		exit();
	}
	qDebug() << "Transcoding...";
}

void Thread::onFinish()
{
	m_server->close();
	qDebug() << "Thread stopped!";
}
