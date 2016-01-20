/*
 * tcpheader.h
 *
 *  Created on: Dec 2, 2015
 *      Author: emilian
 */

#ifndef TCPHEADER_H_
#define TCPHEADER_H_
#include <sys/types.h>

class tcp_header {
public:
	tcp_header();
	virtual ~tcp_header();
    void serialize_tcp(u_char* buff, tcp_header* obj);
};

#endif /* TCPHEADER_H_ */
