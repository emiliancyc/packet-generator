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

	ver = 4;
	ihl = 5;
	ToS = 0;
	length = 20;
	id = htons(1); //offset is set in bytes, but TCPDump shows in bits!
	flags = 0;
	offset = 0;
	ttl = 64;
	protocol = 6;
	sourceip = inet_addr("0.0.0.0");
	destip = inet_addr("0.0.0.0");
	checksum = 0;

}

ip_header::~ip_header() {

	if (buff != NULL)
		delete[] buff;
}

ip_header::ip_header(std::string _dest, std::string _src) {

	ver = 4;
	ihl = 5;
	ToS = 0;
	length = 20;
	id = htons(1); //offset is set in bytes, but TCPDump shows in bits!
	flags = 0;
	offset = 0;
	ttl = 64;
	protocol = 6;
	sourceip = inet_addr(_src.c_str());
	destip = inet_addr(_dest.c_str());
	checksum = 0;

}

void ip_header::updateSrcIP(ip_header *obj, std::string _src) {

	obj->sourceip = inet_addr(_src.c_str());

}

void ip_header::updateDestIP(ip_header *obj, std::string _dest) {

	obj->destip = inet_addr(_dest.c_str());

}

void ip_header::updateValues(ip_header *obj, u_char _ToS,
		unsigned short int _length, unsigned short int _id,
		unsigned short int _flags, unsigned short int _offset, u_char _ttl,
		u_char _protocol) {

	obj->ver = 4;
	obj->ihl = 5; //that could be dynamic value changed by user and connected with options field
	obj->ToS = _ToS;
	obj->length = _length;
	obj->id = htons(_id);
	//offset is set in bytes, but TCPDump shows in bits!
	_offset /= 8;
	obj->flags = (_offset >> 8);
	obj->offset = _offset;

	unsigned short flag = 0;
	if (_flags == 1)
		flag = 2;
	if (_flags == 2)
		flag = 1;
	obj->flags = ((obj->flags) | (flag << 5));

	obj->ttl = _ttl;
	obj->protocol = _protocol;
	obj->checksum = 0;

}

void ip_header::serializeIP(ip_header* obj, u_char* buff) {

	(*buff) = (((obj->ver) << 4) | (obj->ihl));
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

	temp = NULL;
	ptr = NULL;

}

//little help about calcuating IP header checksum:
//http://www.unix.com/programming/117551-calculate-ip-header-checksum-manually.html
//SOURCE:
//http://web.eecs.utk.edu/~cs594np/unp/checksum.html
short unsigned int ip_header::calculateChecksum(ip_header* obj, u_char* buff,
		int n) {

	long sum = 0;
	unsigned short* buff2 = (unsigned short*) buff;

	while (n > 0) {
		sum += *(buff2++);
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		n--;
	}

	if (n)
		sum += (unsigned short) *buff;

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	obj->checksum = ~sum;
	buff2 = NULL;
	return ~sum;
}

void ip_header::randID(ip_header *obj, u_char* &buffer, bool _vlan) {

	unsigned short* temp = (unsigned short*) buffer;
	if (_vlan) {
		temp += 9; // 9 because VLAN header has 18 bytes, and unsigned short is 2 bytes long. 18/2 = 9
	} else {
		temp += 7; //same as above, Ethernet header has 14 bytes
	}

	temp += 2; // jump 4 bytes forward into ID field
	unsigned short int value = rand() % 65536;
	(*temp) = value;
	temp = NULL;
	obj->id = value;
}

void ip_header::randTTL(ip_header *obj, u_char* &buffer, bool _vlan) {

	u_char* temp = buffer;
	if (_vlan) {
		temp += 18;
	} else {
		temp += 14;
	}

	temp += 8;
	u_char value = rand() % 256;
	(*temp) = value;
	obj->ttl = value;
	temp = NULL;

}

void ip_header::randIP(ip_header *obj, u_char* &buffer, bool _vlan,
		bool _src_ip_flag, bool _dest_ip_flag) {

	u_char* temp = buffer;
	if (_vlan) {
		temp += 18;
	} else {
		temp += 14;
	}

	if (_src_ip_flag) {
		temp += 12; // jump 12 bytes forward into source IP field
		unsigned int* ip = (unsigned int*) temp;
		for (int i = 0; i < 4; ++i) {
			(*temp) = rand() % 256;
			temp++;
		}
		obj->sourceip = *ip;
		ip = NULL;
	} else if (_dest_ip_flag) {
		temp += 16; // jump 4 bytes forward into destination IP field
		unsigned int* ip = (unsigned int*) temp;
		for (int i = 0; i < 4; ++i) {
			(*temp) = rand() % 256;
			temp++;
		}
		obj->destip = *ip;
		ip = NULL;
	}
	temp = NULL;
}

void ip_header::updateLength(unsigned short int _length) {
	length += _length;
}

unsigned int ip_header::getSrcIP() {
	return sourceip;
}

unsigned int ip_header::getDestIP() {
	return destip;
}

u_char ip_header::getProtocol() {
	return protocol;
}

void ip_header::setProtocol(u_char _protocol) {
	protocol = _protocol;
}

unsigned short int ip_header::getLength() {
	return length;
}

void ip_header::updateProtocol(ip_header* obj, u_char _protocol) {
	obj->protocol = _protocol;
//    QTableWidgetItem* text10 = new QTableWidgetItem(_protocol);
//    ui->layer3_tableWidget->setItem(2, 2, text10);
}

void ip_header::setLength(unsigned short int _length) {
	length = _length;
}
