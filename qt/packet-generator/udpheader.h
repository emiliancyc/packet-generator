/*
 * udpheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef UDPHEADER_H_
#define UDPHEADER_H_

#include <sys/types.h>
#include <string>
#include <QString>
#include <ipheader.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <includes.h>

class udp_header {
public:
	udp_header();
	udp_header(unsigned short int _src_port, unsigned short int _dest_port);
	virtual ~udp_header();
	void serializeUDP(udp_header* obj, u_char* buff);
	void updateSrcPort(udp_header *obj, std::string _src_port);
	void updateDestPort(udp_header *obj, std::string _dest_port);
	void updateValues(udp_header *obj, unsigned short int _src_port,
			unsigned short int _dest_port, unsigned short int _length);
	unsigned short int calculateChecksum(udp_header* obj, ip_header* obj2,
			u_char* buff, int buff_size);
	void updateChecksum(udp_header *obj, unsigned short int _checksum);
	void fillData(udp_header *obj, QString _data);
	void randPort(udp_header *obj, u_char* &buffer, bool _vlan, bool _src_port,
			bool _dest_port);
	//void update_ip_header_protocol(ip_header* obj, u_char* buff);
private:
	unsigned short int src_port;
	unsigned short int dest_port;
	unsigned short int length;
	unsigned short int checksum;
	char* data = NULL;
	int data_size = 0;
	u_char* buff = NULL;
};

#endif /* UDPHEADER_H_ */
