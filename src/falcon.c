/*
 ============================================================================
 Name        : falcon.c
 Author      : secrule.com
 Version     : v0.1
 Copyright   : GPL
 Description : Ansi-style
 ============================================================================
 */
#include "falcon.h"
#include "config_file.h"

#define    MAX_VAR_NUM          (128)
#define    MAX_VAR_NAME_LEN     (128)
#define    MAX_VAR_VALUE_LEN    (MAX_PATH_LEN)

FALCONCtx* gSvrdCtx = NULL;

int php_keyword_num;
int php_virus_num;
char *phpKeywords[1024];
char *phpVirusname[1024];
const char *phpVirusfeature[1024];

extern char ga_variables[MAX_VAR_NUM][MAX_VAR_NAME_LEN + 1];
extern char ga_values[MAX_VAR_NUM][MAX_VAR_VALUE_LEN + 1];

void eventHandler(struct inotify_event *event, char *keywords[1024],
		int keywordsNum);
int findKeyWord(char *keyword, FILE *fp, int *line, char *source);
char* findKeywords(char **keywords, int wordsNum, char *fileName);

int virusHandler(struct inotify_event *event, const char *phpVirusfeature[1024],
		int php_virus_num);
char* findViruses(const char **viruses, int virusesNum, char *fileName);

int main(int argc, char *argv[]) {

	int opt;
	time_t now; //实例化time_t结构
	struct tm *timenow; //实例化tm结构指针
	char *monitorpath;
	char *vdir;
	char *vdirtmp;
	char *vdirnew;
	char delims[] = "/";

	time(&now);

	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	timenow = localtime(&now);
	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
//	printf("Start   time: %s\n", asctime(timenow));
	fprintf(stderr, "Start   time: %s\n", asctime(timenow));
	//上句中asctime函数把时间转换成字符，通过printf()函数输出

	parse_config_file("../src/conf/global.conf");
	monitorpath = (char *) malloc(sizeof(char) * 1024);
	strcpy(monitorpath, get_config_var("monitorpath"));
//	printf("monitor path:%s\n", monitorpath);
	fprintf(stderr, "monitor path:%s\n", monitorpath);

	parse_config_file("../src/conf/global.conf");
	vdir = (char *) malloc(sizeof(char) * 1024);
	vdirnew = (char *) malloc(sizeof(char) * 1024);
	vdirtmp = (char *) malloc(sizeof(char) * 1024);
	strcpy(vdirtmp, get_config_var("monitorpath"));
//	strReplace(vdir, "/", "/virus", 1024);
	vdirnew = strtok(vdirtmp, delims);
//	vdir = strtok(NULL, delims);
	strcat(vdirnew, "/virus/");
	strcat(vdir, "/");
	strcat(vdir, vdirnew);
//	printf("virus back path:%s\n", vdir);
	
	fprintf(stderr, "virus bak path:%s\n", vdir);

	if (falcon_init() < 0) {
		fprintf(stderr, "falcon server init fail\n");
		return -1;
	}

	snprintf(gSvrdCtx->id, 16, "%s", "secrule");
	snprintf(gSvrdCtx->pid_file, 256, "/var/tmp/falcon_%s.pid", "secrule");
	// load main parameters
//	static struct option szLongOptions[] =
//			{ { "daemon", 0, NULL, 'D' }, { "help", 0, NULL, 'H' }, { "version",
//					0, NULL, 'V' }, { 0, 0, 0, 0 } };
	static struct option szLongOptions[] = { { "help", 0, NULL, 'H' }, {
			"version", 0, NULL, 'V' }, { 0, 0, 0, 0 } };

	int ch;
	while ((ch = getopt_long(argc, argv, "h:v", szLongOptions, NULL)) != EOF) {
		switch (ch) {
		case 'D':
//			gSvrdCtx->is_daemon = 1;
			break;

		case 'H':
			falcon_help();
			free(gSvrdCtx);
			exit(0);

		case 'V':
			fprintf(stderr, "falcon compile at %s %s\n", __DATE__, __TIME__);
			free(gSvrdCtx);
			exit(0);
		}
	}

	if (optind >= argc) {
		falcon_help();
		free(gSvrdCtx);
		exit(0);
	}

	for (opt = optind; opt < argc; opt++) {
		if (0 == strcasecmp(argv[opt], "start")) {
			// if process already exsit, then warning & exit
			if (falcon_is_exist() > 0) {
				fprintf(stderr, "falcon server %s already started. \n",
						gSvrdCtx->id);
				free(gSvrdCtx);
				exit(0);
			}

			fprintf(stderr, "falcon server start. \n");
			break;
		} else if (0 == strcasecmp(argv[opt], "stop")) {
			fprintf(stderr, "falcon server stop. \n");

			// if process not exsit, then warning & exit
			if (falcon_is_exist() <= 0) {
				fprintf(stderr, "falcon server %s not started. \n",
						gSvrdCtx->id);
			}

			// kill previous process
			else if (falcon_kill() < 0) {
				fprintf(stderr, "kill previous falcon server %s fail. \n",
						gSvrdCtx->id);
			} else {
				fprintf(stderr, "kill previous falcon server %s success. \n",
						gSvrdCtx->id);
			}

			free(gSvrdCtx);
			exit(0);
		} else {
			falcon_help();
			free(gSvrdCtx);
			exit(0);
		}
	}

	fprintf(stderr, "init falcon server success!\n");

	if (gSvrdCtx->is_daemon) {
		falcon_daemonize();
	}
	gSvrdCtx->pid = getpid();

	if (falcon_write_pid(gSvrdCtx->pid_file) < 0) {
		fprintf(stderr, "falcon server %s init fail. \n", gSvrdCtx->id);
		free(gSvrdCtx);
		exit(0);
	}

	while (!gSvrdCtx->is_stop) {
		gSvrdCtx->current_time = time(NULL);

		if (falcon_process() < 0) {
			usleep(100);
		}

		falcon_tick();
	}

	falcon_finish();

	fprintf(stderr, "falcon server is exit\n");

	return EXIT_SUCCESS;
}

