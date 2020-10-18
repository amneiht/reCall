/*
 * call.c
 *
 *  Created on: Oct 10, 2020
 *      Author: dccan
 */

#include <alog.h>
#include <asip.h>
#include <g711.h>
#include <stdlib.h>
#include <stdio.h>
#include "help.h"

struct sipsess *sess;
asip_stream_t* asip_initStream();
static const pa_sample_spec sample = { .format = PA_SAMPLE_S16LE, .rate = 8000,
		.channels = 1 };
void connect_handler(const struct sip_msg *msg, void *arg) {
	//TODO
	log_debug("sipsock connect handle");
	call_accept *cp = arg;
	asip_conf_t *con = cp->con;
	int err;
	if (sess) {
		/* Already in a call */
		(void) sip_treply(NULL, con->sip, msg, 486, "Busy Here");
		return;
	}
	bool got_offer = (mbuf_get_left(msg->mb) > 0);
	if (got_offer) {
		err = sdp_decode(con->sdp, msg->mb, true);
		if (err) {
			log_err("unable to decode SDP offer: %s\n", strerror(err));
		}
	}
	struct mbuf *mb;
	err = sdp_encode(&mb, con->sdp, !got_offer);
	if (err) {
		log_err("unable to encode SDP: %s\n", strerror(err));
		goto out;
	}
	asip_user *ua = cp->ua;
	asip_stream_t *stream = asip_initStream();
	call_info *callf = malloc(sizeof(call_info));
	callf->con = con;
	callf->st = stream;
	err = sipsess_accept(&sess, con->sock, msg, 200, "OK", ua->name,
			"application/sdp", mb, auth_handler, ua, false, sipCall_offer,
			sipCall_answer, sipCall_entab, sipCall_info, sipCall_refer,
			sipCall_close, callf, NULL, NULL);
	if (err) {
		log_err("can not anwser");
	}
	mem_deref(mb);
	out: return;
}
void decode_sdp(struct mbuf *mb, const char *str) {
	struct sa laddr;
	struct sdp_session *sess;
	struct sdp_media *au;
	net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 12454);
	sdp_session_alloc(&sess, &laddr);
	sdp_media_add(&au, sess, sdp_media_audio, 9222, sdp_proto_rtpavp);
	sdp_decode(sess, mb, true);
	const struct sa *sp = sdp_media_raddr(au);
	fprintf(stderr, "%s port %d \n", str, ntohs(sp->u.in.sin_port));
	mem_deref(sess);
}

asip_stream_t* asip_initStream() {
	asip_stream_t *res = NULL;
	res = calloc(1, sizeof(asip_stream_t));
	int err = -1;
	res->in = pa_simple_new(NULL, "input", PA_STREAM_RECORD, NULL, "pika",
			&sample, NULL, NULL, &err);
//	log_debug("input err is :%d", err);
	res->out = pa_simple_new(NULL, "out", PA_STREAM_PLAYBACK, NULL, "pika",
			&sample, NULL, NULL, &err);
//	log_debug("output err is :%d", err);
	if (res->in == NULL || res->out == NULL) {
		log_debug("loi tao luong");
		return NULL;
	}
//	err = rtp_listen()
	return res;
}
int16_t dout[5000]; /// output encode data
int16_t din[5000]; /// input data
int8_t ein[5000]; /// encode data array

