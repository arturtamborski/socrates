#include "mainwindow.h"
#include "streamthread.h"

StreamThread::StreamThread(QObject *parent)
	: QThread(parent)
{
	connect(&m_server, 	SIGNAL(error(QTcpSocket::SocketError)),
		parent,		SLOT(onServerError(QTcpSocket::SocketError)));
	connect(this,		&QThread::started,
		this,		&StreamThread::onStart);
	connect(this,		&QThread::finished,
		this,		&StreamThread::onFinish);
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

	if (!m_transcoder.start(m_url, FPS)) {
		emit error(m_server.serverError());
	}
	qDebug() << "Transcoding...";
}

void StreamThread::onFinish()
{
	m_server.close();
	qDebug() << "Closed!";
}
