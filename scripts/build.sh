#!/bin/bash
# 
# Build the project for http access

if [ ! -d "zchttp" ];
then
  echo "zchttp project missing."
  exit 1
fi

if [ ! -d "zchttp/build" ];
then
  mkdir zchttp/build
fi
pushd zchttp/build
git pull
cmake ..
make -j4
popd

pushd zchttp/angular/login-ui
npm install
./build.sh
popd

pushd zchttp/angular/admin-ui
npm install
./build.sh
popd

pushd zchttp/angular/chat-ui
npm install
./build.sh
popd
