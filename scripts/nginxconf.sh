# deploy a simple nginx conf

if [ "$#" -ne 3 ]; then
	echo "usage: $0 HOSTNAME PORT HPORT"
	exit 1
fi

nginx_conf() {
	export SERVER=$3
	export PORT=$4
	export HPORT=$5
  sudo rm -rf $2
	envsubst '$SERVER,$PORT,$HPORT' < $1 > $2
}

HOST=$1
PORT=$2
HPORT=$3

echo "Generate SSL Conf"
nginx_conf nodes/scripts/nginx.tmpl nginx.conf $HOST $PORT $HPORT

echo "Using SSL certificate"
sudo cp /etc/nginx/sites-enabled/default nginx-backup.conf
sudo cp nginx.conf /etc/nginx/sites-enabled/default
sudo cp nginx.conf /etc/nginx/conf.d/default.conf
sudo service nginx restart

exit 0

