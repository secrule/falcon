/*
 * nw_mgr.h
 *
 *  Created on: 2012-5-10
 *      Author: root
 */

#ifndef NW_MGR_H_
#define NW_MGR_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iconv.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

long getlocalhostip();

#endif /* NW_MGR_H_ */
