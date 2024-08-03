# deploy a simple nginx conf

if [ "$#" -ne 2 ]; then
	echo "usage: $0 HOSTNAME HPORT"
	exit 1
fi

nginx_conf() {
	export SERVER=$3
	export HPORT=$4
  sudo rm -rf $2
	envsubst '$SERVER,$HPORT' < $1 > $2
}

HOST=$1
HPORT=$2

echo "Generate Conf"
nginx_conf nodes-web/scripts/nginx-nossl.tmpl nginx.conf $HOST $HPORT

echo "Using"
sudo cp /etc/nginx/sites-enabled/default nginx-backup.conf
sudo cp nginx.conf /etc/nginx/sites-enabled/default
sudo cp nginx.conf /etc/nginx/conf.d/default.conf
sudo service nginx restart

exit 0
