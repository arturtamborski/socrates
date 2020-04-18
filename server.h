#ifndef STREAMSERVER_H
#define STREAMSERVER_H

#include "worker.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include <QPixmap>

class Server : public QTcpServer
{
	Q_OBJECT

public:
	explicit Server(QObject *parent = nullptr);

signals:
	void error(QTcpSocket::SocketError error);
	void update(QPixmap *frame);

protected:
	void incomingConnection(qintptr descriptor) override;

private:
	QTcpSocket m_socket;

	quint64 m_id;
	QPixmap m_frames[FPS];
	Worker m_workers[FPS];
	QThreadPool m_pool;
};

#endif // STREAMSERVER_H
