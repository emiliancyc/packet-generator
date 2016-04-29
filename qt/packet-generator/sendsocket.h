/*
 * sendsocket.h
 *
 *  Created on: Jan 19, 2016
 *      Author: emilian
 */

#ifndef SENDSOCKET_H
#define SENDSOCKET_H

#include <QMainWindow>
#include <string>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <net/if.h>

class sendSocket {
public:
	sendSocket();
	sendSocket(std::string _interface, std::string _dest);
	~sendSocket();

	int socket_fd;
	struct ifreq interface_index;
	struct sockaddr_ll sock_addr;
    void sendPacket(sendSocket *socket, u_char* _send_buff, size_t _size);
    void updateDestMAC(struct sockaddr_ll _struct, std::string _dest);

	u_char* buff_layer2 = NULL;
	u_char* buff_layer3 = NULL;
	u_char* buff_layer4 = NULL;
	u_char* buff_begin = NULL;
	int buff_size_layer2 = 0;
	int buff_size_layer3 = 0;
	int buff_size_layer4 = 0;

	signals:

public slots:
};

#endif // SENDSOCKET_H
