/**
 * testSdp.c
 *
 *  Created on: Oct 22, 2020
 *      Author: dccan
 */
#include <re.h>
#include <alog.h>
#include <string.h>
char info[] =
		"v=0\r\n"
				"o=test 1743 2494 IN IP4 192.168.10.10\r\n"
				"s=Talk\r\n"
				"c=IN IP4 192.168.10.10\r\n"
				"t=0 0\r\n"
				"a=rtcp-xr:rcvr-rtt=all:10000 stat-summary=loss,dup,jitt,TTL voip-metrics\r\n"
				"m=audio 7078 RTP/AVP 96 97 98 0 8 18 101 99 100\r\n"
				"a=rtpmap:96 opus/48000/2\r\n"
				"a=fmtp:96 useinbandfec=1\r\n"
				"a=rtpmap:97 speex/16000\r\n"
				"a=fmtp:97 vbr=on\r\n"
				"a=rtpmap:98 speex/8000\r\n"
				"a=fmtp:98 vbr=on\r\n"
				"a=fmtp:18 annexb=yes\r\n"
				"a=rtpmap:101 telephone-event/48000\r\n"
				"a=rtpmap:99 telephone-event/16000\r\n"
				"a=rtpmap:100 telephone-event/8000\r\n"
				"a=rtcp-fb:* trr-int 1000\r\n"
				"a=rtcp-fb:* ccm tmmbr\r\n";

int main(int argc, char **argv) {
	libre_init();
	struct sdp_session *sdp;
	struct sdp_media *au;
	struct sa laddr;
//	struct sa *la = &laddr;
	int err = net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 0);
	err = sdp_session_alloc(&sdp, &laddr);
	if (err) {
		log_err("can not alocate sdp");
	}
	struct mbuf *mb = mbuf_alloc(2048);
	mbuf_write_str(mb, info);
	mb->pos = 0;
	err = sdp_decode(sdp, mb, false);
	if (err) {
		log_err("can not decode sdp mess");
	}
	err = sdp_media_add(&au, sdp, "ma", 10, sdp_media_audio);
	if (err) {
		log_err("can not add media");
	}
	const struct sa *sp;
	sp = sdp_media_raddr(au);
	log_info("port is %d", ntohl(sp->u.in.sin_port));
	mem_deref(sdp);
	libre_close();
	return err;
}
