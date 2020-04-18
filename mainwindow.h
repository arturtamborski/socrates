#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
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
	void onUpdate(quint64 id, QPixmap *frame);
	void on_pushButtonStream_released();

private:
	void startThread();
	void stopThread();

private:
	Ui::MainWindow *m_ui;
	Thread *m_thread;
	QGraphicsScene m_scene;
	QGraphicsPixmapItem m_frame;
};

#endif // MAINWINDOW_H
