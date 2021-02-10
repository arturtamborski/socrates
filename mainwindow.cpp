#include <QMessageBox>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "strings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_thread(this)
{
    m_ui->setupUi(this);
    m_ui->statusBar->showMessage(STR_READY);
    m_ui->lineEditAddress->setText(STR_LOCAL);

    connect(&m_thread, &Thread::error, this, &MainWindow::onError, Qt::DirectConnection);
    connect(&m_thread, &Thread::frame, this, &MainWindow::onFrame, Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    stop();
    delete m_ui;
}

void MainWindow::onError(QString message)
{
    stop();
    QMessageBox::critical(this, STR_ERROR, message);
}

void MainWindow::onFrame(QPixmap *frame)
{
    m_ui->labelDisplay->setPixmap(*frame);
}

void MainWindow::on_pushButtonStart_released()
{
    m_thread.isRunning() ? stop() : start();
}

void MainWindow::start()
{
    if (m_thread.isRunning())
        return;

    m_ui->statusBar->showMessage(STR_RUNNING);
    m_ui->pushButtonStart->setText(STR_STOP);
    m_ui->lineEditAddress->setFrame(false);
    m_ui->lineEditAddress->setReadOnly(true);

    auto url = m_ui->lineEditAddress->text().trimmed();
    m_thread.setUrl(url);
    m_thread.start();
}

void MainWindow::stop()
{
    if (!m_thread.isRunning())
        return;

    m_ui->statusBar->showMessage(STR_READY);
    m_ui->pushButtonStart->setText(STR_START);
    m_ui->lineEditAddress->setFrame(true);
    m_ui->lineEditAddress->setReadOnly(false);

    m_thread.quit();
    m_thread.wait(100);
    m_thread.requestInterruption();
}
