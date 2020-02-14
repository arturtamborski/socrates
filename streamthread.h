#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QContiguousCache>
#include "streamserver.h"
#include "streamtranscoder.h"

class StreamThread : public QThread
{
	Q_OBJECT

public:
	explicit StreamThread(QObject *parent = nullptr);
	void setUrl(QString &&url);

signals:
	void error(QTcpSocket::SocketError error);

private slots:
	void onStart();
	void onFinish();

private:
	StreamTranscoder m_transcoder;
	StreamServer m_server;
	QString m_url;
};

#endif // STREAMTHREAD_H
