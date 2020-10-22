/*
 * asip.c
 *
 *  Created on: Oct 5, 2020
 *      Author: dccan
 */
#include <asip.h>
#include <stdlib.h>
#include <alog.h>
#include "help.h"
bool sip_messHandle(const struct sip_msg *msg, void *arg) {
	log_info("handle somcmn thing");
	return true;
}

void asip_signalHandle(int sig) {
	log_info("signal is %d", sig);
}
void asip_initConf(asip_conf_t **conf, void *exit_handle, void *arg) {

	int err;
	struct sip *sip;
	struct sa nsv[16];
	struct dnsc *dnsc = NULL;
	struct sa laddr;
	uint32_t nsc;
	nsc = ARRAY_SIZE(nsv);
	err = dns_srv_get(NULL, 0, nsv, &nsc);
	err = dnsc_alloc(&dnsc, NULL, nsv, nsc);
	err = sip_alloc(&sip, dnsc, 32, 32, 32, "amneiht v1.0", exit_handle, NULL);
	err = net_default_source_addr_get(AF_INET, &laddr);
	sa_set_port(&laddr, 0);
	err |= sip_transp_add(sip, SIP_TRANSP_UDP, &laddr);
	err |= sip_transp_add(sip, SIP_TRANSP_TCP, &laddr);
	mem_deref(dnsc);

	*conf = calloc(1, sizeof(asip_conf_t));
	(*conf)->sip = sip;
	asip_conf_t *cf = *conf;
	struct sdp_session *sess;
	sdp_session_alloc(&sess, &laddr);
	cf->sdp = sess;
//	struct sip_lsnr *lsnr;
//	sip_listen(&lsnr, cf->sip, false, sip_messHandle, NULL);

}
void asip_deleteConf(asip_conf_t *conf) {
	mem_deref(conf->reg);
	mem_deref(conf->sip);
	mem_deref(conf->sock);
	mem_deref(conf->lsnr);
	mem_deref(conf->sdp);
	free(conf);
}
void asip_newUser(asip_user **ua, char *user, char *pass, char *proxy, int port) {
	asip_user *us = malloc(sizeof(asip_user));
	us->name = asip_convert(user);
	us->pass = asip_convert(pass);
	us->proxy = asip_convert(proxy);
	us->port = port;
	*ua = us;
}
void asip_deleteUser(asip_user *ua) {
	free(ua->name);
	free(ua->pass);
	free(ua->proxy);
	free(ua);
}
/**
 * Get sip user uri .Example result sip:abc@123.com
 * @param d sip user
 * @param p char array to save result
 */
void asip_getUserUri(asip_user *d, char *p) {
	sprintf(p, "sip:%s@%s:%d", d->name, d->proxy, d->port);
}
/**
 * Get register uri .Example result sip:123.com
 * @param d sip user
 * @param p char array to save result
 */
void asip_getRegUri(asip_user *d, char *p) {
	sprintf(p, "sip:%s:%d", d->proxy, d->port);
}
