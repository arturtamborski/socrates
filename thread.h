#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QObject *parent = nullptr);
    void setUrl(QString &url);
    void run() override;

signals:
    void error(QString message);
    void frame(QPixmap *frame);

private:
    QString m_url;
};

#endif // THREAD_H
