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
void asip_signalHandle(int sig)
{
log_info("signal is %d",sig);
}
void asip_initConf(asip_conf_t **conf, void *exit_handle, void *arg) {

	int err;
	struct sip *sip;
	struct sa nsv[16];
	struct dnsc *dnsc = NULL;
	struct sa laddr;
	uint32_t nsc;
	nsc = ARRAY_SIZE(nsv);
	/* fetch list of DNS server IP addresses */
	err = dns_srv_get(NULL, 0, nsv, &nsc);
//	re_printf("Fetching list of DNS server IP adresses.\n");
	/* create DNS client */
	err = dnsc_alloc(&dnsc, NULL, nsv, nsc);
//	re_printf("Create DNS client.\n");
	/* create SIP stack instance */
	err = sip_alloc(&sip, dnsc, 32, 32, 32,
			"LibRE UAv" VERSION " (" ARCH "/" OS ")", exit_handle, NULL);
//	re_printf("Create SIP stack instance.\n");
	/* fetch local IP address */
	err = net_default_source_addr_get(AF_INET, &laddr);
//	re_printf("Fetch local IP address.\n");
	/* listen on random port */
	sa_set_port(&laddr, 0);
//	re_printf("Set port.\n");
	/* add supported SIP transports */
	err |= sip_transp_add(sip, SIP_TRANSP_UDP, &laddr);
	err |= sip_transp_add(sip, SIP_TRANSP_TCP, &laddr);
//	re_printf("Add UDP SIP transport.\n");
	mem_deref(dnsc);

	*conf = calloc(1, sizeof(asip_conf_t));
	(*conf)->sip = sip;
	asip_conf_t * cf = *conf ;
	struct sdp_session *sess;
	sdp_session_alloc(&sess, &laddr);
	cf->sdp = sess;

}
void asip_deleteConf(asip_conf_t *conf) {
	mem_deref(conf->reg);
	mem_deref(conf->sip);
	mem_deref(conf->sock);
	mem_deref(conf->lsnr);
	mem_deref(conf->sdp);
	free(conf);
}
void asip_newUser(asip_user **ua, char *user, char *pass, char *proxy) {
	asip_user *us = malloc(sizeof(asip_user));
	us->name = asip_convert(user);
	us->pass = asip_convert(pass);
	us->proxy = asip_convert(proxy);
	us->port = 5060;
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
	sprintf(p, "sip:%s@%s", d->name, d->proxy);
}
/**
 * Get register uri .Example result sip:123.com
 * @param d sip user
 * @param p char array to save result
 */
void asip_getRegUri(asip_user *d, char *p) {
	sprintf(p, "sip:%s", d->proxy);
}
