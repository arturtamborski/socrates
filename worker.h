#ifndef STREAMWORKER_H
#define STREAMWORKER_H

#include <QObject>
#include <QRunnable>
#include <QPixmap>
#include <QDebug>

class Worker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Worker() = default;

signals:
    void finished(quint64 id, QPixmap *frame);

protected:
    void run() override;

public:
    QPixmap *m_data;
    quint64 m_id;
};

#endif // STREAMWORKER_H
