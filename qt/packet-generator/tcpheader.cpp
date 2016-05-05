/*
 * tcpheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "tcpheader.h"

tcp_header::tcp_header() {
	src_port = 0;
	dest_port = 0;
	sequence_number = 0;
	acknowledgment_number = 0;
	data_offset = 0;
	ecn = 0;
	control_bits = 0;
	window = 0;
	urgent_pointer = 0;
	options = 0;
	//options = new u_char[9];
	//memset(options, 0, sizeof(options));
}

tcp_header::~tcp_header() {

	if (options != NULL) {
		delete[] options;
		options = NULL;
	}
	if (data != NULL) {
		delete[] data;
		data = NULL;
	}
	if (buff != NULL) {
		delete[] buff;
		buff = NULL;
	}
	//delete this;
}

tcp_header::tcp_header(unsigned short int _src_port,
		unsigned short int _dest_port) {
	src_port = _src_port;
	dest_port = _dest_port;
	sequence_number = 0;
	acknowledgment_number = 0;
	data_offset = 0;
	ecn = 0;
	control_bits = 0;
	window = 0;
	urgent_pointer = 0;
	options = 0;
	//options = new u_char[9];
	//memset(options, 0, sizeof(options));
}

void tcp_header::serializeTCP(tcp_header* obj, u_char* buff) {

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
	if (data_size != 0) {
		for (int i = 0; i < data_size; i++) {
			(*buff) = obj->data[i];
			++buff;
		}
	}

	temp = NULL;
	temp2 = NULL;
}

void tcp_header::updateSrcPort(tcp_header *obj, std::string _src_port) {
	int temp = atoi(_src_port.c_str());
	obj->src_port = (unsigned short int) temp;

}

void tcp_header::updateDestPort(tcp_header *obj, std::string _dest) {
	int temp = atoi(_dest.c_str());
	obj->dest_port = (unsigned short int) temp;
}

void tcp_header::updateValues(tcp_header *obj, unsigned short int _src_port,
		unsigned short int _dest_port, unsigned long int _sequence_number,
		unsigned long int _acknowledgment_number, u_char _data_offset,
		u_char _ecn, u_char _control_bits, unsigned short int _window,
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

void tcp_header::updateChecksum(tcp_header *obj, unsigned short int _checksum) {

	obj->checksum = _checksum;

}

void tcp_header::updateOptions(tcp_header *obj, u_char* _options) {

	obj->options = new u_char[1];
	obj->options = _options;

}

void tcp_header::fillData(tcp_header *obj, QString _data) {

	std::string temp = _data.toStdString();
	char* data = const_cast<char*>(temp.c_str());
	obj->data_size = _data.length();
	obj->data = new char[(obj->data_size) + 1];
	strcpy(obj->data, data);

}

short unsigned int tcp_header::calculateChecksum(tcp_header* obj,
		ip_header *obj2, u_char* buff, int buff_size) {

	//create pseudo-header
	int header_size = (((sizeof(unsigned int)) * 2)
			+ ((sizeof(unsigned short int)) * 2));
	u_char* pseudo_header = new u_char[header_size];
	unsigned int* temp = (unsigned int*) pseudo_header;
	(*temp) = obj2->getSrcIP();
	temp++;
	(*temp) = obj2->getDestIP();
	temp++;
//    (*temp) = 0;
//    temp++;
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
	delete[] pseudo_header;
	pseudo_header = NULL;
	temp = NULL;
	ptr = NULL;
	final = NULL;
	delete[] buff2;
	buff2 = NULL;
	return ~sum;
}

void tcp_header::randPort(tcp_header *obj, u_char* &buffer, bool _vlan,
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

	temp = NULL;
	ptr = NULL;

}

void tcp_header::randSeqNum(tcp_header *obj, u_char* &buffer, bool _vlan) {

	unsigned short int* temp = (unsigned short int*) buffer;
	if (_vlan) {
		temp += 21;
	} else {
		temp += 19;
	}

	unsigned int* ptr = (unsigned int*) temp;
	unsigned int value = (rand() % 4294967296);
	(*ptr) = value;
	obj->sequence_number = value;

	temp = NULL;
	ptr = NULL;

}

void tcp_header::randAckNum(tcp_header *obj, u_char* &buffer, bool _vlan) {

	unsigned short int* temp = (unsigned short int*) buffer;
	if (_vlan) {
		temp += 23;
	} else {
		temp += 21;
	}

	unsigned int* ptr = (unsigned int*) temp;
	unsigned int value = (rand() % 4294967296);
	(*ptr) = value;
	obj->acknowledgment_number = value;
	temp = NULL;
	ptr = NULL;

}

int tcp_header::getDataSize() {
	return data_size;
}
