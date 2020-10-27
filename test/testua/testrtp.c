/*
 * testrtp.c
 *
 *  Created on: Oct 14, 2020
 *      Author: dccan
 */

#include <re.h>
#include <asip.h>
#include <alog.h>
#include <stddef.h>
#include <unistd.h>

char buf[] =
		"thien thuong thien ha ,  duy nga doc ton , ngua mat len troi , han doi vo doi";
void rtph(const struct sa *src, const struct rtp_header *hdr, struct mbuf *mb,
		void *arg) {
//	char *str = arg;
	log_info("name is %s", arg);
	log_info("info is %s", &(mb->buf[12]));
	log_info("timestamp is %u", hdr->ts);
	log_info("sep is %d", hdr->seq);
	log_info();
	re_cancel();

}
static void signal_handler(int sig) {
	re_printf("terminating on signal %d...\n", sig);

}
void udp_h(const struct sa *src, struct mbuf *mb, void *arg) {

}

int main(int argc, char **argv) {
	libre_init();
	struct rtp_sock *r1, *r2;
	struct sa laddr;
	net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 0);

	int err = rtp_listen(&r1, IPPROTO_UDP, (const struct sa*) &laddr, 10000,
			30000, false, rtph,
			NULL, "r1");
	int leng = str_len(buf) + RTP_HEADER_SIZE;
	struct mbuf *mb = mbuf_alloc(leng);
	mb->pos = RTP_HEADER_SIZE;
	mbuf_write_str(mb, buf);

//	log_err("size is %d", mb->size);
	err = rtp_listen(&r2, IPPROTO_UDP, (const struct sa*) &laddr, 10000, 30000,
	false, rtph,
	NULL, "r2");
	sa_set_port(&laddr, 0);
	mb->pos = RTP_HEADER_SIZE;
	struct rtp_header hdr;
	memset(&hdr, 0, sizeof(struct rtp_header));
	hdr.x.len = RTP_HEADER_SIZE;
	hdr.ver = 3;
	hdr.pt = 0;
	hdr.seq = 100;
	hdr.ssrc = 1000;
	mb->pos = 0;
	rtp_hdr_encode(mb, &hdr);
	struct udp_sock *udp ;
	udp_listen(&udp,&laddr,udp_h,NULL);
	sa_set_port(&laddr, sa_port(rtp_local(r1)));
	mb->pos =0;
	udp_send(udp,&laddr,mb);
//	rtp_send(r2, &laddr, 0, 0, 96, time_milisecond(), mb);
	log_info("err  is %d", err);
	re_main(signal_handler);
	libre_close();
}
