/*
 * eth802Q.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef ETH802Q_H_
#define ETH802Q_H_

#include <sys/types.h>
#include "includes.h"
#include <string>

class eth_802Q {
public:
	eth_802Q();
    eth_802Q(std::string _dest, std::string _src, int _pcp, int _dei, int _vid);
	virtual ~eth_802Q();
    void update_src_mac(eth_802Q *obj, std::string src);
    void update_dest_mac(eth_802Q *obj, std::string dest);
    void update_tci(eth_802Q *obj, int tci);
    void serialize_eth_802Q(eth_802Q* obj, u_char* buff);

private:
//	u_char Preamble[7]; // NOT USED (rawSocket creates preamble itself)
//	u_char SFD;			// NOT USED (same here)
	u_char DestMac[6];
	u_char SrcMac[6];
	u_char TPID[2];
	u_char TCI[2];
    u_char Type[2];
//	u_char* Data;		// NOT USED
//	u_char CRC[4];		// NOT USED (rawSocket calculates value itself)
};

#endif /* ETH802Q_H_ */

