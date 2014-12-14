# Even creepier shell script. Monitors who logs in and out of system
new=/tmp/busybody1.$$
old=/tmp/busybody2.$$
touch $old				#make empty file

while : # while true
do
	who > $new
	diff $old $new
	mv $new $old
	sleep 20
done
