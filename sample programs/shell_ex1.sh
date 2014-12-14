#!/bin/sh
for i in * ; do 
	if [ -x $i ] ; then
		echo $i is executable
	else
		echo $i is NOT executable
	fi
done
