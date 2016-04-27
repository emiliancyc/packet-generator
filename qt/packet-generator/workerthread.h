#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "mainwindow.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = 0);
    QThread* thread = NULL;
    Ui::MainWindow* window = NULL;

signals:
    void finished();

public slots:
    void send_all();

};

#endif // WORKERTHREAD_H
