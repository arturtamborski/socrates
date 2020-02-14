#include <QStatusBar>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define READY	"Ready."
#define RUNNING	"Running..."
#define START	"Start"
#define STOP	"Stop"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow)
	, m_thread(new StreamThread(this))
{
	m_ui->setupUi(this);
	QMainWindow::statusBar()->showMessage(READY);

	QString stream =
		"https://videos3.earthcam.com/fecnetwork"
		"/AbbeyRoadHD1.flv/chunklist_w1632720834.m3u8";

	m_ui->lineEditAddress->setText(stream);
	m_ui->lineEditAddress->setText("./stream.mp4");

	// TODO: remove it
	//m_ui->pushButtonStream->click();

	connect(m_thread,	&StreamThread::error,
		this,		&MainWindow::onThreadError);
}

MainWindow::~MainWindow()
{
	delete m_thread;
	delete m_ui;
}

void MainWindow::onThreadError(QTcpSocket::SocketError error)
{
	m_ui->pushButtonStream->setText(START);
	QMainWindow::statusBar()->showMessage(READY);

	if (m_thread->isRunning()) {
		m_thread->exit();
	}

	QMessageBox::critical(
		this,
		"Failed to start listening",
		QVariant::fromValue(error).toString());
}

void MainWindow::onServerError(QTcpSocket::SocketError error)
{
	QMessageBox::critical(
		this,
		"Something went wrong with the socket",
		QVariant::fromValue(error).toString());
}

void MainWindow::on_pushButtonStream_released()
{
	if (!m_thread->isRunning()) {
		m_ui->pushButtonStream->setText(STOP);
		QMainWindow::statusBar()->showMessage(RUNNING);
		m_thread->setUrl(m_ui->lineEditAddress->text());
		m_thread->start();
	} else {
		m_ui->pushButtonStream->setText(START);
		QMainWindow::statusBar()->showMessage(READY);
		m_thread->exit();
	}
}
