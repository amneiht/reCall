/*
 * testLocalCall.c
 *
 *  Created on: Oct 16, 2020
 *      Author: dccan
 */

#include "asip/help.h"
#include <asip.h>
#include <stdlib.h>
#include <pthread.h>
#include <re.h>
int main(int argc, char **argv) {
	libre_init();
	struct rtp_sock *send;
	asip_stream_t *str = asip_initStream();
	int err;
	if (str == NULL)
		goto out;
	str->run = true;
	struct sa laddr;
	net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 0);
	err = rtp_listen(&(str->rtp), IPPROTO_UDP, &laddr, 10000, 30000, false,
			call_rtpHandle, NULL, str->out);
	if (err) {
		log_err("could  not alloc rtp");
	}
	err = rtp_listen(&(send), IPPROTO_UDP, &laddr, 10000, 30000, false,
			call_rtpHandle, NULL, str->out);
	sa_set_port(&laddr, sa_port(rtp_local(str->rtp)));
	struct info_rtp *rs = malloc(sizeof(info_rtp_t));
	rs->addr = malloc(sizeof(struct sa));
	memcpy(rs->addr, &laddr, sizeof(struct sa));
	rs->stream = str;
	pthread_create((&str->ptx), NULL, call_sendRtp, rs);
	re_main(asip_signalHandle);
	out: libre_close();
}
