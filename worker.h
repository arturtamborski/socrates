#ifndef STREAMWORKER_H
#define STREAMWORKER_H

#include <QRunnable>
#include <QTcpSocket>

class Worker : public QRunnable
{
public:
	Worker();

protected:
	void run() override;

public:
	qintptr m_descriptor;
	QByteArray *m_data;
	int m_id;
};

#endif // STREAMWORKER_H
