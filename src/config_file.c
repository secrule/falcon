/*
 ============================================================================
 Name        : redconf.c
 Author      : secrule
 Version     :
 Copyright   : GPL
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>

#include "config_file.h"

#define    MAX_VAR_NUM          (128)
#define    MAX_VAR_NAME_LEN     (128)
#define    MAX_VAR_VALUE_LEN    (MAX_PATH_LEN)

#define COMMENT_CHARACTER   '#'
#define LINE_SIZE    (768)

char ga_variables[MAX_VAR_NUM][MAX_VAR_NAME_LEN + 1];
char ga_values[MAX_VAR_NUM][MAX_VAR_VALUE_LEN + 1];
int g_var_num = 0;

void get_dir_path_of_file(char *file, char *dir_path) {
	char *temp;
	strncpy(dir_path, file, MAX_PATH_LEN);
	temp = strrchr(dir_path, '/');
	temp[0] = '\0';

}

void remove_trailing_chars(char *path, char c) {
	size_t len;

	len = strlen(path);
	while (len > 0 && path[len - 1] == c)
		path[--len] = '\0';
}

int get_key(char **line, char **key, char **value) {
	char *linepos;
	char *temp;

	linepos = *line;
	if (!linepos) {
		return -1;
	}

	/* skip whitespace */
	while (isspace(linepos[0]))
		linepos++;

	if (linepos[0] == '\0') {
		return -1;
	}

	/* get the key */
	*key = linepos;
	while (1) {
		linepos++;
		if (linepos[0] == '\0') {
			return -1;
		}
		if (isspace(linepos[0]))
			break;
		if (linepos[0] == '=')
			break;
	}

	/* terminate key */
	linepos[0] = '\0';

	while (1) {
		linepos++;
		if (linepos[0] == '\0') {
			return -1;
		}
		if (isspace(linepos[0]))
			continue;
		if (linepos[0] == '=')
			continue;
		break;
	}

	/* get the value*/
	if (linepos[0] == '"') {
		linepos++;
	} else {
		return -1;
	}
	*value = linepos;

	temp = strchr(linepos, '"');
	if (!temp) {
		return -1;
	}

	temp[0] = '\0';
	return 0;
}

int parse_config_file(char *path_to_config_file) {
	char line[LINE_SIZE + 2];
	char *bufline;
	char *linepos;
	char *variable;
	char *value;
	size_t cur;
	size_t count;
	int lineno;
	int retval = 0;
	g_var_num = 0;

	FILE *cfg_file = fopen(path_to_config_file, "r");
	if (NULL == cfg_file) {
		printf("can't open '%s' as config file: %s", path_to_config_file,
				strerror(errno));
		goto EXIT;
	}

	/* loop through the whole file */
	lineno = 0;
	cur = 0;
	while (NULL != fgets(line, sizeof(line), cfg_file)) {
		lineno++;
		bufline = line;
		count = strlen(line);

		if (count > LINE_SIZE) {
			printf("line too long, conf line skipped %s, line %d",
					path_to_config_file, lineno);
			continue;
		}

		/* eat the whitespace */
		while ((count > 0) && isspace(bufline[0])) {
			bufline++;
			count--;
		}
		if (count == 0)
			continue;

		/* see if this is a comment */
		if (bufline[0] == COMMENT_CHARACTER)
			continue;

		memcpy(line, bufline, count);
		line[count] = '\0';

		linepos = line;
		retval = get_key(&linepos, &variable, &value);
		if (retval != 0) {
			printf("error parsing %s, line %d:%d\n", path_to_config_file,
					lineno, (int) (linepos - line));
			continue;
		}

		if (g_var_num >= MAX_VAR_NUM) {
			printf("too many vars in  %s, line %d:%d", path_to_config_file,
					lineno, (int) (linepos - line));
			continue;
		}

		if (strlen(variable) > MAX_VAR_NAME_LEN) {
			printf("var name to long %s, line %d:%d", path_to_config_file,
					lineno, (int) (linepos - line));
			continue;
		}

		if (strlen(value) > MAX_VAR_VALUE_LEN) {
			printf("value to long %s, line %d:%d", path_to_config_file, lineno,
					(int) (linepos - line));
			continue;
		}

		strncpy(ga_variables[g_var_num], variable,
				sizeof(ga_variables[g_var_num]));
		remove_trailing_chars(value, '/');
		strncpy(ga_values[g_var_num], value, sizeof(ga_values[g_var_num]));
		g_var_num++;
		continue;

	}
	EXIT: fclose(cfg_file);
	return g_var_num;
}

char * get_config_var(char *var_name) {
	int i;
	for (i = 0; i < g_var_num; i++) {
		if (strcasecmp(ga_variables[i], var_name) == 0) {
			return ga_values[i];
		}
	}
	printf("get %s failed\n", var_name);
	return NULL;
}

void print_all_vars() {
	int i;

	for (i = 0; i < g_var_num; i++) {
		printf("%s = %s\n", ga_variables[i], ga_values[i]);
	}
}

void print_keys() {
	int i;

	for (i = 0; i < g_var_num; i++) {
		printf("key[%d] = %s\n", i, ga_variables[i]);
	}
}

void print_values() {
	int i;

	for (i = 0; i < g_var_num; i++) {
		printf("values[%d] = %s\n", i, ga_values[i]);
	}
}

char *strReplace(char *src, const char *oldstr, const char *newstr, int len)
{
     char *needle;
    char *tmp;
   if (strlen(oldstr) == strlen(newstr) && strcmp(oldstr, newstr) == 0)
    {
        return 0;
    }

    while ((needle = strstr(src, oldstr)) && (needle - src <= len))
    {
        tmp = (char*)malloc(strlen(src) + (strlen(newstr) - strlen(oldstr)) +1);
        strncpy(tmp, src, needle-src);
        tmp[needle-src] = '\0';
        strcat(tmp, newstr);
        strcat(tmp, needle+strlen(oldstr));
        src = strdup(tmp);
        free(tmp);
    }
    return src;
}

