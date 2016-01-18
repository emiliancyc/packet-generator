/*
 * ipheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "ipheader.h"
#include <arpa/inet.h>

ip_header::ip_header() {
	// TODO Auto-generated constructor stub

}

ip_header::~ip_header() {
	// TODO Auto-generated destructor stub
}

ip_header::ip_header(std::string _dest, std::string _src) {
}

void ip_header::update_src_ip(ip_header *obj, std::string src) {

    obj->sourceip = inet_addr(src.c_str());

}

void ip_header::update_dest_ip(ip_header *obj, std::string dest) {

    obj->destip = inet_addr(dest.c_str());

}


void ip_header::update_values(ip_header *obj, u_char _ToS, unsigned short int _length, unsigned short int _id,
                   unsigned short int _flags, unsigned short int _offset, u_char _ttl, u_char _protocol) {

    obj->ver = 4;
    obj->ihl = 5;
    obj->ToS = _ToS;
    obj->length = _length;
    obj->id = htons(_id); //offset is set in bytes, but TCPDump shows in bits!

    _offset /= 8;
    obj->flags = (_offset >> 8);
    obj->offset = _offset;

    unsigned short flag = 0;
    if(_flags == 1)
        flag = 2;
    if(_flags == 2)
        flag = 1;
    obj->flags = ((obj->flags) | (flag << 5));

    obj->ttl = _ttl;
    obj->protocol = _protocol;
    obj->checksum = 0;
}
