#include "worker.h"

Worker::Worker()
	: m_data(nullptr)
{
}

void Worker::run()
{
	QTcpSocket socket;

	if (!socket.setSocketDescriptor(m_descriptor)) {
		qDebug() << "Socket error" << m_descriptor;
		qDebug() << socket.errorString();
		return;
	}

	if (!socket.waitForReadyRead()) {
		qDebug() << "Socket timeout" << m_descriptor;
		return;
	}

	*m_data = socket.readAll();
	qDebug() << m_id << "Received" << m_data->size() << "bytes!";
	socket.close();
}
