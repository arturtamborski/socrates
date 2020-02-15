#include "mainwindow.h"
#include "streamthread.h"

StreamThread::StreamThread(QObject *parent)
	: QThread(parent)
{
	connect(this,		&QThread::started,
		this,		&StreamThread::onStart);
	connect(this,		&QThread::finished,
		this,		&StreamThread::onFinish);
	connect(this,		&QThread::finished,
		&m_transcoder,	&StreamTranscoder::finish);
	connect(&m_server,	SIGNAL(error(QTcpSocket::SocketError)),
		parent,		SLOT(onServerError(QTcpSocket::SocketError)));
}

void StreamThread::setUrl(QString &&url)
{
	m_url = url;
}

void StreamThread::onStart()
{
	if (!m_server.listen(QHostAddress::LocalHost, 2563)) {
		qDebug() << "Error listening!";
		emit error(m_server.serverError());
		exit();
	}
	qDebug() << "Listening...";

	if (!m_transcoder.start(m_url, FPS, 1)) {
		qDebug() << "Error transcoding!";
		exit();
	}
	qDebug() << "Transcoding...";
}

void StreamThread::onFinish()
{
	m_server.close();
	qDebug() << "Closed!";
}
