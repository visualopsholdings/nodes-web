
descendent_pids() {
    pids=$(pgrep -P $1)
    echo $pids
    for pid in $pids; do
        descendent_pids $pid
    done
}

cat $1
descendent_pids `cat $1`
