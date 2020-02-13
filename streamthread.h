#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H

#include <QThread>
#include <QTcpSocket>

class StreamThread : public QThread
{
	Q_OBJECT

public:
	explicit StreamThread(QObject *parent, qintptr descriptor);
	~StreamThread();

	void run() override;

signals:
	void error(QTcpSocket::SocketError error);

public slots:
	void connected();
	void disconnected();

private:
	QTcpSocket *m_socket;
	qintptr m_descriptor;
};

#endif // STREAMTHREAD_H
