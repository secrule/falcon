#!/bin/sh

if [ -f /usr/include/sys/inotify.h ]
then
	echo "Found inotify success!"
else
	echo "inotify not found!Plz update your linux kernel to 2.6.13 or later"
	exit 1
fi

if [ -d /usr/local/include/inotifytools ]
then
	echo "Found inotifytools success!"
else
	echo "inotifytools not found!Plz install inotifytools first"
	exit 1
fi

if [ -d /usr/include/mysql ] || [ -d /usr/lib/mysql ]
then
	echo "Found mysql-dev environment success!"
else
	echo "Plz install mysql-dev enviroment.use 'yum install mysql-devel or apt-get install libmysqlclient15-dev'"
	exit 1
fi

