/*
 * udpheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "udpheader.h"
#include <string>

udp_header::udp_header() {
	src_port = 0;
	dest_port = 0;
	length = 8;
	checksum = 0;

}

udp_header::~udp_header() {
	// TODO Auto-generated destructor stub
}

udp_header::udp_header(unsigned short int _src_port,
		unsigned short int _dest_port) {
	src_port = _src_port;
	dest_port = _dest_port;
	length = 8;
	checksum = 0;
}

void udp_header::serializeUDP(udp_header* obj, u_char* buff) {
	unsigned short int* temp = (unsigned short int*) buff;
	(*temp) = htons(obj->src_port);
	++temp;
	(*temp) = htons(obj->dest_port);
	++temp;

	(*temp) = htons(obj->length);
	++temp;
	(*temp) = 0;
	++temp;

	buff = (u_char*) temp;

	if (data_size != 0) {
		for (int i = 0; i < data_size; i++) {
			(*buff) = obj->data[i];
			++buff;
		}
	}

	temp = NULL;
}

void udp_header::updateSrcPort(udp_header *obj, std::string _src_port) {
	int temp = atoi(_src_port.c_str());
	obj->src_port = (unsigned short int) temp;
}

void udp_header::updateDestPort(udp_header *obj, std::string _dest_port) {
	int temp = atoi(_dest_port.c_str());
	obj->dest_port = (unsigned short int) temp;

}

void udp_header::updateValues(udp_header *obj, unsigned short int _src_port,
		unsigned short int _dest_port, unsigned short int _length) {
	obj->src_port = _src_port;
	obj->dest_port = _dest_port;
	obj->length = _length;
	obj->checksum = 0;

}

unsigned short int udp_header::calculateChecksum(udp_header* obj,
		ip_header* obj2, u_char* buff, int buff_size) {
	//create pseudo-header
	int header_size = (((sizeof(unsigned int)) * 2)
			+ ((sizeof(unsigned short int)) * 2));
	u_char* pseudo_header = new u_char[header_size];
	unsigned int* temp = (unsigned int*) pseudo_header;
	(*temp) = obj2->getSrcIP();
	temp++;
	(*temp) = obj2->getDestIP();
	temp++;

	unsigned short int* ptr = (unsigned short int*) temp;
	(*ptr) = htons(obj2->getProtocol());
	ptr++;
	(*ptr) = htons((unsigned short int) buff_size);

	// add padding byte if needed
	if (buff_size & 1) {
		buff_size++;
		u_char* new_buff = new u_char[buff_size];
		memcpy(new_buff, buff, buff_size - 1);
		new_buff[buff_size - 1] = 0;

		//TODO MEMORY LEAK!
		//delete [] buff;
		buff = new_buff;
	}

	//allocate temporary buffer for checksum calculation
	u_char* buff2 = new u_char[buff_size + header_size];

	memcpy(buff2, buff, buff_size);
	memcpy(buff2 + buff_size, pseudo_header, header_size);

	// checksum calculation
	long sum = 0;
	unsigned short* final = (unsigned short*) buff2;

	int n = buff_size + header_size;

	for (int i = 0; i < n; i = i + 2) {
		sum += *final++; // add pointed value and increment pointer
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
	}

	if (n & 1)
		sum += (unsigned short) *final;

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	obj->checksum = ~sum;

	delete[] buff2;
	delete[] pseudo_header;
	pseudo_header = NULL;
	temp = NULL;
	buff2 = NULL;
	ptr = NULL;
	return ~sum;

}

void udp_header::updateChecksum(udp_header *obj,
		unsigned short int _checksum) {
	obj->checksum = _checksum;

}

void udp_header::fillData(udp_header *obj, QString _data) {

	std::string temp = _data.toStdString();
	char* data = const_cast<char*>(temp.c_str());
	obj->data_size = _data.length();
	obj->data = new char[(obj->data_size) + 1];
	strcpy(obj->data, data);

}

void udp_header::randPort(udp_header *obj, u_char* &buffer, bool _vlan,
		bool _src_port, bool _dest_port) {

	unsigned short int* temp = (unsigned short int*) buffer;
	if (_vlan) {
		temp += 19;
	} else {
		temp += 17;
	}

	unsigned short int* ptr = (unsigned short int*) temp;
	if (_src_port == true) {
		unsigned short int value = rand() % 65536;
		(*ptr) = value;
		obj->src_port = value;
	}
	if (_dest_port == true) {
		ptr++;
		unsigned short int value = rand() % 65536;
		(*ptr) = value;
		obj->dest_port = value;
	}
}
