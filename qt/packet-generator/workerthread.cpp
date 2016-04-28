#include "workerthread.h"

WorkerThread::WorkerThread(QObject *parent) :
		QThread(parent) {

}

void WorkerThread::sendAll(MainWindow* window, sendSocket* socket,
		ip_header* ip_h, tcp_header* tcp_h, udp_header* udp_h,
		unsigned short int* to_send_ip, bool ip_cksm,
		unsigned short int* to_send_tcp, bool tcp_cksm,
		unsigned short int* to_send_udp, bool udp_cksm) {

	connect(this, SIGNAL(updateProgress(int)), window, SLOT(updateProgress(int)));
    connect(this, SIGNAL(finished()), window, SLOT(sendingFinished()));

	double counter = window->getNumOfPackets();
    cont = true;

	//SENDING LOOP
	for (double i = 1; i <= counter; ++i) {

		//check if flags are set and randomize values
		window->randomize(window->getRandFlags());

		if (tcp_h != NULL) {
			if (tcp_cksm == true) {
				(*to_send_tcp) = 0;
                (*to_send_tcp) = tcp_h->calculateChecksum(tcp_h, ip_h,
						((socket->buff_begin) + socket->buff_size_layer2
								+ socket->buff_size_layer3),
						(socket->buff_size_layer4));
			}
		}

		if (udp_h != NULL) {
			if (udp_cksm == true) {
				(*to_send_udp) = 0;
                (*to_send_udp) = udp_h->calculateChecksum(udp_h, ip_h,
						((socket->buff_begin) + socket->buff_size_layer2
								+ socket->buff_size_layer3),
						(socket->buff_size_layer4));
			}
		}

		//check if user has chosen to calculate IP header checksum
		if (ip_h != NULL) {
			if (ip_cksm == true) {
				(*to_send_ip) = 0;
                (*to_send_ip) = ip_h->calculateChecksum(ip_h,
						((socket->buff_begin) + socket->buff_size_layer2), 10);
			}
		}

		//sending section
        socket->sendPacket(socket, socket->buff_begin,
				(socket->buff_size_layer2 + socket->buff_size_layer3
						+ socket->buff_size_layer4));

		emit(updateProgress(i));

		if (cont == false) {
			break;
		}
	}

    emit(finished());

}

void WorkerThread::setCont(bool value) {

    cont = value;

}

void WorkerThread::breakSending() {

    setCont(false);

}
