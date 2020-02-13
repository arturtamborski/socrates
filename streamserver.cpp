#include "streamthread.h"
#include "streamserver.h"

StreamServer::StreamServer(QObject *parent)
	: QTcpServer(parent)
{
}

void StreamServer::serve()
{
	if (!this->listen(QHostAddress::LocalHost, 2563)) {
		qDebug() << "nie działa";
	}
}

void StreamServer::incomingConnection(qintptr descriptor)
{
	auto *thread  = new StreamThread(this, descriptor);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();
}
