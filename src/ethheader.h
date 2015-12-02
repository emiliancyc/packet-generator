/*
 * ethheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#include <sys/types.h>

#ifndef ETHHEADER_H_
#define ETHHEADER_H_

class eth_header {
public:
	eth_header();
	eth_header(char* _dest, char* _src);
	virtual ~eth_header();

private:
	u_char Preamble[7]; // NOT USED (rawSocket creates preamble itself)
	u_char SFD;			// NOT USED (same here)
	u_char DestMac[6];
	u_char SrcMac[6];
	u_char Type[2];
	u_char* Data;		// NOT USED
	u_char CRC[4];		// NOT USED (rawSocket calculates value itself)
};

#endif /* ETHHEADER_H_ */
