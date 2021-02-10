#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QRunnable>
#include <QPixmap>
#include <QDebug>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

using namespace cv::dnn::dnn4_v20200310;

class Worker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Worker() = default;

signals:
    void finished(quint64 id);

protected:
    void run() override;

public:
    Net *m_net;
    QPixmap *m_frame;
    quint64 m_id;
};

#endif // WORKER_H
