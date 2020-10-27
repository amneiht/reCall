/*
 * app.c
 *
 *  Created on: Oct 27, 2020
 *      Author: dccan
 */

#include <re.h>
#include <asip.h>
#include "asip/help.h"
#include <stdlib.h>
#include <pthread.h>
//TODO
sip_state state;
char **buff;
extern struct sipsess *sess; /// from call.c
extern int accept_rtpport; /// from accept_call.c
void** initMarix(int row, int col, int size);
void freeMatrix(void **in, int row);
int praseParameters(char *str, char **mg);
int login(int d, char **mx, asip_conf_t *con, asip_user **ua);
int call(int d, char **mx, asip_conf_t *con, asip_user *ua);
int accept_call();
void* console(void *arg);
int accept_call() {
	if (state.iscoming) {
		state.iscoming = 0;
		int err;
		struct sdp_session *sdp;
		struct sdp_media *audio;
		struct sa addr;
		net_default_source_addr_get(AF_INET, &addr);
		sa_set_port(&addr, 0);
		sdp_session_alloc(&sdp, &addr);
		sdp_media_add(&audio, sdp, sdp_media_audio, accept_rtpport,
				sdp_proto_rtpavp);
		err = sdp_format_add(NULL, audio, false, "0", "PCMU", 8000, 1,
		NULL, NULL, NULL, false, NULL);
		err = sdp_format_add(NULL, audio, false, "8", "PCMA", 8000, 1,
		NULL, NULL, NULL, false, NULL);
		if (err) {
			re_fprintf(stderr, "sdp format error: %s\n", strerror(err));
			goto out;
		}
		struct mbuf *mb;
		err = sdp_encode(&mb, sdp, true);
		if (err) {
			re_fprintf(stderr, "sdp encode error: %s\n", strerror(err));
			goto out;
		}
		//	log_info("mbuf is :\n%s",mb->buf);
		err = sipsess_answer(sess, 200, "OK 123", mb, NULL, NULL);
		mem_deref(mb);
		return 0;
		out: return -1;
	}
	return 0;
}
int call(int d, char **mx, asip_conf_t *con, asip_user *ua) {
	if (state.login) {
		asip_user call;
		log_info("data %s %s", mx[0], mx[1]);
		call.name = mx[1];
		if (d < 2)
			return -2;
		if (d > 2)
			call.proxy = mx[2];
		else
			call.proxy = ua->proxy;
//		call.port = ua->port;
		if (d > 3)
			call.port = atoi(mx[3]);
		return asip_call(con, ua, &call);
	} else
		return -1;
}
int login(int d, char **mx, asip_conf_t *con, asip_user **ua) {
	if (d < 3)
		return -1;
	if (*ua != NULL)
		asip_deleteUser(*ua);
	int port = 5060;
	if (d > 4) {
		log_info("co 4 phan tu");
		port = atoi(mx[4]);
	}
	log_info("data is %s , %s , %s , %d", mx[1], mx[2], mx[3], port);
	asip_newUser(ua, mx[1], mx[2], mx[3], port);
	return asip_login(con, *ua);
}
void** initMarix(int row, int col, int size) {
	void **res;
	res = (void**) malloc(row * sizeof(void*));
	for (int i = 0; i < row; i++) {
		res[i] = (void*) malloc(col * size);
	}
	return res;
}
void freeMatrix(void **in, int row) {
	for (int i = 0; i < row; i++) {
		free(in[i]);
	}
	free(in);
}
int praseParameters(char *str, char **mg) {
	int dem = strlen(str);
	int point = 0;
	int lamp = 0;
	int vt = 0;
	while (str[dem - 1] <= 32) {
		dem = dem - 1;
	}
	for (int i = 0; i < dem; i++) {
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			if (lamp) {
				mg[vt][point] = '\0';
				vt++;
				point = 0;
				lamp = 0;
			}
		} else {
			lamp = 1;
			mg[vt][point] = str[i];
			point++;
		}
	}
	if (point > 0)
	{
		mg[vt][point]='\0';
		vt++;
	}
	return vt;
}
void exit_handle(void *arg) {
	printf("ket thuc sip \n");
	asip_deleteConf((asip_conf_t*) arg);
}
void* console(void *arg) {
	asip_conf_t *con = NULL;
	memset(&state, 0, sizeof(sip_state));
	asip_initConf(&con, exit_handle, con);
	char input[200];
	buff = (char**) initMarix(50, 50, sizeof(char));
	asip_user *ua = NULL; //TODO
	int err;
	while (1) {
		fflush(stdin); // lam sach luong
		fgets(input, 200, stdin);
		int d = praseParameters(input, buff);
		fprintf(stderr,"hello \n");
		if (d > 0) {

			if (strcmp(buff[0], "login") == 0) {
				/// login command : login <user> <pass> <sipserver> [port]
				login(d, buff, con, &ua);
			} else if (strcmp(buff[0], "call") == 0) {
				/// call command : call <user> [sipserver] [port]
				err = call(d, buff, con, ua);
				if (err)
					log_err("loi cmnl");
			} else if (strcmp(buff[0], "a") == 0) {
				accept_call(d, buff, con, ua);
			}

		}
	}
	freeMatrix((void**) buff, 50);
}
int main(int argc, char **argv) {
	libre_init();
	pthread_t ptx;
	pthread_create(&ptx, NULL, console, NULL);
	re_main(asip_signalHandle);
	libre_close();
	return 0;

}
