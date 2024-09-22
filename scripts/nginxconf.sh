#!/bin/bash
# 
# deploy a simple nginx conf

if [ "$#" -lt 3 ]; 
then
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

if [ "$#" -eq 3 ]; 
then
  FOLDER="/home/nodes"
else
  FOLDER=$4
fi


echo "Generate SSL Conf"
nginx_conf nodes-web/scripts/nginx.tmpl nginx.conf $HOST $PORT $HPORT $FOLDER

echo "Using SSL certificate"
sudo cp /etc/nginx/sites-enabled/default nginx-backup.conf
sudo cp nginx.conf /etc/nginx/sites-enabled/default
sudo cp nginx.conf /etc/nginx/conf.d/default.conf
sudo service nginx restart

exit 0

