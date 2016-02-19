/*
 * udpheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef UDPHEADER_H_
#define UDPHEADER_H_

#include <sys/types.h>

class udp_header {
public:
	udp_header();
	virtual ~udp_header();
    void serialize_udp(udp_header* obj, u_char* buff);
};

#endif /* UDPHEADER_H_ */
