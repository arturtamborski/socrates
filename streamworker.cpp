#include "streamworker.h"

StreamWorker::StreamWorker()
	: m_data(nullptr)
{
}

void StreamWorker::run()
{
	QTcpSocket socket;

	if (!socket.setSocketDescriptor(m_descriptor)) {
		qDebug() << "Socket error" << m_descriptor;
		return;
	}

	*m_data = socket.readAll();
	qDebug() << m_id << "Received" << m_data->size() << "bytes!";
	socket.close();
}
