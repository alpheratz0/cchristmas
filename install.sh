#!/bin/bash

if [ $EUID -ne 0 ] ; then
	echo 'error: root privileges required to install' >&2
	exit 1
fi

sudo cp -r "$(dirname "$(realpath "$BASH_SOURCE")")" /opt
sudo ln -fs /opt/christmas/tree.sh /usr/local/bin/tree.sh
