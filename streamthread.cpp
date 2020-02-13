#include <QFile>
#include "streamthread.h"

StreamThread::StreamThread(QObject *parent, qintptr descriptor)
	: QThread(parent)
	, m_socket(new QTcpSocket)
	, m_descriptor(descriptor)
{
}

StreamThread::~StreamThread()
{
	delete m_socket;
}

void StreamThread::run()
{
	if (!m_socket->setSocketDescriptor(m_descriptor)) {
		emit error(m_socket->error());
		return;
	}

	auto d = Qt::DirectConnection;
	connect(m_socket, SIGNAL(connected()), this, SLOT(connected()), d);
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

	exec(); // loop
}

void StreamThread::connected()
{
	auto data = m_socket->readAll();

	QFile file("./file.jpg");
	file.open(QFile::WriteOnly);
	file.write(data);
	file.flush();
	file.close();
}

void StreamThread::disconnected()
{
	m_socket->deleteLater();
	exit(0);
}
