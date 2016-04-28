#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "mainwindow.h"
#include "includes.h"

class WorkerThread: public QThread {
	Q_OBJECT
public:
	explicit WorkerThread(QObject *parent = 0);
	//QThread* thread = NULL;
	//Ui::MainWindow* window = NULL;
    void setCont(bool value);

	signals:
    void finished();
	void updateProgress(int _value);

public slots:
    void sendAll(MainWindow* window, sendSocket* socket, ip_header* ip_h, tcp_header* tcp_h, udp_header* udp_h, unsigned short int* to_send_ip, bool ip_cksm, unsigned short int* to_send_tcp, bool tcp_cksm, unsigned short int* to_send_udp, bool udp_cksm);
	void breakSending();

private:
	bool cont = true;

};

#endif // WORKERTHREAD_H
