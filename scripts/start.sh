#!/bin/bash
# 
# Start nodes-web

if [ ! -f "nodes-web/build/nodes-web" ];
then
  echo "Need to build project first"
  exit 1
fi

./nodes-web/build/nodes-web \
  --logLevel=trace \
  > nodes-web.log 2>&1 &
