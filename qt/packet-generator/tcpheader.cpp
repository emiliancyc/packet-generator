/*
 * tcpheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "tcpheader.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <includes.h>
#include <string>

tcp_header::tcp_header() {
    this->src_port = 0;
    this->dest_port = 0;
    this->sequence_number = 0;
    this->acknowledgment_number = 0;
    this->data_offset = 0;
    this->ecn = 0;
    this->control_bits = 0;
    this->window = 0;
    this->urgent_pointer = 0;
    this->options = new u_char[9];
    memset(this->options, 0, sizeof(this->options));
}

tcp_header::~tcp_header() {

    if (options) delete [] options;
    if (data) delete [] data;
    if (buff) delete [] buff;
    delete this;
}

tcp_header::tcp_header(unsigned short int _src_port, unsigned short int _dest_port) {
    this->src_port = _src_port;
    this->dest_port = _dest_port;
    this->sequence_number = 0;
    this->acknowledgment_number = 0;
    this->data_offset = 0;
    this->ecn = 0;
    this->control_bits = 0;
    this->window = 0;
    this->urgent_pointer = 0;
    this->options = new u_char[9];
    memset(this->options, 0, sizeof(this->options));
}

void tcp_header::serialize_tcp(tcp_header* obj, u_char* buff) {

    unsigned short int* temp = (unsigned short int*) buff;
    (*temp) = htons(obj->src_port);
    ++temp;
    (*temp) = htons(obj->dest_port);
    ++temp;

    unsigned int* temp2 = (unsigned int*) temp;
    (*temp2) = ntohl(obj->sequence_number);
    ++temp2;
    (*temp2) = ntohl(obj->acknowledgment_number);
    ++temp2;

    buff = (u_char*) temp2;
    u_char value = 0;
    value = (((obj->data_offset) << 4) & 0xF0);
    value = (value | (obj->ecn));
    (*buff) = value;
    ++buff;
    (*buff) = obj->control_bits;
    ++buff;

    temp = (unsigned short int*) buff;
    (*temp) = htons(obj->window);
    ++temp;
    (*temp) = 0; //checksum
    ++temp;
    (*temp) = htons(obj->urgent_pointer);
    ++temp;

    buff = (u_char*) temp;
    //(*buff) = 0;//obj->options;
    //++buff;
    if (this->data_size != 0) {
        for (int i = 0; i < this->data_size; i++) {
            (*buff) = obj->data[i];
            ++buff;
        }
    }
    /* (*temp) = obj->checksum;
    ++temp; */

}

void tcp_header::update_src_port(tcp_header *obj, std::string _src_port) {
    int temp = atoi(_src_port.c_str());
    obj->src_port = (unsigned short int) temp;

}

void tcp_header::update_dest_port(tcp_header *obj, std::string _dest_port) {
    int temp = atoi(_dest_port.c_str());
    obj->dest_port = (unsigned short int) temp;
}

void tcp_header::update_values(tcp_header *obj, unsigned short int _src_port, unsigned short int _dest_port, unsigned long int _sequence_number,
                               unsigned long int _acknowledgment_number, u_char _data_offset, u_char _ecn, u_char _control_bits, unsigned short int _window,
                               unsigned short int _urgent_pointer) {

    obj->src_port = _src_port;
    obj->dest_port = _dest_port;
    obj->sequence_number = _sequence_number;
    obj->acknowledgment_number = _acknowledgment_number;
    obj->data_offset = _data_offset;
    obj->ecn = _ecn;
    obj->control_bits = _control_bits;
    obj->window = _window;
    obj->urgent_pointer = _urgent_pointer;
    //obj->options = new u_char(0);

}

void tcp_header::update_checksum(tcp_header *obj, unsigned short int _checksum) {

    obj->checksum = _checksum;

}

void tcp_header::update_options(tcp_header *obj, u_char* _options) {

    obj->options = new u_char[1];
    obj->options = _options;

}

void tcp_header::fill_data(tcp_header *obj, QString _data) {

    std::string temp = _data.toStdString();
    char* data = const_cast<char*>(temp.c_str());
    obj->data_size = _data.length();
    obj->data = new char[(obj->data_size) + 1];
    strcpy(obj->data, data);

}

short unsigned int tcp_header::calculate_checksum(tcp_header *obj, u_char* buff, int n) {

    unsigned sum = 0;

    /* Accumulate checksum */
/*    for (int i = 0; i < size - 1; i += 2) {
         unsigned short word16 = *(unsigned short *) &buf[i];
         sum += word16;
    }

    /* Handle odd-sized case */
 /*   if (size & 1) {
        unsigned short word16 = (unsigned char) buf[i];
        sum += word16;
    }

    /* Fold to get the ones-complement result */
 /*   while (sum >> 16)
        sum = (sum & 0xFFFF)+(sum >> 16);

    /* Invert to get the negative in ones-complement arithmetic */
    //return ~sum;
    return sum;
}
