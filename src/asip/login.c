/*
 * login.c
 *
 *  Created on: Oct 6, 2020
 *      Author: dccan
 */
#include <asip.h>
#include <alog.h>

int auth_handler(char **username, char **password, const char *realm,
		void *arg) {
	asip_user *ua = arg;
	int err = 0;
	err |= str_dup(username, ua->name);
	err |= str_dup(password, ua->pass);
	return err;
}
static void register_handler(int err, const struct sip_msg *msg, void *arg) {
	if (err)
		log_err("register error: %s\n", strerror(err));
	else
		re_printf("register reply: %u %r\n", msg->scode, &msg->reason);
}
int asip_login(asip_conf_t *con, asip_user *ua) {
	int err;
	char reg_uri[200];
	char to_uri[200];
	asip_getRegUri(ua, reg_uri);
	asip_getUserUri(ua, to_uri);
	struct sipreg *reg;
	err = sipreg_register(&reg, con->sip, reg_uri, to_uri, NULL, to_uri, 3600,
			ua->name, NULL, 0, 0, auth_handler, ua, false, register_handler,
			NULL, NULL, NULL);

	if (err) {
		log_err("Can not register a sip\n");
	}
	con->reg = reg;
	return err;
}
