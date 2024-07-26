#!/bin/bash
# 
# Stop zchttp http daemon

LIST=`ps -ef | grep "zchttp" | grep -v grep | awk '{print $2}'`
if [ ! -z "$LIST" ]; then
	sudo kill -9 $LIST
fi

exit 0