void eventHandler(struct inotify_event *event, char *keywords[php_keyword_num],
		int keywordsNum) {
	char content[65535];
	char *tmpkeywords;
	char fullPathTemp[1024];
	int len;
	char *Keywordsfile;

	MYSQL *connection;

	inotifytools_snprintf(fullPathTemp, 1024, event, "%w%f");
	memset(content, 0, 65535);

	tmpkeywords = (char *) malloc(sizeof(char) * 65535);

	if (access(fullPathTemp, F_OK) == 0) {
		if (event->mask & IN_CREATE) {
			if (event->mask & IN_ISDIR) {

			} else {
				tmpkeywords = findKeywords(keywords, keywordsNum, fullPathTemp);
				if (tmpkeywords != NULL) {
					if ((len = strlen(tmpkeywords)) != 0) {
//						inotifytools_printf(event, "%T %w%f %e\t");
						inotifytools_fprintf(stderr, event, "%T %w%f %e\n");
						strcat(content, " contains keyword(s):\r\n");
						strcat(content, tmpkeywords);
						printf("%s length=%d\n", content, len);
					}

					connection = conn_init();

					Keywordsfile = (char *) malloc(sizeof(char) * 65535);
					memset(Keywordsfile, 0, 65535);

					strcat(Keywordsfile, fullPathTemp);
					strcat(Keywordsfile, "\r\n");
					strcat(Keywordsfile, tmpkeywords);

					printf("Keywordsfile : %s\n", Keywordsfile);
					insert_item(connection, "f_monitor", "发现新增可疑文件",
							Keywordsfile, "3", 0);
					conn_close(connection);
				}
			}

			return;
		}

		if (event->mask & IN_MODIFY) {
			if (event->mask & IN_ISDIR) {

			} else {
				tmpkeywords = findKeywords(keywords, keywordsNum, fullPathTemp);
				if (tmpkeywords != NULL) {
					if ((len = strlen(tmpkeywords)) != 0) {
//						inotifytools_printf(event, "%T %w%f %e\t modify ");
						inotifytools_fprintf(stderr, event, "%T %w%f %e\n");
						strcat(content, " contains keyword(s): ");
						strcat(content, tmpkeywords);
						printf("%s length=%d\n", content, len);
					}

					connection = conn_init();

					Keywordsfile = (char *) malloc(sizeof(char) * 65535);
					memset(Keywordsfile, 0, 65535);

					strcat(Keywordsfile, fullPathTemp);
					strcat(Keywordsfile, "\r\n");
					strcat(Keywordsfile, tmpkeywords);

					insert_item(connection, "f_monitor", "发现文件被修改",
							Keywordsfile, "3", 0);
					conn_close(connection);
				}
			}
			return;
		}
	}

	if (event->mask & IN_DELETE) {
		if (event->mask & IN_ISDIR) {

		} else {
			connection = conn_init();
			inotifytools_fprintf(stderr, event, "%T %w%f %e\n");
			insert_item(connection, "f_monitor", "发现文件被删除", fullPathTemp, "1",
					0);
			conn_close(connection);
		}
		return;
	}

}

