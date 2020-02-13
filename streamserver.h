#ifndef STREAMSERVER_H
#define STREAMSERVER_H

#include <QTcpServer>

class StreamServer : public QTcpServer
{
	Q_OBJECT

public:
	explicit StreamServer(QObject *parent = nullptr);

	void serve();

signals:

public slots:

protected:
	void incomingConnection(qintptr descriptor) override;
};

#endif // STREAMSERVER_H
