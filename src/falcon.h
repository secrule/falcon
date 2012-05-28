/*
 * falcon.h
 *
 *  Created on: 2012-5-2
 *      Author: root
 */

#ifndef FALCON_H_
#define FALCON_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <regex.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/inotify.h>

#include "db_mgr.h"

#ifdef __cplusplus
extern "C" {
#endif

// falcon daemon server contxt
typedef struct FALCONContext
{
	int		pid;
	char	pid_file[256];
	char	id[16];
	int	is_stop;
	int 	is_daemon;
	time_t	current_time;
}FALCONCtx;

int	falcon_init();
int falcon_finish();
int falcon_daemonize();
int falcon_signal_init();
void falcon_signal_handler(int);
int falcon_process();
int falcon_tick();
int falcon_help();
int falcon_write_pid(const char*);
int falcon_read_pid(const char*);
int falcon_is_exist();
int falcon_kill();

extern FALCONCtx* gSvrdCtx;

#ifdef __cplusplus
}
#endif

#endif /* FALCON_H_ */
