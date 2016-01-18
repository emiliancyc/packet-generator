/*
 * ipheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "ipheader.h"
#include <arpa/inet.h>

ip_header::ip_header() {

    this->ver = 4;
    this->ihl = 5;
    this->ToS = 0;
    this->length = 20;
    this->id = htons(1); //offset is set in bytes, but TCPDump shows in bits!
    this->flags = 0;
    this->offset = 0;
    this->ttl = 64;
    this->protocol = 6;
    this->sourceip = inet_addr("0.0.0.0");
    this->destip = inet_addr("0.0.0.0");
    this->checksum = 0;

}

ip_header::~ip_header() {

    delete this;
}

ip_header::ip_header(std::string _dest, std::string _src) {

    this->ver = 4;
    this->ihl = 5;
    this->ToS = 0;
    this->length = 20;
    this->id = htons(1); //offset is set in bytes, but TCPDump shows in bits!
    this->flags = 0;
    this->offset = 0;
    this->ttl = 64;
    this->protocol = 6;
    this->sourceip = inet_addr(_src.c_str());
    this->destip = inet_addr(_dest.c_str());
    this->checksum = 0;

}

void ip_header::update_src_ip(ip_header *obj, std::string _src) {

    obj->sourceip = inet_addr(_src.c_str());

}

void ip_header::update_dest_ip(ip_header *obj, std::string _dest) {

    obj->destip = inet_addr(_dest.c_str());

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
