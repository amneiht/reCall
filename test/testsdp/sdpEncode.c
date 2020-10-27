/*
 * sdpEncode.c
 *
 *  Created on: Oct 26, 2020
 *      Author: dccan
 */

#include <re.h>
#include <stdio.h>
int main(int argc, char **argv) {
	libre_init();
	int err;
	struct sa laddr;
	struct sdp_session *sdp;
	struct sdp_media *audio;
	net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 0);
	sdp_session_alloc(&sdp, &laddr);
	sdp_media_add(&audio, sdp, "audio", 6789, sdp_media_audio);
	err = sdp_format_add(NULL, audio, false, "0", "PCMU", 8000, 1,
	NULL, NULL, NULL, false, NULL);
	err = sdp_format_add(NULL, audio, false, "8", "PCMA", 8000, 1,
	NULL, NULL, NULL, false, NULL);
	if (err) {
		re_fprintf(stderr, "sdp format error: %s\n", strerror(err));
		return -1;
	}
	struct mbuf *mb;
	sdp_encode(&mb, sdp, true);
	fprintf(stderr, "sdp is:\n%s\n", mb->buf);
	libre_close();
	return 0;
}

