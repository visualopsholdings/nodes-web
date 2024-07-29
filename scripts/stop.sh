#!/bin/bash
# 
# Stop nodes http daemon

LIST=`ps -ef | grep "nodes-web" | grep -v grep | awk '{print $2}'`
if [ ! -z "$LIST" ]; then
	sudo kill -9 $LIST
fi

exit 0
