/*
 * functions.c
 *
 *  Created on: Dec 4, 2015
 *      Author: emilian
 */
#include "functions.h"
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

unsigned short ByteSwap(unsigned short x)
{
	x  = ( ((x<<8) & 0xFFFF) | ((x >>8) & 0xFFFF));		// swaps the bytes
	return x;
}

//little help about calcuating IP header checksum:
//http://www.unix.com/programming/117551-calculate-ip-header-checksum-manually.html
//SOURCE:
//http://web.eecs.utk.edu/~cs594np/unp/checksum.html
short unsigned int checksum(u_char* buff, int n)
{
	long sum = 0;
	unsigned short* buff2 = (unsigned short*) buff;

	while (n>0){
		sum += *(buff2++);
		if(sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		n--;
	}

	if(n)
		sum += (unsigned short)* buff;

	while(sum>>16)
		sum = (sum & 0xFFFF) + (sum >>16);

	return ~sum;
}

int isMAC(char *s){
	int i=0;
	for(;i<17; i++){
//		if(i % 3 != 2 && !isxdigit(s[i]))
//			return 1;
		if(i % 3 == 2 && s[i] != ':')
			return 1;
	}
	if(s[17] != '\0')
		return 1;
	return 0;
}

char* mac_addr(char* addr)
{
//	unsigned int intMac[6];
//	memset(&intMac, 0, 6*sizeof(unsigned int));
//	char* p = malloc(6*sizeof(char));
//	memset(p, 0, 6*sizeof(char));
//	sscanf(addr,"%x:%x:%x:%x:%x:%x", &intMac[0], &intMac[1], &intMac[2],&intMac[3], &intMac[4], &intMac[5]);	 //using sscanf to read heximal values from string and put in into unsigned int array
//	int i = 0;
//	for(; i<6; i++)
//	{
//		p[i]= (char) intMac[i];		//type casting to u_char - for compatibility
//	}
//	return p;
}

void random_mac_addr(u_char *packet, bool RandomDestFlag, bool RandomSrcFlag)
{
	int i=6,k=6;
	if(RandomDestFlag)
		i=0;
	if(RandomSrcFlag)
		k=12;
	for(;i<k;++i)
	{
		if(i == 0 || i==6)
		{
			//this block of code makes sure that MAC is unicast to avoid problems
//			int r=rand()%255;
//			if(r%2)
//				packet[i]=(r-1);
//			else
//				packet[i]=r;
		}
//		else
//			packet[i]= rand()%255;
	}
}

void rand_pcp(u_char* buff){
	int pcp;
	int temp = (int)buff[14];
	buff[14] = 0;
//	pcp = rand()%8;
//	u_char PCPbin = pcp << 5;
//	u_char tempbin = PCPbin | (temp<<8);
//	buff[14] = tempbin;
}

