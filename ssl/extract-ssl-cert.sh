# extract an SSL cert downloaded from the upstream

if [ "$#" -ne 3 ]; then
	echo "usage: $0 NAME LIVE.TGZ ARCHIVE.TGZ"
	exit 1
fi

NAME=$1
LIVE=$2
ARCHIVE=$3

date "+%H:%M:%S"
echo $NAME
echo $LIVE
echo $ARCHIVE

cd working
rm -rf etc
tar xzf $LIVE
tar xzf $ARCHIVE
sudo rm -rf /etc/letsencrypt/archive/$NAME
sudo rm -rf /etc/letsencrypt/live/$NAME
sudo mv etc/letsencrypt/archive/$NAME /etc/letsencrypt/archive
sudo mv etc/letsencrypt/live/$NAME /etc/letsencrypt/live
sudo chown -R root:root /etc/letsencrypt/archive/$NAME
sudo chown -R root:root /etc/letsencrypt/live/$NAME
rm -rf etc

echo "Certificates successfully extracted"
