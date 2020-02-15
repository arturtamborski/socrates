#ifndef STREAMTRANSCODER_H
#define STREAMTRANSCODER_H

#include <QObject>
#include <QProcess>

class StreamTranscoder : public QObject
{
	Q_OBJECT

public:
	StreamTranscoder(QObject *parent = nullptr);
	~StreamTranscoder();

	bool start(QString &url, int fps, int speed = 1);
	void finish();
	bool isRunning();

public slots:
	void onStart();
	void onReadyRead();
	void onFinish(int code);

private:
	QProcess m_process;
	QStringList m_args;
};

#endif // STREAMTRANSCODER_H