int virusHandler(struct inotify_event *event,
		const char *phpVirusfeature[php_virus_num], int php_virus_num) {
	char content[65535];
	char *tmpkeywords;
	char *virusfile;
	char fullPathTemp[1024];
	char vfilename[1024];
	char *vdir;
	char *vdirtmp;
        char *vdirnew;
        char delims[] = "/";		
	char *newpath;
	int len;

	MYSQL *connection;

	inotifytools_snprintf(fullPathTemp, 1024, event, "%w%f");
	inotifytools_snprintf(vfilename, 1024, event, "%f");

	parse_config_file("../src/conf/global.conf");
        vdir = (char *) malloc(sizeof(char) * 1024);
        vdirnew = (char *) malloc(sizeof(char) * 1024);
        vdirtmp = (char *) malloc(sizeof(char) * 1024);
        strcpy(vdirtmp, get_config_var("monitorpath"));
        vdirnew = strtok(vdirtmp, delims);
        strcat(vdirnew, "/virus/");
        strcpy(vdir, "/");
        strcat(vdir, vdirnew);
	
	if (access(vdir, 0) != 0) {
		mkdir(vdir, S_IRWXU || S_IRWXG || S_IRWXO); 
	}

	memset(content, 0, 65535);

	if (access(fullPathTemp, F_OK) == 0) {
		if (event->mask & IN_CREATE) {
			if (event->mask & IN_ISDIR) {
				inotifytools_watch_recursively(fullPathTemp, IN_ALL_EVENTS);
			} else {
				tmpkeywords = findViruses(phpVirusfeature, php_virus_num,
						fullPathTemp);
				if (tmpkeywords != NULL) {
					if ((len = strlen(tmpkeywords)) != 0) {
//						inotifytools_printf(event, "%T %w%f %e\t ");
						inotifytools_fprintf(stderr, event, "%T %w%f %e\n");
						strcat(content, " contains virus(es): ");
						strcat(content, tmpkeywords);
//						printf("%s length=%d\n", content, len);

						newpath = (char *) malloc(sizeof(char) * 1024);
						strncpy(newpath, vdir, 1024);
//						strcat(newpath, "/");
						strcat(newpath, vfilename);
						newpath = strReplace(newpath, "php", "virus", 1024);

//						printf("newpath : %s\n", newpath);

						if (rename(fullPathTemp, newpath) != 0) {
							printf("virus file : %s\n", fullPathTemp);
							printf("quarantine file : %s\n", newpath);
							perror("move virus file");
						}

						connection = conn_init();

						virusfile = (char *) malloc(sizeof(char) * 65535);
						memset(virusfile, 0, 65535);

						strcat(virusfile, newpath);
						strcat(virusfile, "\r\n");
						strcat(virusfile, tmpkeywords);

						insert_item(connection, "f_monitor", "发现后门文件",
								virusfile, "3", 1);
						conn_close(connection);
//						printf("IN_CREATE Done\n");
						return 1;
					}
				}

			}
			return 0;
		}

		if (event->mask & IN_MODIFY) {
			if (event->mask & IN_ISDIR) {

			} else {
				tmpkeywords = findViruses(phpVirusfeature, php_virus_num,
						fullPathTemp);
				if (tmpkeywords != NULL) {
					if ((len = strlen(tmpkeywords)) != 0) {
//						inotifytools_printf(event, "%T %w%f %e\t modify ");
						inotifytools_fprintf(stderr, event, "%T %w%f %e\n");
						strcat(content, " contains virus(es): ");
						strcat(content, tmpkeywords);
						printf("%s length=%d\n", content, len);

						newpath = (char *) malloc(sizeof(char) * 1024);
						strncpy(newpath, vdir, 1024);
						strcat(newpath, "/");
						strcat(newpath, vfilename);
						newpath = strReplace(newpath, "php", "virus", 1024);
						printf("newpath : %s\n", newpath);

						if (rename(fullPathTemp, newpath) != 0) {
							printf("virus file : %s\n", fullPathTemp);
							printf("quarantine file : %s\n", newpath);
							perror("move virus file");
						}
						connection = conn_init();

						virusfile = (char *) malloc(sizeof(char) * 65535);
						memset(virusfile, 0, 65535);

						strcat(virusfile, newpath);
						strcat(virusfile, "\r\n");
						strcat(virusfile, tmpkeywords);
						strcat(virusfile, "\r\n");

						insert_item(connection, "f_monitor", "发现后门文件",
								virusfile, "3", 1);
						conn_close(connection);

						return 1;
					}
				}
			}
			return 0;
		}
	}
	return -1;
}

