#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QPixmap>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

#include "worker.h"

using namespace cv::dnn::dnn4_v20200310;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void start();

signals:
    void error(QString message);
    void frame(QPixmap *frame);

public slots:
    void onProcessedFrame(quint64 id);

protected:
    void incomingConnection(qintptr descriptor) override;

private:
    quint64 m_counter;
    QTcpSocket m_socket;

    QVector<Net> m_nets;
    QVector<QPixmap> m_frames;
    QVector<Worker*> m_workers;
};

#endif // SERVER_H
