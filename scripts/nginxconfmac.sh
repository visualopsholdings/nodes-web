#!/bin/bash
# 
# deploy a simple nginx conf but on the mac.

if [ "$#" -ne 4 ]; then
	echo "usage: $0 HOSTNAME PORT HPORT FOLDER"
	exit 1
fi

nginx_conf() {
	export SERVER=$3
	export PORT=$4
	export HPORT=$5
	export FOLDER=$6
  sudo rm -rf $2
	envsubst '$SERVER,$PORT,$HPORT,$FOLDER' < $1 > $2
}

HOST=$1
PORT=$2
HPORT=$3
FOLDER=$4

echo "Generate SSL Conf"
nginx_conf nodes-web/scripts/nginx.tmpl nginx.conf $HOST $PORT $HPORT $FOLDER

echo "Using SSL certificate"
sudo cp /opt/homebrew/etc/nginx/servers/default nginx-backup.conf
sudo cp nginx.conf /opt/homebrew/etc/nginx/servers/default
brew services restart nginx

exit 0