/*find all keywords in the file*/
char* findKeywords(char *keywords[php_keyword_num], int wordsNum,
		char *fileName) {
	int i = 0;
	int exitno = 0;
	FILE * fp = 0;
	int line;
	char *lineno;
	char *source;
	static char tmpkeywords[1024];
	memset(tmpkeywords, 0, 1024);
	lineno = (char *) malloc(sizeof(char) * 1024);
	source = (char *) malloc(sizeof(char) * 1024);

	if (fileName == NULL)
		return NULL;
	fp = fopen(fileName, "r");

	for (; i < wordsNum; i++) {
		if (findKeyWord(keywords[i], fp, &line, source) == 0) {
			sprintf(lineno, "line %d :", line);
			strcat(tmpkeywords, lineno);
			strcat(tmpkeywords, " ");
			strcat(tmpkeywords, source);
			strcat(tmpkeywords, "\r\n");

			exitno++;
			if (exitno == 3) {
				strcat(tmpkeywords, "......");
				break;
			}
		}

	}
	strcat(tmpkeywords, "\0");

	fclose(fp);
	return tmpkeywords;
}

/*find single keyword in the file*/
int findKeyWord(char *keyword, FILE *fp, int *line, char *source) {
	int ret = 0;
	size_t nmatch = 10;
	regex_t reg;
	char buf[1024];
	regmatch_t pm[10];
	int i = 1;

	if (fp == NULL || keyword == NULL)
		return -1;
	rewind(fp);
	ret = regcomp(&reg, keyword, 0);
	if (ret != 0)
		return -1;

	while (fgets(buf, 1024, fp) != NULL) {

		if ((ret = strlen(buf)) > 0 && buf[ret - 1] == '\n')
			buf[32] = 0;
		ret = regexec(&reg, buf, nmatch, pm, 0);
		if (ret == 0) {
			*line = i;
			strncpy(source, buf, 128);
			return 0;
		}

		i++;
	}

	regfree(&reg);
	return -1;
}

