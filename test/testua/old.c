#include <alog.h>
#include <asip.h>
#include <stdio.h>
#include <stdlib.h>
void exit_handle(void *arg) {
	printf("nothing");
}
int main(int argc, char **argv) {
	libre_init();
	asip_user *ua;
	asip_newUser(&ua, "dccan_159", "123456", "sip.linphone.org");
	asip_conf_t *con;
	asip_initConf(&con, exit_handle, NULL);
	asip_login(con, ua);
	re_main(NULL);
	libre_close();
	return 0;
}
