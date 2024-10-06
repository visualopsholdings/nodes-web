PID_FILE=$1.pid

if [ ! -f $PID_FILE ]; then
	exit 0
fi

for pid in $(cat $PID_FILE; ci/ptree.sh $PID_FILE); 
do 
	kill -9 $pid > /dev/null 2>&1
  ps -ax $pid > /dev/null 2>&1
	while [ $? == "0" ];
	do
	  echo "waiting on $pid"
	  sleep 1
    ps -ax $pid > /dev/null 2>&1
  done  
done
rm $PID_FILE
