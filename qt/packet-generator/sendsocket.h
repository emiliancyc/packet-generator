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

class sendSocket
{
public:
    sendSocket();
    sendSocket(std::string _interface, std::string _dest);
    ~sendSocket();

    int socket_fd;
    struct ifreq interface_index;
    struct sockaddr_ll sock_addr;
    int send_packet(sendSocket sock, u_char* send_buff, size_t _size);
    void update_dest_mac(struct sockaddr_ll _struct, std::string dest);
    u_char* buff;

signals:

public slots:
};

#endif // SENDSOCKET_H
