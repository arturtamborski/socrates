#include <QStatusBar>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "unistd.h"

#define READY   " Ready."
#define RUNNING " Running..."

#define START   " Start"
#define STOP    " Stop"

#define LOCAL   "./stream.mp4"
#define REMOTE  \
    "https://videos3.earthcam.com/fecnetwork/" \
    "AbbeyRoadHD1.flv/chunklist_w1632720834.m3u8"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_thread(new Thread(this))
    , m_video(new QVideoWidget)
    , m_player(new QMediaPlayer)
{
    m_ui->setupUi(this);
    m_scene.addItem(&m_frame);
    m_ui->graphicsView->setScene(&m_scene);
    m_ui->graphicsView->show();
    m_ui->statusBar->showMessage(READY);

//    m_player.setMedia();
    m_player.setVideoOutput(&m_video);
    m_video.show();

//    this.setCentralWidget(m_video);
    m_player.play();

    m_ui->lineEditAddress->setText(LOCAL);
//    m_ui->lineEditAddress->setText(REMOTE);

    connect(m_thread, &Thread::error, this, &MainWindow::onThreadError);
}

MainWindow::~MainWindow()
{
    delete m_thread;
    delete m_ui;
}

void MainWindow::onThreadError(QTcpSocket::SocketError error)
{
    stopThread();

    QMessageBox::critical(
        this,
        "Failed to start listening",
        QVariant::fromValue(error).toString());
}

void MainWindow::onServerError(QTcpSocket::SocketError error)
{
    stopThread();

    QMessageBox::critical(
        this,
        "Something went wrong with the socket",
        QVariant::fromValue(error).toString());
}

void MainWindow::onUpdate(quint64 id, QPixmap *frame)
{
    qDebug() << id << "updating screen...";

    //usleep(100 * 1000);
    m_frame.setPixmap(*frame);
    delete frame;
}

void MainWindow::on_pushButtonStart_released()
{
    m_thread->isRunning()
        ? stopThread()
        : startThread();
}

void MainWindow::startThread()
{
    m_ui->statusBar->showMessage(RUNNING);
    m_ui->pushButtonStart->setText(STOP);
    m_ui->lineEditAddress->setFrame(false);
    m_ui->lineEditAddress->setReadOnly(true);

    m_thread->setUrl(m_ui->lineEditAddress->text());
    m_thread->start();
}

void MainWindow::stopThread()
{
    m_ui->statusBar->showMessage(READY);
    m_ui->pushButtonStart->setText(START);
    m_ui->lineEditAddress->setFrame(true);
    m_ui->lineEditAddress->setReadOnly(false);

    m_thread->exit();
}
