/*
 * eth802Q.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include "eth802Q.h"
#include <iostream>

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

eth_802Q::eth_802Q(char* _dest, char* _src, int _pcp, int _dei, int _vid) {

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

	this->TPID[0] = 0x81;
	this->TPID[1] = 0x00;

	int PCPbin = 0, DEIbin = 0;
	if (_pcp != NULL)
		PCPbin = _pcp << 13;

	if (_dei != NULL)
		int DEIbin = _dei << 12;

	if (_vid == NULL)
		_vid = 1;

	int TCIbin = PCPbin | DEIbin | _vid;
	this->TCI[0] = TCIbin >> 8;
	this->TCI[1] = TCIbin & 0x00ff;
	this->Type[0] = 0x08;
	this->Type[1] = 0x00;
}

eth_802Q::~eth_802Q() {
	/* delete this->Type;
	delete this->TCI;
	delete this->TPID;
	delete this->DestMac;
	delete this->SrcMac; */
	delete this;
}

void eth_802Q::update_src_mac(eth_802Q *obj, const char *src) {
        for (int i = 0; i < 6; ++i) {
            obj->SrcMac[i] = (u_char) src[i];
        }
}

void eth_802Q::update_dest_mac(eth_802Q *obj, const char *dest) {
        for (int i = 0; i < 6; ++i) {
            obj->DestMac[i] = (u_char) dest[i];
        }
}
void eth_802Q::update_tci(eth_802Q *obj, int tci) {
    obj->TCI[0] = tci >> 8;
    obj->TCI[1] = tci & 0x00ff;
}
