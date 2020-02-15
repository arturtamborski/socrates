#ifndef STREAMSERVER_H
#define STREAMSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QThreadPool>

class Server : public QTcpServer
{
	Q_OBJECT

public:
	explicit Server(QObject *parent = nullptr);

signals:
	void error(QTcpSocket::SocketError error);

protected:
	void incomingConnection(qintptr descriptor) override;

private:
	int id();

private:
	unsigned long long int m_id;
	QByteArray m_frames[24];
	QThreadPool m_pool;
};

#endif // STREAMSERVER_H
