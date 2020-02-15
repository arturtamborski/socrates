#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include "thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void onThreadError(QTcpSocket::SocketError error);
	void onServerError(QTcpSocket::SocketError error);
	void on_pushButtonStream_released();

private:
	Ui::MainWindow *m_ui;
	Thread *m_thread;
};

#endif // MAINWINDOW_H
