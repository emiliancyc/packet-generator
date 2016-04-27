#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "mainwindow.h"
#include "includes.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = 0);
    //QThread* thread = NULL;
    //Ui::MainWindow* window = NULL;

signals:
    void finished(unsigned short int* to_send_ip, unsigned short int* to_send_tcp, unsigned short int* to_send_udp);
    void updateProgress(int _value);

public slots:
    void send_all(MainWindow* window, sendSocket* socket, ip_header* ip_h, tcp_header* tcp_h, udp_header* udp_h, unsigned short int* to_send_ip, bool ip_cksm, unsigned short int* to_send_tcp, bool tcp_cksm, unsigned short int* to_send_udp, bool udp_cksm);

};

#endif // WORKERTHREAD_H
