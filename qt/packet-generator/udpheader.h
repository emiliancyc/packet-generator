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
    void serialize_udp(u_char* buff, udp_header* obj);
};

#endif /* UDPHEADER_H_ */
