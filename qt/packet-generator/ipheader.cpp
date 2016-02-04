/*
 * ipheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "ipheader.h"
#include <arpa/inet.h>
#include "includes.h"

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
    obj->ihl = 5; //that could be dynamic value changed by user and connected with options field
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

void ip_header::serialize_ip(ip_header* obj, u_char* buff) {

    //u_char ver_ihl = (((obj->ver) << 4 ) | (obj->ihl));
    (*buff) = (((obj->ver) << 4 ) | (obj->ihl));
    ++buff;
    (*buff) = obj->ToS;
    ++buff;

    unsigned short int* temp = (unsigned short int*) buff;
    (*temp) = htons(obj->length);
    ++temp;
    (*temp) = (obj->id);
    ++temp;

    buff = (u_char*) temp;
    (*buff) = obj->flags;
    ++buff;
    (*buff) = obj->offset;
    ++buff;
//    buff = (u_char*) temp;

//    (*buff) = (u_char) *buff;
    (*buff) = obj->ttl;
    ++buff;
    (*buff) = obj->protocol;
    ++buff;

    temp = (unsigned short int*) buff;
    (*temp) = obj->checksum;
    ++temp;

    unsigned int* ptr = (unsigned int*) temp;
    (*ptr) = obj->sourceip;
    ++ptr;
    (*ptr) = obj->destip;
    ++ptr;

}

//little help about calcuating IP header checksum:
//http://www.unix.com/programming/117551-calculate-ip-header-checksum-manually.html
//SOURCE:
//http://web.eecs.utk.edu/~cs594np/unp/checksum.html
short unsigned int ip_header::calculate_checksum(ip_header* obj,u_char* buff, int n)
{
    long sum = 0;
    unsigned short* buff2 = (unsigned short*) buff;

    while (n>0){
        sum += *(buff2++);
        if(sum & 0x80000000)
            sum = (sum & 0xFFFF) + (sum >> 16);
        n--;
    }

    if(n)
        sum += (unsigned short)* buff;

    while(sum>>16)
        sum = (sum & 0xFFFF) + (sum >>16);

    obj->checksum = ~sum;
    return ~sum;
}

