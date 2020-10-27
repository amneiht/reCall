#include <alog.h>
#include <asip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "asip/help.h"
sip_state state ;
void exit_handle(void *arg) {
	printf("ket thuc sip \n");
}
static void signal_handler(int sig) {
	re_printf("terminating on signal %d...\n", sig);
}

int main(int argc, char **argv) {
	libre_init();
	asip_user *ua;
//	asip_newUser(&ua, "dccan_159", "123456", "sip.linphone.org", 5060);
//	asip_user call = { .name = "dccan_158", .pass = NULL, .proxy =
//			"sip.linphone.org" };
//	asip_newUser(&ua, "60001", "zxcvbnm", "129.vgisc.com", 65060);
//	asip_newUser(&ua, "test2", "123456", "192.168.3.9", 5060);
	asip_newUser(&ua, "test2", "123456", "192.168.10.246", 5060);
	asip_user call = { .name = "test", .pass = NULL, .proxy = "192.168.10.246" };
	asip_conf_t *con;
	asip_initConf(&con, exit_handle, NULL);
	int err = asip_login(con, ua);
	if (err) {
		log_err("can not login");
		goto out;
	}
//	sleep(5);
//	asip_call(con, ua, &call);
	/* main loop */
	re_main(signal_handler);
	out: asip_deleteConf(con);
	asip_deleteUser(ua);
	libre_close();
	return 0;
}