void call_rtpHandle(const struct sa *src, const struct rtp_header *hdr,
		struct mbuf *mb, void *arg) {
//	log_info("pt is %d", hdr->pt);
	static int dls = 0;
	if (dls < 4) {
		log_info("buff size is %ld", mb->size);
		log_info("rtp payload is %d", hdr->pt);
		log_info("rtp header is %d", hdr->ver);
		log_info("seq is : %d", hdr->seq);
		log_info("ccrs is %d", hdr->cc);
		log_info("time stamp %u", hdr->ts);
		log_info("data is : %u",hdr->ssrc);
		log_info();
		dls++;
	}
	pa_simple *out = arg;
	static int size = sizeof(int16_t);
	int lg = mb->size - RTP_HEADER_SIZE; //so data trong rtp
	UlawArrayDecode((int8_t*) &(mb->buf[RTP_HEADER_SIZE]), lg, dout);
	pa_simple_write(out, dout, lg * size, NULL);
}
void asip_closeStream(asip_stream_t *d) {
	d->run = false;
	pthread_join(d->ptx, NULL); // doi het luong
	pa_simple_free(d->in);
	pa_simple_free(d->out);
	mem_deref(d->rtp);
	free(d);
}
int asip_initSession(asip_conf_t *conf, asip_user *ua) {
	call_accept *call = malloc(sizeof(call_info));
	call->con = conf;
	call->ua = ua;
	int err = sipsess_listen(&(conf->sock), conf->sip, 32, connect_handler,
			call);
	return err;
}
void asip_sessionHandle(const struct sip_msg *msg, void *arg) {
	log_info("code is %d", msg->scode);
}
void sipCall_info(struct sip *sip, const struct sip_msg *msg, void *arg) {
	log_info("call ");
}
void sipCall_refer(struct sip *sip, const struct sip_msg *msg, void *arg) {
	log_info("refer is : ");
}
void sipCall_progress(const struct sip_msg *msg, void *arg) {

	log_info("progress code is %d", msg->scode);
//	decode_sdp(msg->mb, __func__);
}

int sipCall_answer(const struct sip_msg *msg, void *arg) {

	log_info("answer code is %d", msg->scode);
//	decode_sdp(msg->mb, __func__);
	return 0;
}
int sipCall_offer(struct mbuf **descp, const struct sip_msg *msg, void *arg) {
	log_info("has a offered ");
//	decode_sdp(msg->mb, __func__);
	return 0;
}
void sipCall_close(int err, const struct sip_msg *msg, void *arg) {
	log_info("close sip call");
//	decode_sdp(msg->mb, __func__);
	call_info *p = arg;
	asip_stream_t *st = p->st;
	asip_closeStream(st);
	mem_deref(sess);
	sess = NULL;
	free(arg);
	re_cancel();
}
/**
 * entable a sip call
 * @param msg sip mess
 * @param arg pointer to call_info structure
 */
void sipCall_entab(const struct sip_msg *msg, void *arg) {
	log_info("tao ket noi");
//	decode_sdp(msg->mb, __func__);
	call_info *callf = arg;
	struct mbuf *mb;
	mb = msg->mb;
	sdp_decode(callf->con->sdp, mb, true);
	const struct sa *addr = sdp_media_raddr(callf->con->media);
	if (addr == NULL) {
		log_err("no remote address ");
		return;
	}
	log_info("Connect Rtp port is %d", ntohs(addr->u.in.sin_port));
	asip_stream_t *str = callf->st;
	info_rtp_t *rs = malloc(sizeof(info_rtp_t));
	rs->addr = malloc(sizeof(struct sa));
	memcpy(rs->addr, addr, sizeof(struct sa));
	rs->stream = str;
	pthread_create(&(str->ptx), NULL, call_sendRtp, rs);
//	free(callf);
}
uint32_t time_milisecond() {
	struct timeval time;
	gettimeofday(&time, NULL);
	uint64_t lp = time.tv_sec * 1000000 + time.tv_usec;
	return (uint32_t) lp;
}
/**
 *  send audio thread fuuntion
 * @param arg info_rtp_t data
 */
