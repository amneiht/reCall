#include <re.h>
#include <alog.h>
#include <stdlib.h>
char info[] = "v=0\r\n"
		"o=dccan_158 1515 1963 IN IP4 192.168.10.10\r\n"
		"s=Talk\r\n"
		"c=IN IP4 54.37.202.229\r\n"
		"t=0 0\r\n"
		"a=Ik:d30BgX6RBeP5KA4GKwrYp3tAF6Rmi3jUuO6C41cyTM8=\r\n"
		"m=audio 33446 RTP/AVP 0 8\r\n";
int main(int argc, char **argv) {
	libre_init();
	struct sdp_session *sdp;
	struct sdp_media *media;
	struct mbuf * mb = mbuf_alloc(2048);
	struct sa laddr ;
	sa_init(&laddr, AF_INET);
	sdp_session_alloc(&sdp,&laddr);
	sdp_decode(sdp,mb,true);
//	sdp_media_add(&media)
	libre_close();
}

