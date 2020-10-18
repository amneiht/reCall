/*
 * help.h
 *
 *  Created on: Oct 5, 2020
 *      Author: dccan
 */

#ifndef ASIP_HELP_H_
#define ASIP_HELP_H_

#include <asip.h>
#include <bits/stdint-uintn.h>

/**
 * struct info to send rtp
 */
typedef struct info_rtp {
	struct sa *addr; ///addr to send
	asip_stream_t *stream; /// audio stream ;
} info_rtp_t;
/**
 * to close a call
 */
typedef struct call_info {
	asip_conf_t *con; /// poniter to save configtion to close socket
	asip_stream_t *st; /// pointer to audio stream
} call_info;
/**
 *  struct for handle for accept a call
 *
 */
typedef struct call_accept {
	asip_user *ua; /// user
	asip_conf_t *con; /// program configtion
} call_accept;
#ifdef __cplusplus
extern "C" {
#endif

char* asip_convert(char *lp);
int auth_handler(char **username, char **password, const char *realm, void *arg);
uint32_t time_milisecond();




// test call function
void* call_sendRtp(void *arg);
//static void call_rtpHandle(const struct sa *src, const struct rtp_header *hdr,
//		struct mbuf *mb, void *arg);
void connect_handler(const struct sip_msg *msg, void *arg);
void decode_sdp(struct mbuf *mb, const char *str);

void sipCall_info(struct sip *sip, const struct sip_msg *msg, void *arg);
int sipCall_answer(const struct sip_msg *msg, void *arg);
void sipCall_progress(const struct sip_msg *msg, void *arg);
void sipCall_info(struct sip *sip, const struct sip_msg *msg, void *arg);
void sipCall_refer(struct sip *sip, const struct sip_msg *msg, void *arg);
void sipCall_close(int err, const struct sip_msg *msg, void *arg);
int sipCall_offer(struct mbuf **descp, const struct sip_msg *msg, void *arg);
void sipCall_entab(const struct sip_msg *msg, void *arg);
void call_rtpHandle(const struct sa *src, const struct rtp_header *hdr,
		struct mbuf *mb, void *arg);
void* call_sendRtp(void *arg) ;
#ifdef __cplusplus
}
#endif
#endif /* ASIP_HELP_H_ */
