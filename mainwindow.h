#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

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
    void onError(QString message);
    void onFrame(QPixmap *frame);
    void on_pushButtonStart_released();

private:
    void start();
    void stop();

private:
    Ui::MainWindow *m_ui;
    Thread m_thread;
};

#endif // MAINWINDOW_H