/*find all keywords in the file*/
char* findViruses(const char *viruses[php_virus_num], int virusesNum,
		char *fileName) {
	int i = 0, j = 0;
	char delims[] = ",";
	char *featureItem = NULL;
	FILE * fp = 0;
	static char tmpkeywords[1024];
	char *virustmp;
	int line;
	char *lineno;
	char *source;

	char *phpVirusItem[1024][1024];

	memset(tmpkeywords, 0, 1024);
	lineno = (char *) malloc(sizeof(char) * 1024);
	source = (char *) malloc(sizeof(char) * 65535);

	if (fileName == NULL)
		return NULL;
	fp = fopen(fileName, "r");

	for (i = 0; i < virusesNum; i++) {
		j = 0;

		virustmp = (char *) malloc(sizeof(char) * 1024);
		strncpy(virustmp, viruses[i], 1024);

		featureItem = strtok(virustmp, delims);

		while (featureItem != NULL) {
			phpVirusItem[i][j] = (char *) malloc(sizeof(char) * 1024);
			strncpy(phpVirusItem[i][j], featureItem, 1024);

			if (findKeyWord(phpVirusItem[i][j], fp, &line, source) == 0) {
				sprintf(lineno, "line %d :", line);
				strcat(tmpkeywords, lineno);
				strcat(tmpkeywords, " ");
				strcat(tmpkeywords, source);
				strcat(tmpkeywords, "\r\n");
			}

			j++;
			featureItem = strtok(NULL, delims);
		}
	}

	strcat(tmpkeywords, "\0");

	fclose(fp);
	return tmpkeywords;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int falcon_init() {
	gSvrdCtx = (FALCONCtx*) malloc(sizeof(FALCONCtx));

	if (!gSvrdCtx) {
		fprintf(stderr, "malloc server context fail, no free memory..\n");
		return -1;
	}

	memset(gSvrdCtx, 0, sizeof(FALCONCtx));
	gSvrdCtx->current_time = time(NULL);
	gSvrdCtx->is_stop = 0;
	gSvrdCtx->is_daemon = 0;

	// TODO:

	return 0;
}

int falcon_finish() {
	// TODO:

	free(gSvrdCtx);
	return 0;
}

int falcon_process() {
	// TODO:

	return -1;
}

int falcon_tick() {
	int i;
	char *monitorpath;
	char *monitorfiletype;
	// TODO:

	parse_config_file("../src/conf/global.conf");
	monitorpath = (char *) malloc(sizeof(char) * 1024);
	strcpy(monitorpath, get_config_var("monitorpath"));
	monitorfiletype = (char *) malloc(sizeof(char) * 1024);
	strcpy(monitorfiletype, get_config_var("monitorfiletype"));

	//keyword
	php_keyword_num = parse_config_file("../src/conf/phpkeywords.conf");

	for (i = 0; i < php_keyword_num; i++) {
		phpKeywords[i] = (char *) malloc(sizeof(char) * 1024);
		strncpy(phpKeywords[i], ga_variables[i], 1024);
	}

	//virus

	php_virus_num = parse_config_file("../src/conf/phpvirus.conf");

	for (i = 0; i < php_virus_num; i++) {
		phpVirusname[i] = (char *) malloc(sizeof(char) * 1024);
		strncpy(phpVirusname[i], ga_variables[i], 1024);
		phpVirusfeature[i] = (char *) malloc(sizeof(char) * 1024);
		strncpy((char *__restrict) phpVirusfeature[i], ga_values[i], 1024);
	}

	// initialize and watch the entire directory tree from the current working
	// directory downwards for all events
	if (!inotifytools_initialize()
			|| !inotifytools_watch_recursively(monitorpath, IN_ALL_EVENTS)) {
		fprintf(stderr, "%s\n", strerror(inotifytools_error()));
		return -1;
	}

	// set time format
	inotifytools_set_printf_timefmt("%F %T");

	struct inotify_event * event = inotifytools_next_event(-1);

	while (event) {

		if (((strstr(event->name, ".php") != NULL
				&& strstr(event->name, ".swp") == NULL
				&& strstr(event->name, ".swx") == NULL
				&& strstr(event->name, "~") == NULL) || (event->mask & IN_ISDIR))) {
			if (virusHandler(event, phpVirusfeature, php_virus_num) != 1) {
				eventHandler(event, phpKeywords, php_keyword_num);
			}
		}
		event = inotifytools_next_event(-1);
	}

	return -1;
}

int falcon_daemonize() {
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	int i, pid;
	if ((pid = fork())) {
		exit(0);
	} else if (pid < 0) {
		exit(1);
	}
	setsid();

	signal(SIGHUP, SIG_IGN);
	if ((pid = fork())) {
		exit(0);
	} else if (pid < 0) {
		exit(1);
	}

	for (i = 0; i < 255; i++) {
		close(i);
	}

	umask(0);
	return 0;
}

int falcon_signal_init() {
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGALRM, &act, NULL);

	act.sa_handler = falcon_signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	return 0;
}

