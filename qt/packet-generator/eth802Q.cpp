/*
 * eth802Q.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "eth802Q.h"
#include <iostream>
#include "includes.h"

eth_802Q::eth_802Q() {
	//setting Source MAC address in Ethernet header
	for (int i = 0; i < 6; ++i) {
		this->SrcMac[i] = 0x00;
	}

	//setting destination MAC address in Ethernet header
	for (int i = 0; i < 6; ++i) {
		this->DestMac[i] = 0x00;
	}

	this->TPID[0] = 0x81;
	this->TPID[1] = 0x00;

	int PCPbin = 0, DEIbin = 0;
	PCPbin = 0x00 << 13;
	DEIbin = 0x00 << 12;

	int vid = 1;

	int TCIbin = PCPbin | DEIbin | vid;
	this->TCI[0] = TCIbin >> 8;
	this->TCI[1] = TCIbin & 0x00ff;

	this->Type[0] = 0x08;
	this->Type[1] = 0x00;

}

eth_802Q::eth_802Q(std::string _dest, std::string _src, int _pcp, int _dei,
		int _vid) {

	//setting Source MAC address in Ethernet header
	if (!(_src.empty())) {
		for (int i = 0; i < 6; ++i) {
			this->SrcMac[i] = (u_char) _src[i];
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

	this->TPID[0] = 0x81;
	this->TPID[1] = 0x00;

	int PCPbin = 0, DEIbin = 0;
//	if (_pcp != NULL)
	PCPbin = _pcp << 13;

//	if (_dei != NULL)
	DEIbin = _dei << 12;

//	if (_vid == NULL)
	_vid = 1;

	int TCIbin = PCPbin | DEIbin | _vid;
	this->TCI[0] = TCIbin >> 8;
	this->TCI[1] = TCIbin & 0x00ff;
	this->Type[0] = 0x08;
	this->Type[1] = 0x00;

}

eth_802Q::~eth_802Q() {

	delete this;

}

void eth_802Q::update_src_mac(eth_802Q *obj, std::string src) {

	int values[6];
	const char* src_str = src.c_str();
	sscanf(src_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2],
			&values[3], &values[4], &values[5]);

	for (int i = 0; i < 6; ++i) {
		obj->SrcMac[i] = (u_char) values[i];
		//SrcMac stores integer values of hex input
	}

}

void eth_802Q::update_dest_mac(eth_802Q *obj, std::string dest) {

	int values[6];
	const char* dest_str = dest.c_str();
	sscanf(dest_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2],
			&values[3], &values[4], &values[5]);

	for (int i = 0; i < 6; ++i) {
		obj->DestMac[i] = (u_char) values[i];
		//SrcMac stores integer values of hex input
	}

}
void eth_802Q::update_tci(eth_802Q *obj, int tci) {

	obj->TCI[0] = tci >> 8;
	obj->TCI[1] = tci & 0x00ff;
	//check in TCPDump if correct!

}

void eth_802Q::serialize_eth_802Q(eth_802Q* obj, u_char* buff) {

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

	*buff = obj->TPID[0];
	++buff;
	*buff = obj->TPID[1];
	++buff;

	*buff = obj->TCI[0];
	++buff;
	*buff = obj->TCI[1];
	++buff;

	*buff = obj->Type[0];
	++buff;
	*buff = obj->Type[1];
	++buff;

}

void eth_802Q::rand_pcp(u_char* &buff) {
	u_char temp = (u_char)(buff[14]);
	temp = (temp & 0x1F);
	buff[14] = 0;
	u_char pcp = rand() % 8;
	u_char PCPbin = pcp << 5;
	u_char tempbin = (PCPbin | temp);
	buff[14] = tempbin;
}

void eth_802Q::rand_dei(u_char* &buff) {
	u_char dei = rand() % 2;
//    u_char temp = (u_char)buff[14];
	if (dei == 0) {
		buff[14] = (buff[14] ^ ((0x10)));
	}
}

void eth_802Q::rand_vid(u_char* &buff) {
	u_char temp = (u_char)(buff[14]);
	temp = (temp & 0xF0);
	int vid = rand() % 4096;
	buff[15] = vid;
	u_char vid14 = vid >> 8;
	u_char tempbin = (vid14 | temp);
	buff[14] = tempbin;
}

void eth_802Q::random_mac_addr(u_char* &buffer, bool _rand_dest_flag,
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

