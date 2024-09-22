#!/bin/bash
# 
# request a new certificate

if [ "$#" -lt 2 ]; 
then
	echo "usage: $0 HOSTNAME EMAIL"
	exit 1
fi

HOST=$1
EMAIL=$2

sudo certbot --nginx --non-interactive --agree-tos --domains $HOST --email $EMAIL
