/*
 * time.c
 *
 *  Created on: Oct 12, 2020
 *      Author: dccan
 */

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
	struct timeval time;
	uint64_t lp;
	uint32_t lo ;
	for (int i = 0; i < 20; i++) {
		gettimeofday(&time, NULL);
		lp = time.tv_sec * 1000000 + time.tv_usec ;
		lo = (uint32_t)lp ;
		printf("%u\n",lo);
		sleep(1);
	}
	return (0);
}
