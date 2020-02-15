#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QContiguousCache>
#include "server.h"
#include "transcoder.h"

class Thread : public QThread
{
	Q_OBJECT

public:
	explicit Thread(QObject *parent = nullptr);
	void setUrl(QString &&url);

signals:
	void error(QTcpSocket::SocketError error);

private slots:
	void onStart();
	void onFinish();

private:
	Transcoder m_transcoder;
	Server m_server;
	QString m_url;
};

#endif // STREAMTHREAD_H