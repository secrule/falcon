/*
 * got_db_conn_mgr.h
 *
 *  Created on: 2012-5-9
 *      Author: root
 */

#ifndef DB_MGR_H_
#define DB_MGR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "nw_mgr.h"
#include "config_file.h"

extern MYSQL *conn_init();
extern int conn_close(MYSQL *connection);
extern int insert_item(MYSQL *connection, char *tablename, char *content, char *source,
		char *level, int remove);

#endif /* GOT_DB_CONN_MGR_H_ */
