/*
 * tcpheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef TCPHEADER_H_
#define TCPHEADER_H_
#include <sys/types.h>
#include <string>
class tcp_header {
public:
	tcp_header();
    tcp_header(unsigned short int _src_port, unsigned short int _dest_port);
	virtual ~tcp_header();
    void serialize_tcp(tcp_header* obj, u_char* buff);
    void update_src_port(tcp_header *obj, std::string src);
    void update_dest_port(tcp_header *obj, std::string dest);
    void update_values(tcp_header *obj, unsigned short int _src_port, unsigned short int _dest_port, unsigned long int _sequence_number,
                       unsigned long int _acknowledgment_number, u_char _data_offset, u_char _ecn, u_char _control_bits, unsigned short int _window,
                       unsigned short int urgent_pointer);
    void update_checksum(tcp_header *obj, unsigned short int _checksum);
    void update_options(tcp_header *obj, u_char* _options);
    void fill_data(tcp_header *obj, u_char* data);

private:
    unsigned short int src_port;
    unsigned short int dest_port;
    unsigned long int sequence_number;
    unsigned long int acknowledgment_number;
    u_char data_offset;
    u_char ecn;
    u_char control_bits;
    unsigned short int window;
    unsigned short int checksum;
    unsigned short int urgent_pointer;
    u_char* options = NULL;
    u_char* data = NULL;
    u_char* buff = NULL;

};

#endif /* TCPHEADER_H_ */
