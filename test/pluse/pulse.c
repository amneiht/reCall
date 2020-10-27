/*
 * pulse.c
 *
 *  Created on: Oct 14, 2020
 *      Author: dccan
 */

#include <pulse/simple.h>
#include <stdio.h>
#include <alog.h>
#include <g711.h>
int main(int argc, char **argv) {
	pa_sample_spec sample = { .format = PA_SAMPLE_S16LE, .rate = 8000,
			.channels = 2 };
	pa_simple *out;
	pa_simple *in;
	int err;
	out = pa_simple_new(NULL, "a1", PA_STREAM_PLAYBACK, NULL, "pika", &sample,
	NULL, NULL, &err);
	in = pa_simple_new(NULL, "a2", PA_STREAM_RECORD, NULL, "pika", &sample,
	NULL, NULL, &err);
	if (out == NULL || in == NULL) {
		log_err("false install voice");
	}
	int16_t lib[2000];
	int i = 0;
	fprintf(stderr, "helo");
	int8_t en[2000];
	int16_t dec[2000];
	while (i < 1000) {
		err = pa_simple_read(in, lib, 2000 * 2, NULL);
		if (err) {
			log_err("can record voice");
			goto out;
		}
		UlawArrayEncode(lib, 2000, en);
		UlawArrayDecode(en, 2000, dec);
		err = pa_simple_write(out, dec, 2000 * 2, NULL);
		if (err) {
			log_err("can record voice");
			goto out;
		}
		i++;
	}
	out: pa_simple_free(in);
	pa_simple_free(out);
	return 0;
}