void* call_sendRtp(void *arg) {
	log_info("luong gui data");
	int lg = 160; // 20ms
	info_rtp_t *rs = arg;
	static int s16 = sizeof(int16_t);
	asip_stream_t *str = rs->stream;
	struct sa *sa = rs->addr;
	struct mbuf *buf = mbuf_alloc(lg + RTP_HEADER_SIZE);
	buf->size = lg + RTP_HEADER_SIZE;
	buf->end = lg + RTP_HEADER_SIZE;
	free(rs);
	int read = lg * s16;
	int err;
	int8_t *point = (int8_t*) &(buf->buf[RTP_HEADER_SIZE]);
	buf->pos = RTP_HEADER_SIZE;
	log_info("send size is %d", buf->size);
	while (str->run) {
		err = pa_simple_read(str->in, din, read, NULL);
		if (err) {
			log_err("loi read data");
		} else {
			//TODO

			UlawArrayEncode(din, lg, point);
			buf->pos = RTP_HEADER_SIZE;
			rtp_send(str->rtp, sa, false, false, 0, time_milisecond(), buf);
//			pa_simple_write(str->out,din,read,NULL);
		}
	}
	free(sa);
	return NULL;
}

int asip_call(asip_conf_t *con, asip_user *ua, asip_user *call) {
	if (sess != NULL) {
		log_info("you are calling ");
		return -1;
	}
	int err;
	char name[200];
	asip_getUserUri(call, name);
	log_info("call name is %s", name);
	char to[200];
	struct mbuf *mb;
	asip_getUserUri(ua, to);
	asip_stream_t *stream = asip_initStream();
	stream->run = true;
//	struct sdp_session *sdp;
	struct sdp_media *sdp_media;
	if (stream == NULL) {
		log_err("can not alloc a stream");
		return -1;
	}
	struct sa laddr;
	err = net_default_source_addr_get(AF_INET, &laddr);
	if (err) {
		log_err("local address error: %s\n", strerror(err));
		return -1;
	}
	/* listen on random port */
	sa_set_port(&laddr, 0);
	err = rtp_listen(&(stream->rtp), IPPROTO_UDP, &laddr, 10000, 30000, false,
			call_rtpHandle, NULL, stream->out);
	if (err) {
		log_err("rtp listen false \n");
	}

//	err = sdp_session_alloc(&sdp, &laddr);
	if (err) {
		re_fprintf(stderr, "sdp session error: %s\n", strerror(err));
		goto out;
	}

	/* add audio sdp media, using port from RTP socket */
	err = sdp_media_add(&sdp_media, con->sdp, sdp_media_audio,
			sa_port(rtp_local(stream->rtp)), sdp_proto_rtpavp);
	con->media = sdp_media;
	log_info("my rtp port is %d", sa_port(rtp_local(stream->rtp)));
	if (err) {
		re_fprintf(stderr, "sdp media error: %s\n", strerror(err));
		goto out;
	}

	/* add G.711 sdp media format */
	err = sdp_format_add(NULL, sdp_media, false, "0", "PCMU", 8000, 1,
	NULL, NULL, NULL, false, NULL);
	err = sdp_format_add(NULL, sdp_media, false, "8", "PCMA", 8000, 1,
	NULL, NULL, NULL, false, NULL);
	if (err) {
		re_fprintf(stderr, "sdp format error: %s\n", strerror(err));
		goto out;
	}

	/* create SDP offer */
	err = sdp_encode(&mb, con->sdp, true);
	if (err) {
		re_fprintf(stderr, "sdp encode error: %s\n", strerror(err));
		goto out;
	}
	if (con->sock == NULL) {
		err = asip_initSession(con, ua);
		if (err) {
			log_err("can not init sipsession socket");
			goto out;
		}
	}
	//TODO
	call_info *callf = malloc(sizeof(call_info));
	callf->con = con;
	callf->st = stream;
	err = sipsess_connect(&sess, con->sock, name, ua->name, to, ua->name, NULL,
			0, "application/sdp", mb, auth_handler, ua, false, sipCall_offer,
			sipCall_answer, sipCall_progress, sipCall_entab, sipCall_info,
			sipCall_refer, sipCall_close, callf, NULL, NULL);
	goto end;
	out: asip_closeStream(stream);
	end: mem_deref(mb);
	return err;
}
void testpl() {
//	pa_simple *s = NULL;
}
