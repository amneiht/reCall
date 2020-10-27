/*
 * testMbuf.c
 *
 *  Created on: Oct 15, 2020
 *      Author: dccan
 */

#include <re.h>
#include <stdlib.h>
#include <stdio.h>
#include <g711.h>
int main(int argc, char **argv) {
	libre_init();
	int lm = 12;
	struct mbuf *mb = mbuf_alloc(200);
	int16_t *lp = malloc(200 * sizeof(int16_t));
	for (int i = 0; i < 200; i++) {
		lp[i] = 1000 + i;
	}
	int8_t *pt = &(mb->buf[20]);
	UlawArrayEncode(lp, 180, pt);
	for (int i = 0; i < 200; i++) {
		fprintf(stderr, "%d ", mb->buf[i]);
	}
	libre_close();
}
