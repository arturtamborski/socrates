#include <QTcpSocket>
#include "streamserver.h"
#include "streamworker.h"

StreamServer::StreamServer(QObject *parent)
	: QTcpServer(parent)
	, m_id(0)
{
	// 1 min of cache
	//m_frames.setCapacity(FPS * 60);
	m_pool.setMaxThreadCount(FPS);
}

void StreamServer::incomingConnection(qintptr descriptor)
{
	qDebug() << "Incomming connection" << descriptor;

	auto worker = new StreamWorker();
	worker->setAutoDelete(true);

	m_id++;
	worker->m_id = m_id;
	worker->m_descriptor = descriptor;
	worker->m_data = &m_frames[m_id % FPS];

	m_pool.start(worker);
}
