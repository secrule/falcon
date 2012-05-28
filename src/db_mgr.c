/*
 * got_db_conn_mgr.c
 *
 *  Created on: 2012-5-9
 *      Author: root
 */

#include "db_mgr.h"

union ipu {
	long ip;
	unsigned char ipchar[4];
};

char *trim(char *str) {
	char *p = str;
	char *p1;
	if (p) {
		p1 = p + strlen(str) - 1;
		while (*p) {
			p++;
			if (*p == '\0') {
				break;
			}
		}

		while (p1 > p)
			*p1-- = '\0';
	}
	return p;
}

MYSQL *conn_init() {

	MYSQL *connection = NULL;
	char *mysqlserver;
	char *mysqlport;
	char *mysqlusr;
	char *mysqlpwd;
	char *mysqldb;

	parse_config_file("../src/conf/global.conf");

	mysqlserver = (char *) malloc(sizeof(char) * 1024);
	strcpy(mysqlserver, get_config_var("mysqlserver"));

	mysqlport = (char *) malloc(sizeof(char) * 1024);
	strcpy(mysqlport, get_config_var("mysqlport"));

	mysqlusr = (char *) malloc(sizeof(char) * 1024);
	strcpy(mysqlusr, get_config_var("mysqlusr"));

	mysqlpwd = (char *) malloc(sizeof(char) * 1024);
	strcpy(mysqlpwd, get_config_var("mysqlpwd"));

	mysqldb = (char *) malloc(sizeof(char) * 1024);
	strcpy(mysqldb, get_config_var("mysqldb"));

	connection = mysql_init(NULL);

	if (connection == NULL) {
		printf("connection NULL\n");
		return NULL;
	}

	mysql_real_connect(connection, mysqlserver, mysqlusr, mysqlpwd, mysqldb, 0,
			NULL, 0);
	if (connection == NULL) {
		printf("%s\n", mysql_error(connection));
	} else {

	}

	/*set no auto commit*/
	mysql_autocommit(connection, 0);

	return connection;
}

int conn_close(MYSQL *connection) {

	printf("beginning to close DB_CONNECTION.....\n");
	mysql_close(connection);
	connection = NULL;
	printf("it's successfully to close DB_CONNCETION.....!\n");

	return 0;
}

int insert_item(MYSQL *connection, char *tablename, char *content, char *source,
		char *level, int remove) {
	char *sql;
	char *chunk;
	char *ip = "192.168.1.106";
	union ipu iptest;

	sql = (char *) malloc(sizeof(char) * 1024);
	chunk = (char *) malloc(sizeof(char) * 1024);
	ip = (char *) malloc(sizeof(char) * 1024);
//	buf = (char *) malloc(sizeof(char) * (65535));

	iptest.ip = getlocalhostip();
	sprintf(ip, "%3u.%3u.%3u.%3u", iptest.ipchar[0], iptest.ipchar[1],
			iptest.ipchar[2], iptest.ipchar[3]);

//	mysql_real_escape_string(connection, chunk, source, strlen(source) * 2 + 1);
	mysql_real_escape_string(connection, chunk, source, strlen(source));

	sprintf(sql,
			"insert into %s (`ip`, `content`, `source`, `level`, `remove`, `date`) VALUES('%s','%s','%s',%d,%d,now())",
			tablename, ip, content, chunk, atoi(level), remove);

	mysql_real_query(connection, sql, (unsigned int) (strlen(sql)));

	if (mysql_errno(connection)) {
		fprintf(stderr, "Retrive error: %s\n", mysql_error(connection));
	}

	free(ip);
	free(chunk);
	free(sql);

	return 0;
}
