#include <QVariant>
#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "server.h"
#include "strings.h"
#include "worker.h"

#define FRAME_BUFFER_SIZE	(FPS * 60 * 4)
#define FRAME_TICK_LENGTH	(1000 / FPS)
#define FRAME_DETECT_TIME   (1600)
#define THREAD_POOL_SIZE    ((FRAME_DETECT_TIME / FRAME_TICK_LENGTH) + 1)

Server::Server(QObject *parent)
    : QTcpServer(parent)
    , m_nets(THREAD_POOL_SIZE)
    , m_frames(FRAME_BUFFER_SIZE)
    , m_workers(THREAD_POOL_SIZE)
{
    QThreadPool::globalInstance()->setMaxThreadCount(THREAD_POOL_SIZE);
}

void Server::start()
{
    qDebug() << STR_SERVER_STARTING;

    // try to bind, quit on failure
    if (!listen(QHostAddress::LocalHost, PORT)) {
        emit error(STR_SERVER + errorString());
    }

    // prepare workers and networks
    for (int i = 0; i < m_workers.size(); i++) {
        auto w = m_workers[i] = new Worker();
        auto n = &m_nets[i];
        w->setAutoDelete(false);
        connect(w,		&Worker::finished,
                this,	&Server::onProcessedFrame,
                Qt::DirectConnection);

        // each worker has it's own network to work with
        *n = cv::dnn::readNetFromDarknet("yolov4.cfg", "yolov4.weights");
        n->setPreferableBackend(DNN_BACKEND_OPENCV);
        n->setPreferableTarget(DNN_TARGET_CPU);
        w->m_net = n;
    }

    m_counter = 0;
}

void Server::onProcessedFrame(quint64 id)
{
    auto back_pressure = (m_counter - id) * FRAME_TICK_LENGTH;
    auto f = &m_frames[id % FRAME_BUFFER_SIZE];

    qDebug() << STR_SERVER << "received processed frame" << id
             << "\tback pressure: " << back_pressure << "ms";

    emit frame(f);
}

void Server::incomingConnection(qintptr descriptor)
{
    QElapsedTimer timer;
    timer.start();

    // validate connection
    if (!m_socket.setSocketDescriptor(descriptor)) {
        emit error(STR_SOCKET + m_socket.errorString());
        return;
    }

    if (!m_socket.waitForReadyRead()) {
        qDebug() << STR_SOCKET_TIMEOUT << descriptor;
        return;
    }

    // load received frame
    QByteArray raw(m_socket.readAll());
    m_socket.close();

    // prepare frame for validation
    auto &f = m_frames[m_counter % FRAME_BUFFER_SIZE];
    auto jpeg_tail = raw.right(2).toHex().toStdString();
    auto jpg_ok = f.loadFromData(raw, "JPG");

    // todo: simplify QByteArray -> QPixmap -> QImage -> cv::Mat
    // cv::Mat _mat = cv::Mat(320+320/2, 560, CV_8UC1, raw.data());

    // validate frame
    if (jpeg_tail != "ffd9" || !jpg_ok) {
        qDebug() << STR_FRAME_INVALID;
        return;
    }

    // prepare worker
    auto w = m_workers[m_counter % THREAD_POOL_SIZE];
    w->m_id = m_counter;
    w->m_frame = &f;

    // limit frame rate
    quint64 elapsed = static_cast<quint64>(timer.elapsed());
    qint64 free_time = static_cast<qint64>(FRAME_TICK_LENGTH - elapsed - 5);
    free_time = free_time > 0 ? free_time : 0;
    qDebug() << STR_SERVER "Frame" << m_counter << "took" << elapsed
             << "ms to receive, sleeping for" << free_time << "ms";
    QThread::msleep(static_cast<quint64>(free_time));

    // delegate work
    QThreadPool::globalInstance()->start(w);
    m_counter++;
}

// TODO: czasami sie ksztusi i dekoduje 15ms zamiast 5ciu
// Worker to porażka, 0.01FPS i gorzej, nie mam pojęcia dlaczego
