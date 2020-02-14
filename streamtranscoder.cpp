#include <QDebug>
#include "streamtranscoder.h"

StreamTranscoder::StreamTranscoder(QObject *parent)
	: QObject(parent)
{
	connect(&m_process,	&QProcess::readyRead,
		this,		&StreamTranscoder::onReadyRead);
	connect(&m_process,	&QProcess::started,
		this,		&StreamTranscoder::onStart);
	connect(&m_process,	SIGNAL(finished(int)),
		this,		SLOT(onFinish(int)));
}

StreamTranscoder::~StreamTranscoder()
{
	m_process.kill();
}

bool StreamTranscoder::start(QString &url, int fps)
{
	m_args.clear();

	// source
	m_args << "-i" << QString("'%1'").arg(url);

	// logging
	m_args << "-loglevel" << "info";

	// scale
	m_args << "-s" << "960x540";

	// frame limit
	m_args << "-vf" << QString("fps=%1").arg(fps);

	// overwrite output
	m_args << "-y";

	// remove audio track
	m_args << "-an";

	// go fast
	m_args << "-preset" << "ultrafast";
	m_args << "-tune" << "zerolatency";

	// drop frames
	m_args << "-vsync" << "drop";

	// output format, overwrite
	//m_args << "-f" << "mpegts";
	m_args << "-f" << "image2";
	m_args << "-update" << "1";

	// limit frames
	m_args << "-vframes" << "10";

	// output destination
	m_args << "tcp://127.0.0.1:2563";

	m_process.start("/home/artur/code/self/temp/a.out", m_args);
	if (!m_process.waitForStarted()) {
		qDebug() << "Timeout! failed to start";
		return false;
	}

	return true;
}

void StreamTranscoder::finish()
{
	m_process.kill();
}

bool StreamTranscoder::isRunning()
{
	return m_process.state() == m_process.Running;
}

void StreamTranscoder::onStart()
{
	qDebug() << "StreamTranscoder: started!";
	qDebug() << "Arguments passed:" << m_args;

}

void StreamTranscoder::onReadyRead()
{
	qDebug() << "OUT:" << m_process.readAllStandardOutput();
	qDebug() << "ERR:" << m_process.readAllStandardError();
}

void StreamTranscoder::onFinish(int code)
{
	qDebug() << "StreamTranscoder: finished!" << code;
	qDebug() << "Exit code:" << m_process.exitStatus();
}
