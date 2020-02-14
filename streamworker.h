#ifndef STREAMWORKER_H
#define STREAMWORKER_H

#include <QRunnable>
#include <QTcpSocket>

class StreamWorker : public QRunnable
{
public:
	StreamWorker();

protected:
	void run() override;

public:
	qintptr m_descriptor;
	QByteArray *m_data;
	int m_id;
};

#endif // STREAMWORKER_H
