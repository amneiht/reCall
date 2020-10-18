/*
 * pika.h
 *
 *  Created on: Oct 1, 2020
 *      Author: dccan
 */

#ifndef ASIP_H_
#define ASIP_H_

#include <pulse/simple.h>
#include <re.h>
#include <alog.h>
#include <pthread.h>
typedef struct asip_user {
	char *name;
	char *pass;
	char *proxy;
	int port;
} asip_user;
typedef struct asip_conf {
	struct sip *sip;
	struct sipreg *reg;
	struct sipsess_sock *sock;
	struct sip_lsnr **lsnr;
	struct sdp_session *sdp;
	struct sdp_media *media;
} asip_conf_t;
/**
 * sip audio stream
 */
typedef struct asip_stream {
	pa_simple *in; /// audio input stram
	pa_simple *out; /// audio out put stream
	pthread_t ptx; /// thread of send stream
	bool run; /// run control
	struct rtp_sock *rtp; /// rtp socket
} asip_stream_t;
#ifdef __cplusplus
extern "C" {
#endif
void asip_initConf(asip_conf_t **conf, void *exit_handler, void *arg);
void asip_deleteConf(asip_conf_t *conf);
/**
 * init new session for call
 * @param conf  config of sio
 * @param listen  listen funtion for each comming in sip session
 * @param arg varial of funtion
 * @return 0 or true , otherwise is false
 */
int asip_initSession(asip_conf_t *conf, asip_user *ua);
void asip_newUser(asip_user **ua, char *user, char *pass, char *proxy);
void asip_deleteUser(asip_user *ua);
void asip_getUserUri(asip_user *d, char *p);
void asip_getRegUri(asip_user *d, char *p);

int asip_login(asip_conf_t *con, asip_user *ua);

int asip_call(asip_conf_t *con, asip_user *ua, asip_user *call);

uint32_t time_milisecond();

void asip_signalHandle(int sig);

asip_stream_t* asip_initStream();
//int asip_listenCall(asip_conf_t *con, asip_user *ua);
#ifdef __cplusplus
}
#endif
#endif /* ASIP_H_ */
