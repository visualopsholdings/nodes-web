#!/bin/bash
# 
# Start zchttp

if [ ! -f "zchttp/build/zchttp" ];
then
  echo "Need to build project first"
  exit 1
fi

./zchttp/build/zchttp \
  --logLevel=trace \
  > zchttp.log 2>&1 &
