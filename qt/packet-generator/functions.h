/*
 * functions.h
 *
 *  Created on: Dec 4, 2015
 *      Author: emilian
 */

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


int isMAC(char*);
char* mac_addr(char*);
void random_mac_addr(u_char*, bool, bool);
void rand_pcp(u_char*);
unsigned short ByteSwap(unsigned short x);



#endif /* FUNCTIONS_H_ */
