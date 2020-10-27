/*
 * ua.c
 *
 *  Created on: Oct 6, 2020
 *      Author: dccan
 */

#include <string.h>
#include <re.h>

#define USER    "dccan_159"
#define PASS    "123456"
#define DOMAIN  "sip.linphone.org"
#define URI     "sip:" USER "@" DOMAIN
const char *registrar = "sip:sip.linphone.org";


struct sipsess_sock *sess_sock;

/*!
 * Authentication handler
 */
int auth_handler(char **user, char **pass, const char *realm, void *arg) {
	int err = 0;
	(void) realm;
	(void) arg;

	err |= str_dup(user, USER);
	err |= str_dup(pass, PASS);

	return err;
}

/*!
 * Register handler
 */
void register_handler(int err, const struct sip_msg *msg, void *arg) {
	(void) arg;

	if (err)
		re_printf("register error: %s\n", strerror(err));
	else
		re_printf("register reply: %u %r\n", msg->scode, &msg->reason);
}

/*!
 * Exit handler
 */
void exit_handler(void *arg) {
	(void) arg;

	/* stop libre main loop */
	re_cancel();
}

/*!
 * Signal handler
 */
void signal_handler(int sig) {
	re_printf("terminating on signal %d...\n", sig);

	/* terminate registration */
}
void testre() {
	int err;
	struct sa nsv[16];
	struct dnsc *dnsc = NULL;
	struct sa laddr;
	uint32_t nsc;
	struct sipreg *reg;
	struct sip *sip;
	nsc = ARRAY_SIZE(nsv);

	/* fetch list of DNS server IP addresses */
	err = dns_srv_get(NULL, 0, nsv, &nsc);
	re_printf("Fetching list of DNS server IP adresses.\n");

	/* create DNS client */
	err = dnsc_alloc(&dnsc, NULL, nsv, nsc);
	re_printf("Create DNS client.\n");

	/* create SIP stack instance */
	err = sip_alloc(&sip, dnsc, 32, 32, 32,
			"LibRE UAv" VERSION " (" ARCH "/" OS ")", exit_handler, NULL);
	re_printf("Create SIP stack instance.\n");

	/* fetch local IP address */
	err = net_default_source_addr_get(AF_INET, &laddr);
	re_printf("Fetch local IP address.\n");

	/* listen on random port */
	sa_set_port(&laddr, 0);
	re_printf("Set port.\n");

	/* add supported SIP transports */
	err |= sip_transp_add(sip, SIP_TRANSP_UDP, &laddr);
	err |= sip_transp_add(sip, SIP_TRANSP_TCP, &laddr);
	re_printf("Add UDP SIP transport.\n");

	printf("registrar : %s \n", registrar);
	printf("to uri : %s \n", URI);
	printf("from uri : %s \n", URI);

	printf("USER : %s \n", USER);
	err = sipreg_register(&reg, sip, registrar, URI, NULL, URI, 3600, USER,
	NULL, 0, 0, auth_handler, NULL, true, register_handler, NULL, NULL,
	NULL);
	if (err) {
		re_fprintf(stderr, "register error: [%d] %s\n", err, strerror(err));
		goto exit;
	}

	re_printf("registering <%s>...\n", URI);

	/* main loop */
	err = re_main(signal_handler);

	exit: mem_deref(sess_sock);
	mem_deref(sip);
	mem_deref(dnsc);
}
/*!
 * Main proc
 */
int main(int argc, const char *argv[]) {
	int err = 0; // re functions return holder
	/* initialize libre state */
	err = libre_init();
	if (err) {
		re_fprintf(stderr, "re init failed: %s\n", strerror(err));
		goto exit;
	}
	testre();
	libre_close();
	exit: return err;
}

