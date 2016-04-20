/*
 * ethheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "ethheader.h"

eth_header::eth_header() {

	for (int i = 0; i < 6; ++i) {
		this->SrcMac[i] = 0x00;
	}

	for (int i = 0; i < 6; ++i) {
		this->DestMac[i] = 0x00;
	}

	this->Type[0] = 0x08;
	this->Type[1] = 0x00;

}

eth_header::eth_header(std::string _src, std::string _dest) {

	//setting Source MAC address in Ethernet header
	if (!(_src.empty())) {
		for (int i = 0; i < 6; ++i) {
			this->SrcMac[i] = _src[i];
		}
	} else {
		for (int i = 0; i < 6; ++i) {
			this->SrcMac[i] = 0x00;
		}
	}

	//setting destination MAC address in Ethernet header
	if (!(_dest.empty())) {
		for (int i = 0; i < 6; ++i) {
			this->DestMac[i] = _dest[i];
		}
	} else {
		for (int i = 0; i < 6; ++i) {
			this->DestMac[i] = 0x00;
		}
	}

	this->Type[0] = 0x08;
	this->Type[1] = 0x00;

}

eth_header::~eth_header() {

	if (buff) {
		delete[] buff;
	}

}

void eth_header::update_src_mac(eth_header *obj, std::string src) {

	int values[6];
	const char* src_str = src.c_str();
	sscanf(src_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2],
			&values[3], &values[4], &values[5]);

	for (int i = 0; i < 6; ++i) {
		obj->SrcMac[i] = (u_char) values[i]; //SrcMac stores integer values of hex input
	}

}

void eth_header::update_dest_mac(eth_header *obj, std::string dest) {

	int values[6];
	const char* dest_str = dest.c_str();
	sscanf(dest_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2],
			&values[3], &values[4], &values[5]);

	for (int i = 0; i < 6; ++i) {
		obj->DestMac[i] = (u_char) values[i]; //SrcMac stores integer values of hex input
	}

}

void eth_header::serialize_eth(eth_header* obj, u_char* buff) {

	for (int i = 0; i < 6; ++i) {
		(*buff) = (u_char) obj->SrcMac[i];
		++buff;
	}
	int j = 0;
	for (int i = 6; i < 12; ++i) {
		(*buff) = (u_char) obj->DestMac[j];
		++buff;
		++j;
	}

	*buff = obj->Type[0];
	++buff;
	*buff = obj->Type[1];
	++buff;

}

void eth_header::random_mac_addr(u_char* &buffer, bool _rand_dest_flag,
		bool _rand_src_flag) {
	int i = 6, k = 6;
	if (_rand_dest_flag)
		i = 0;
	if (_rand_src_flag)
		k = 12;

	for (; i < k; ++i) {
		int r = (u_char) std::rand() % 255;
		if ((i == 0) || (i == 6)) {
			//this block of code makes sure that MAC is unicast to avoid problems
			if (r % 2)
				buffer[i] = (r - 1);
			else
				buffer[i] = r;
		} else {
			buffer[i] = r;
		}
	}
}
