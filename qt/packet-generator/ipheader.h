/*
 * ipheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef IPHEADER_H_
#define IPHEADER_H_

#include <sys/types.h>
#include <string>

class ip_header {
public:
	ip_header();
	ip_header(std::string _dest, std::string _src);
	virtual ~ip_header();
	void updateSrcIP(ip_header *obj, std::string _src);
	void updateDestIP(ip_header *obj, std::string _dest);
	void updateValues(ip_header *obj, u_char _ToS, unsigned short int _length,
			unsigned short int _id, unsigned short int _flags,
			unsigned short int _offset, u_char _ttl, u_char _protocol);
	void serializeIP(ip_header* obj, u_char* buff);
    unsigned short int calculateChecksum(ip_header *obj, u_char* buff, int n);
	void randID(ip_header *obj, u_char* &buffer, bool _vlan);
	void randTTL(ip_header *obj, u_char* &buffer, bool _vlan);
	void randIP(ip_header *obj, u_char* &buffer, bool _vlan, bool _src_ip_flag,
			bool _dest_ip_flag);
	void updateLength(unsigned short int _length);
	unsigned int getSrcIP();
	unsigned int getDestIP();
	u_char getProtocol();
	void setProtocol(u_char _protocol);
	unsigned short int getLength();
	void setLength(unsigned short int _length);
	void updateProtocol(ip_header* obj, u_char _protocol);

private:
	u_char ver;					//version of used IP protocol, by default IPv4
	u_char ihl;                     //IP header length, usually 5 (no options)
	u_char ToS;	//(combined DSCP and ECN) type of service field, values from 0 to 7
	unsigned short int length; 		//total packet lenght in bytes
	unsigned short int id; 			//ID required to reassemble packet fragments
	unsigned short int flags;					//fragmentation flags
	unsigned short int offset; //sequence number for bytes in packet when reassembling
	u_char ttl; 					//maximum life of packet
	u_char protocol; 				//protocol used in the data portion
    unsigned short int checksum; 	//16bit checksum of IP header
	unsigned int sourceip;
	unsigned int destip;
	u_char* buff = NULL;
};

#endif /* IPHEADER_H_ */
