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
  export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib:/home/nodes/nodes/build:/home/nodes/nodes-lib:$LD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
fi


$LIBS ./nodes-web/build/nodes-web \
  --logLevel=trace \
  > nodes-web.log 2>&1 &
