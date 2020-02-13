#include <QStatusBar>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow)
	, m_server(new StreamServer)
	, m_video(new QVideoWidget)
	, m_frame(new QPixmap)
{
	m_ui->setupUi(this);
	//frame->loadFromData();
	//scene->addPixmap(pixmap);
	//scene->setSceneRect(pixmap.rect());
	//ui->graphicsView->setScene(scene);
	QMainWindow::statusBar()->showMessage("Ready");

	// TODO: remove it
	m_ui->pushButtonStream->click();
}

MainWindow::~MainWindow()
{
	delete m_frame;
	delete m_video;
	delete m_server;
	delete m_ui;
}

void MainWindow::on_pushButtonStream_released()
{
	if (m_server->isListening()) {
		m_ui->pushButtonStream->setText("Start listening");
		QMainWindow::statusBar()->showMessage("Ready");
		m_server->close();
		return;
	}

	if (!m_server->listen(QHostAddress::LocalHost, 2563)) {
		QMessageBox::critical(
			this,
			"Permission denied",
			"Failed to start listening");
		return;
	}

	m_ui->pushButtonStream->setText("Stop listening");
	QMainWindow::statusBar()->showMessage("Listening on port 2563...");
}
