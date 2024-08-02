#!/bin/bash
# 
# Start nodes-web

if [ ! -f "nodes-web/build/nodes-web" ];
then
  echo "Need to build or install project first"
  exit 1
fi

if [ -d /home/nodes/nodes-lib ];
then
  LIBS="LD_LIBRARY_PATH=/usr/local/lib;/home/nodes/nodes/build;/home/nodes/nodes-lib"
fi

$LIBS ./nodes-web/build/nodes-web \
  --logLevel=trace \
  > nodes-web.log 2>&1 &
