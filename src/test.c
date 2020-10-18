#include <alog.h>
#include <asip.h>
#include <stdio.h>
#include <stdlib.h>

#define USER    "dccan_159"
#define PASS    "123456"
#define DOMAIN  "sip.linphone.org"

void exit_handle(void *arg) {
	printf("ket thuc sip \n");
}
static void signal_handler(int sig) {
	re_printf("terminating on signal %d...\n", sig);
}

int main(int argc, char **argv) {
	libre_init();
	asip_user *ua;
	asip_newUser(&ua, "dccan_159", "123456", "sip.linphone.org");
	asip_user call = { .name = "dccan_158", .pass = NULL, .proxy =
			"sip.linphone.org" };
	asip_conf_t *con;
	asip_initConf(&con, exit_handle, NULL);
//	int err = asip_login(con, ua);
//	if (err) {
//		log_err("can not login");
//		goto out;
//	}
	asip_call(con, ua, &call);
	/* main loop */
	re_main(signal_handler);
	out:
	asip_deleteConf(con);
	asip_deleteUser(ua);
	libre_close();
	return 0;
}
