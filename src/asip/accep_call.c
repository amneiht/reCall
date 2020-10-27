/*
 * accep_call.c
 *
 *  Created on: Oct 23, 2020
 *      Author: dccan
 */
#include <re.h>
#include <asip.h>
#include "help.h"
#include <stdlib.h>
int accept_rtpport = 0 ;
extern struct sipsess *sess;/// from call.c
// for local call
extern sip_state state ;
void connect_handler(const struct sip_msg *msg, void *arg) {
	//TODO
	log_debug("sipsock connect handle");

	call_accept *cp = arg;
	asip_conf_t *con = cp->con;
	int err;
	if (sess != NULL) {
		/* Already in a call */
		(void) sip_treply(NULL, con->sip, msg, 486, "Busy Here");
		return;
	}
	asip_user *ua = cp->ua;
	asip_stream_t *stream = asip_initStream();
	call_info *callf = (call_info*) malloc(sizeof(call_info));
	callf->con = con;
	callf->st = stream;
	//get info
	struct sdp_session *sdp = NULL;
	struct sdp_media *audio;
	struct sa laddr;
	sa_init(&laddr, AF_INET);
	err = sdp_session_alloc(&sdp, &laddr);
	err = sdp_media_add(&audio, sdp, sdp_media_audio, 5004, sdp_proto_rtpavp);
	err = sdp_decode(sdp, msg->mb, true);
	const struct sa *sl = sdp_media_raddr(audio);
	help_memcpy((void**) &(callf->addr), (const void*) sl, sizeof(struct sa));
	err = sipsess_accept(&sess, con->sock, msg, 180, "Ok", ua->name,
			"application/sdp", NULL, auth_handler, ua, false, sipCall_offer,
			sipCall_answer, sipCall_entab, sipCall_info, sipCall_refer,
			sipCall_close, callf, NULL, NULL);

	if (err) {
		log_err("can not anwser");
	}
	mem_deref(audio);
	mem_deref(sdp);
	fprintf(stderr,"you has a call from : %s \n", msg->from.dname.p);
	accept_rtpport = sa_port(rtp_local(stream->rtp));
	state.iscoming = 1 ;
}
