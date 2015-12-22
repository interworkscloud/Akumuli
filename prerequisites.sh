#!/bin/sh

distri=$(awk '/^DISTRIB_ID=|^ID=/' /etc/*-release | sed 's/.*=//' | sed 's/"//g' | tr '[:upper:]' '[:lower:]')

case $distri in
	"rhel") pkgman='yum';;
	"debian") pkgman='apt';;
	#more to come...
esac

if [ "x$pkgman" = "xyum" ]; then
	echo "Building for Centos/Fedora/RHEL"
	sudo yum install cmake
	#sudo yum install boost boost-devel boost-thread
	sudo yum install log4cxx log4cxx-devel
	sudo yum install sqlite sqlite-devel
	sudo yum install apr-util-devel
	sudo yum install libmicrohttpd-devel
	sudo yum install jemalloc
else
	if [ "x$pkgman" = "xapt" ]; then
		echo 'The script will install packages using apt-get.' \
		     'It can ask for your sudo password.'
		     
		echo 'Trying to install boost libraries'
		sudo apt-get install -y libboost-dev libboost-system-dev libboost-thread-dev \
		     libboost-filesystem-dev libboost-test-dev 
		sudo apt-get install -y libboost-coroutine-dev \
		     libboost-context-dev \
		     libboost-program-options-dev libboost-regex-dev
		     
		echo 'Trying to install other libraries'
		sudo apt-get install -y libapr1-dev libaprutil1-dev libaprutil1-dbd-sqlite3 libmicrohttpd-dev
		sudo apt-get install -y liblog4cxx10-dev liblog4cxx10
		sudo apt-get install -y libjemalloc-dev
		sudo apt-get install -y libsqlite3-dev

		echo 'Trying to install cmake'
		sudo apt-get install -y cmake
	else
		echo "ERROR: Unknown package manager"
		exit 1
	fi
fi #package manager check
