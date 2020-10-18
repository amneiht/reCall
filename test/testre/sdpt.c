/*
 * sdpt.c
 *
 *  Created on: Oct 10, 2020
 *      Author: dccan
 */

#include <stdio.h>
#include <re.h>
char info[] =
		"SIP/2.0 200 Ok\r\n"
				"Via: SIP/2.0/TCP 192.168.2.246:46569;received=118.70.233.52;branch=z9hG4bK2369f064662573f4;rport=36514\r\n"
				"Record-Route: <sips:sip6.linphone.org:5223;lr>\r\n"
				"Record-Route: <sip:54.37.202.229:5228;transport=tcp;lr>\r\n"
				"From: \"dccan_159\" <sip:dccan_159@sip.linphone.org>;tag=2818c1304d89efe8\r\n"
				"To: <sip:dccan_158@sip.linphone.org>;tag=EDgJf7h\r\n"
				"Call-ID: 62bd308b00d8c156\r\n"
				"CSeq: 25312 INVITE\r\n"
				"Contact: <sip:dccan_158@sip.linphone.org;gr=urn:uuid:85d2c1f9-cf7b-00c4-bb26-3a56134c98c6>;+org.linphone.specs=\"ephemeral,groupchat,lime\"\r\n"
				"User-Agent: LinphoneAndroid/4.3.1 (Mi A2 Lite) LinphoneSDK/4.4.2 (tags/4.4.2^0)\r\n"
				"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO, UPDATE\r\n"
				"Supported: replaces, outbound, gruu\r\n"
				"Content-Type: application/sdp\r\n"
				"Content-Length: 187\r\n"
				"\r\n"
				"v=0\r\n"
				"o=dccan_158 3442 3533 IN IP4 192.168.2.10\r\n"
				"s=Talk\r\n"
				"c=IN IP4 54.37.202.229\r\n"
				"t=0 0\r\n"
				"a=Ik:d30BgX6RBeP5KA4GKwrYp3tAF6Rmi3jUuO6C41cyTM8=\r\n"
				"m=audio 14414 RTP/AVP 0\r\n"
				"a=rtpmap:96 PCMU/8000\r\n";
void sig_handle(int i) {
	printf("close main");
	re_cancel();
}
int main(int argc, char **argv) {
	libre_init();
	struct mbuf *mb = mbuf_alloc(2048);
	mbuf_write_str(mb, info);
	mb->pos = 0;
	struct sa laddr;
	struct sdp_session *sess;
	struct sdp_media *au;
	net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 12454);
	sdp_session_alloc(&sess, &laddr);
	sdp_media_add(&au, sess, sdp_media_audio, 9222, sdp_proto_rtpavp);
	sdp_decode(sess, mb, true);
	const struct sa *sp = sdp_media_raddr(au);

	printf("%d \n", ntohs(sp->u.in.sin_port));
//	re_main(sig_handle);
	libre_close();
}

