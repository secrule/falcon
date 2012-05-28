/*
 * config_file.h
 *
 *  Created on: 2012-5-4
 *      Author: root
 */

#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <inotifytools/inotify.h>
#include <inotifytools/inotifytools.h>

#define    MAX_PATH_LEN         (512)
#define    MAX_FILE_NAME_LEN    (128)

int parse_config_file(char *path_to_config_file);
char * get_config_var(char *var_name);
void print_all_vars();
void print_keys();
void print_values();
char *strReplace(char *src, const char *oldstr, const char *newstr, int len);
#endif /* CONFIG_FILE_H_ */
