#!/bin/bash 


PATH=/sbin:/usr/sbin:/bin:/usr/bin:/home/c/script



func1() {
	echo "func1 calling........."
}
func2() {
	echo "func2 calling........."
}

reboot() {
	echo "system will reboot......."
	if [ -f /etc/init.d/reboot ]; then 
		/etc/init.d/reboot start 
	fi
}
halt() {
	echo "system will shutdown ......"
	if [ -f /etc/init.d/halt ]; then 
		/etc/init.d/halt start 
	fi
}

catpasswd() {
	if [ -f /etc/passwd ] ;then 
		cat /etc/passwd 
	fi
}

case "$1" in
	func1)
	func1
	;;
	func2)
	func2
	;;
	reboot)
	reboot
	;;
	halt)
	halt
	;;
	catpasswd)
	catpasswd
	;;
	*)
		echo "Usage: $0 func1 | func2" >&2
		exit 3
		;;
esac
