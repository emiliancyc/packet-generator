#include "sendsocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
//#include <string.h>
#include <stddef.h>
#include "includes.h"

sendSocket::sendSocket() {

	if ((this->socket_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        //delete this;
	}

	///opening socket

	if ((ioctl(this->socket_fd, SIOCGIFINDEX, this->interface_index)) < 0) {
        //delete this;
	}

    std::string iname = "lo";
	struct ifreq if_index;
	memset(&if_index, 0, sizeof(struct ifreq));
    strncpy(if_index.ifr_name, iname.c_str(), iname.length());

	sock_addr.sll_ifindex = if_index.ifr_ifindex;
	sock_addr.sll_halen = ETH_ALEN;

	for (int i = 0; i < 6; ++i) {
		sock_addr.sll_addr[i] = 0;
	}

}

sendSocket::sendSocket(std::string _interface, std::string _dest) {

	if ((this->socket_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {

		//TODO
		delete this;
	}

	struct ifreq if_index;
	memset(&if_index, 0, sizeof(struct ifreq));
	strncpy(if_index.ifr_name, _interface.c_str(), _interface.length());

	if (ioctl(this->socket_fd, SIOCGIFINDEX, &if_index) < 0) {

		//TODO
        //delete this;
	}

	interface_index = if_index;

	struct sockaddr_ll socket_address;
	memset(&socket_address, 0, sizeof(struct sockaddr_ll));
	socket_address.sll_ifindex = if_index.ifr_ifindex;
	socket_address.sll_halen = ETH_ALEN;

	update_dest_mac(socket_address, _dest);
	sock_addr = socket_address;

}

sendSocket::~sendSocket() {

//    if (buff_layer4 != NULL) {
//        delete [] buff_layer4;
//        buff_layer4 = NULL;
//    }
//    if (buff_layer3 != NULL) {
//        delete [] buff_layer3;
//        buff_layer3 = NULL;
//    }
//    if (buff_layer2 != NULL) {
//        delete [] buff_layer2;
//        buff_layer2 = NULL;
//    }
//    if (buff_begin != NULL) {
//        delete [] buff_begin;
//        buff_begin = NULL;
//    }

}

void sendSocket::send_packet(sendSocket _socket, u_char* send_buff,
		size_t _size) {
	socklen_t len = sizeof(struct sockaddr_ll);
	if (sendto(_socket.socket_fd, send_buff, _size, 0,
			(struct sockaddr*) &(sock_addr), len) < 0) {
		//error handling

	}
}

void sendSocket::update_dest_mac(struct sockaddr_ll _struct,
		std::string _dest) {

	int values[6];
	const char* dest_str = _dest.c_str();
	sscanf(dest_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2],
			&values[3], &values[4], &values[5]);

	for (int i = 0; i < 6; ++i) {
		_struct.sll_addr[i] = (u_char) values[i]; //SrcMac stores integer values of hex input
	}

}
