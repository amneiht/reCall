/*
 * help.c
 *
 *  Created on: Oct 5, 2020
 *      Author: dccan
 */

#include "help.h"
#include <string.h>
#include <stdlib.h>
char* asip_convert(char *lp) {
	int dem = strlen(lp);
	char *res;
	res = malloc((dem + 1) * sizeof(char));
	for (int i = 0; i <= dem; i++) {
		res[i] = lp[i];
	}
	return res;
}
void help_memcpy(void **dst, const void *src, size_t lg) {
	*dst = malloc(lg);
	char *cp = *dst;
	char *st = src;
	for (int i = 0; i < lg; i++) {
		cp[i] = st[i];
	}
}
