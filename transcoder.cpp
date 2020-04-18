#include <QDebug>
#include "transcoder.h"

Transcoder::Transcoder(QObject *parent)
	: QObject(parent)
{
	connect(&m_process,	&QProcess::started,
		this,		&Transcoder::onStart);
	connect(&m_process,	SIGNAL(finished(int)),
		this,		SLOT(onFinish(int)));
}

Transcoder::~Transcoder()
{
	m_process.kill();
}

bool Transcoder::start(QString &url, int fps, int speed)
{
	m_args.clear();

	// source
	m_args << "-i" << url;

	// logging
	m_args << "-loglevel" << "info";

	// scale
	m_args << "-s" << "960x540";

	// frame limit
	m_args << "-vf" << QString("fps=%1").arg(fps);

	// slow down
	m_args << "-filter:v" << QString("setpts=%1*PTS").arg(speed);

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
	//m_args << "-vframes" << "10";

	// output destination
	m_args << "tcp://127.0.0.1:2563";

	m_process.start("ffmpeg", m_args);
	if (!m_process.waitForStarted()) {
		qDebug() << "Timeout! failed to start";
		return false;
	}

	return true;
}

void Transcoder::finish()
{
	qDebug() << "stopping transcoder!";
	m_process.kill();
}

bool Transcoder::isRunning()
{
	return m_process.state() == m_process.Running;
}

void Transcoder::onStart()
{
	qDebug() << "StreamTranscoder: started!";
	qDebug() << "Arguments passed:" << m_args;

}

void Transcoder::onFinish(int code)
{
	qDebug() << "StreamTranscoder: finished!" << code;
	qDebug() << "Exit code:" << m_process.exitStatus();
}
