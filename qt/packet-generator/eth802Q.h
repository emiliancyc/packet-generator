/*
 * eth802Q.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef ETH802Q_H_
#define ETH802Q_H_

#include <sys/types.h>

class eth_802Q {
public:
	eth_802Q();
	eth_802Q(char* _dest, char* _src, int _pcp, int _dei, int _vid);
	virtual ~eth_802Q();
    void update_src_mac(eth_802Q *obj, const char* src);
    void update_dest_mac(eth_802Q *obj, const char* dest);
    void update_tci(eth_802Q *obj, int tci);

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

