/*
 * ethheader.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "ethheader.h"

eth_header::eth_header() {
	// TODO Auto-generated constructor stub

}

eth_header::eth_header(char* _src, char* _dest) {

	//setting Source MAC address in Ethernet header
	if (_src != NULL) {
		for (int i = 0; i < 6; ++i) {
			this->SrcMac[i] = _src[i];
		}
	} else {
		for (int i = 0; i < 6; ++i) {
			this->SrcMac[i] = 0x00;
		}
	}

	//setting destination MAC address in Ethernet header
	if (_dest != NULL) {
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
	/* delete this->DestMac;
	delete this->SrcMac;
	delete this->Type; */
	delete this;
}

void eth_header::update_src_mac(eth_header *obj, const char *src) {
        for (int i = 0; i < 6; ++i) {
            obj->SrcMac[i] = (u_char) src[i];
        }
}

void eth_header::update_dest_mac(eth_header *obj, const char *dest) {
        for (int i = 0; i < 6; ++i) {
            obj->DestMac[i] = (u_char) dest[i];
        }
}

