#include "worker.h"

Worker::Worker()
{
}

void Worker::run()
{
	qDebug() << "Processing frame " << m_id;
	emit finished(m_id, m_data);
}
