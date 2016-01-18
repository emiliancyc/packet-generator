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


    if((this->socket_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        //std::cout << "Error while opening socket" << endl;
        delete this;
    }

    ///opening socket
 //   std::string iname = "lo";

 //   this->interface_index.ifr_name = iname.c_str();

    if(ioctl(this->socket_fd, SIOCGIFINDEX, this->interface_index)<0)
    {
        //cout << "interface index error" << endl;
        delete this;
        //return -1;
    }

    char* iname = "lo";
    struct ifreq if_index;
    memset(&if_index, 0, sizeof(struct ifreq));
    strncpy(if_index.ifr_name, iname, strlen(iname));

    this->sock_addr.sll_ifindex = if_index.ifr_ifindex;
    this->sock_addr.sll_halen = ETH_ALEN;

    for (int i = 0; i < 6; ++i) {
        this->sock_addr.sll_addr[i] = 0;
    }

}

sendSocket::sendSocket(std::string _interface, std::string _dest) {

    if((this->socket_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        delete this;
    }

    struct ifreq if_index;
    memset(&if_index, 0, sizeof(struct ifreq));
    strncpy(if_index.ifr_name, _interface.c_str(), _interface.length());

    if(ioctl(this->socket_fd, SIOCGIFINDEX, &if_index) < 0)
    {
       delete this;
    }

    this->interface_index = if_index;

    struct sockaddr_ll socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_ll));
    socket_address.sll_ifindex = if_index.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;

    this->update_dest_mac(socket_address, _dest);

    this->sock_addr = socket_address;

}

sendSocket::~sendSocket() {

    //delete this;

}

int sendSocket::send_packet(sendSocket _socket, u_char* send_buff, size_t _size) {

    socklen_t len = sizeof(struct sockaddr_ll);
    //if(sendto(this>socket_fd,_buff, _size,0,(struct sockaddr*) &(self->socket_address),len)<0)
   // {
   //    std::cout << "Error in sendto\n" << endl;
   //    return -1;
   // }

    return 0;


}

void sendSocket::update_dest_mac(struct sockaddr_ll _struct, std::string _dest) {

    int values[6];
    const char* dest_str = _dest.c_str();
    sscanf(dest_str, "%x:%x:%x:%x:%x:%x", &values[0],&values[1],&values[2],&values[3],&values[4],&values[5]);

    for (int i = 0; i < 6; ++i) {
         _struct.sll_addr[i] = (u_char) values[i]; //SrcMac stores integer values of hex input
    }

}
