#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
    , m_id(0)
{
    m_pool.setMaxThreadCount(FPS);

    for (auto &worker : m_workers) {
        worker.setAutoDelete(false);
        connect(&worker,
                SIGNAL(finished(quint64, QPixmap *)),
                parent,
                SLOT(onUpdate(quint64, QPixmap *)), Qt::DirectConnection);
    }
}

void Server::incomingConnection(qintptr descriptor)
{
    if (!m_socket.setSocketDescriptor(descriptor)) {
        qDebug() << "Socket error" << descriptor;
        qDebug() << m_socket.errorString();
        return;
    }

    if (!m_socket.waitForReadyRead()) {
        qDebug() << "Socket timeout" << descriptor;
        return;
    }

    //nanosleep((const struct timespec[]){{0, 10 * 1000000L}}, NULL);

    auto frame = new QPixmap;
    auto data = m_socket.readAll();
    frame->loadFromData((uchar *)data.data(), data.size());
    m_socket.close();

    m_id++;
    auto worker = &m_workers[m_id % FPS];
    worker->m_id = m_id;
    worker->m_data = frame;
    m_pool.start(worker);
}