void falcon_signal_handler(int signal) {
	if (SIGINT == signal //ctrl + c, kill -2
	|| SIGUSR1 == signal // quit, kill -10
	|| SIGKILL == signal // kill -9
	|| SIGQUIT == signal) // kill -3
			{
		fprintf(stderr, "catch signal SIGINT, server shutdown\n");
		gSvrdCtx->is_stop = 1;
	}
}

int falcon_help() {
	const char* szHelpInfo = "falcon start-up parameters: \n"
			"	--help \n"
			"	--version \n"
			"	[start | stop] \n";
	fprintf(stderr, szHelpInfo);
	return 0;
}

int falcon_write_pid(const char* szPidFile) {
	if (!szPidFile) {
		fprintf(stderr, "no pid file to write!\n");
		return -1;
	}

	int fd = open(szPidFile, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd < 0) {
		fprintf(stderr, "open(create) pid file[%s] fail!\n", szPidFile);
		return -1;
	}

	// lock pid file
	if (0 != flock(fd, LOCK_EX | LOCK_NB)) {
		fprintf(stderr, "lock to pid file[%s] fail!\n", szPidFile);
		close(fd);
		return -1;
	}

	char szPid[16];
	memset(szPid, 0, 16);
	snprintf(szPid, 16, "%d", gSvrdCtx->pid);

	int len = strlen(szPid);
	if (len != write(fd, szPid, len)) {
		fprintf(stderr, "write pid file[%s] fail!\n", szPidFile);
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

int falcon_read_pid(const char* szPidFile) {
	if (!szPidFile) {
		fprintf(stderr, "no pid file to read!\n");
		return -1;
	}

	int fd = open(szPidFile, O_RDONLY, 0644);
	if (fd < 0) {
		return -1;
	}

	char szPid[16];
	memset(szPid, 0, 16);
	if (-1 == read(fd, szPid, sizeof(szPid))) {
		fprintf(stderr, "read pid file[%s] fail!\n", szPidFile);
		close(fd);
		return -1;
	}

	int pid = atoi(szPid);
	close(fd);
	return pid;
}

// return 1, exist
// return 0, not exist
// return -1, check fail
int falcon_is_exist() {
	if (!gSvrdCtx || !gSvrdCtx->pid_file) {
		fprintf(stderr, "invalid falcon context!\n");
		return -1;
	}

	int pid = falcon_read_pid(gSvrdCtx->pid_file);

	// no pid read, then not existed
	if (pid < 0) {
		return 0;
	}

	char szProcDir[256];
	memset(szProcDir, 0, sizeof(szProcDir));
	snprintf(szProcDir, 256, "/proc/%d", pid);

	// check whether process existed in /proc
	struct stat stStat;
	int ret = stat(szProcDir, &stStat);
	if (ret < 0 || !S_ISDIR(stStat.st_mode)) {
		return 0;
	}

	// check process name whether equals to falcon
	snprintf(szProcDir, 256, "/proc/%d/status", pid);
	FILE* fd = fopen(szProcDir, "r");
	if (fd) {
		char buff[64];
		char name_str[64];
		memset(name_str, 0, sizeof(name_str));
		memset(buff, 0, sizeof(buff));

		// read first line("Name")
		if (NULL != fgets(buff, sizeof(buff), fd)) {
			// dest name found
			sscanf(buff, "%*s %s", name_str);
			if (0 == strcmp(name_str, "falcon")) {
				fclose(fd);
				return 1;
			}
		}
		fclose(fd);
	}
	return 0;
}

int falcon_kill() {
	if (!gSvrdCtx || !gSvrdCtx->pid_file) {
		fprintf(stderr, "invalid falcon context!\n");
		return -1;
	}

	int ret = -1;
	if (falcon_is_exist() > 0) {
		int pid = falcon_read_pid(gSvrdCtx->pid_file);
		ret = kill(pid, SIGINT);
	}

	// delete pid file
	unlink(gSvrdCtx->pid_file);
	return ret;
}
