/*
 * ipheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef IPHEADER_H_
#define IPHEADER_H_

#include <sys/types.h>

class ip_header {
public:
	ip_header();
	virtual ~ip_header();
private:
	u_char ver;						//version of used IP protocol, by default IPv4
	u_char ToS;						//type of service field, values from 0 to 7
	u_char length[2]; 				//total packet lenght in bytes
	unsigned short int id; 			//ID required to reassemble packet fragments
	u_char flags;					//fragmentation flags
	u_char offset; 					//sequence number for bytes in packet when reassembling
	u_char ttl; 					//maximum life of packet
	u_char protocol; 				//protocol used in the data portion
	short unsigned int checksum; 	//16bit checksum of IP header
	unsigned int sourceip;
	unsigned int destip;
};

#endif /* IPHEADER_H_ */
