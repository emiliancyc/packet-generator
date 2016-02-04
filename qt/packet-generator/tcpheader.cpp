/*
 * tcpheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "tcpheader.h"
#include <stdlib.h>

tcp_header::tcp_header() {
	// TODO Auto-generated constructor stub

}

tcp_header::~tcp_header() {
	// TODO Auto-generated destructor stub
}

tcp_header::tcp_header(unsigned short int _src_port, unsigned short int _dest_port) {

}

void tcp_header::serialize_tcp(tcp_header* obj, u_char* buff) {


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
    obj->options = 0;

}

void tcp_header::update_checksum(tcp_header *obj, unsigned short int _checksum) {

}

void tcp_header::update_options(tcp_header *obj, u_char* _options) {

}

void tcp_header::fill_data(tcp_header *obj, u_char* data) {

}
