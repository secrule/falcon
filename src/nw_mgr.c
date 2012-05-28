/*
 * nw_mgr.c
 *
 *  Created on: 2012-5-9
 *      Author: root
 */
#include "nw_mgr.h"

union ipu {
	long ip;
	unsigned char ipchar[4];
};

long getlocalhostip() {
	int MAXINTERFACES = 16;
	long ip;
	int fd, intrface;
	struct ifreq buf[MAXINTERFACES];
	struct ifconf ifc;
	ip = -1;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
		ifc.ifc_len = sizeof buf;
		ifc.ifc_buf = (caddr_t) buf;
		if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) {
			intrface = ifc.ifc_len / sizeof(struct ifreq);
			while (intrface-- > 0) {
				if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[intrface]))) {
					ip =
							inet_addr(
									inet_ntoa(
											((struct sockaddr_in*) (&buf[intrface].ifr_addr))->sin_addr)); //types
					break;
				}

			}
		}
		close(fd);
	}
	return ip;
}
