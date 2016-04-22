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
#include <QString>
#include <ipheader.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <includes.h>

class tcp_header {
public:
	tcp_header();
	tcp_header(unsigned short int _src_port, unsigned short int _dest_port);
	virtual ~tcp_header();
	void serialize_tcp(tcp_header* obj, u_char* buff);
	void update_src_port(tcp_header *obj, std::string _src);
	void update_dest_port(tcp_header *obj, std::string _dest);
	void update_values(tcp_header *obj, unsigned short int _src_port,
			unsigned short int _dest_port, unsigned long int _sequence_number,
			unsigned long int _acknowledgment_number, u_char _data_offset,
			u_char _ecn, u_char _control_bits, unsigned short int _window,
			unsigned short int _urgent_pointer);
	short unsigned int calculate_checksum(tcp_header* obj, ip_header* obj2,
			u_char* buff, int buff_size);
	void update_checksum(tcp_header *obj, unsigned short int _checksum);
	void update_options(tcp_header *obj, u_char* _options);
	void fill_data(tcp_header *obj, QString data);
	void rand_port(tcp_header *obj, u_char* &buffer, bool _vlan, bool _src_port,
			bool _dest_port);
	void rand_seq_num(tcp_header *obj, u_char* &buffer, bool _vlan);
	void rand_ack_num(tcp_header *obj, u_char* &buffer, bool _vlan);
	int getDataSize();

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
	char* data = NULL;
	int data_size = 0;
	u_char* buff = NULL;

};

#endif /* TCPHEADER_H_ */
