#!/bin/sh
# This is to flash a the new hex file on our dev board

pre="[fla.sh]"
usage="./fla.sh [-u]"

# config options (defaults should work if running from prog root)
mountdir="JLINK"
build_dir="BUILD"
target="NRF51_DK"
toolchain="GCC_ARM"
prog_dir=${PWD##*/}
program="$prog_dir".hex

if [ $# != 1 ]; then
	echo "Note: mounts and copies over the thing.  Use ./fla.sh -u to unmount"
fi

if [ ! -d $mountdir ]; then
	mkdir $mountdir
fi

# try and select the device file that is the JLINK
# TODO this could use a better system...
#if [ -d /dev/sdb ]; then
#	dev=sdb
#elif [ -d /dev/sdc ]; then
#	dev=sdc
#elif [ -d /dev/sdd ]; then
#	dev=sdd
#fi
dev=sda # device name override

# mount in the board if needbe
if ! mount | grep $mountdir > /dev/null; then
	sudo mount -t vfat -o uid=$USER,gid=users /dev/"$dev" $mountdir
	echo "$pre Mounted /dev/$dev to $mountdir"
fi

# copy over the hex file
cp "$build_dir/$target/$toolchain/$program" $mountdir

# unmount the board (if requested)
if [ $# == 1 ]; then
	if mount | grep $mountdir > /dev/null; then
		if [ $1 == '-u' ]; then
			sync
			sudo umount $mountdir
			echo "$pre Un-mounted $mountdir"
		fi
	fi
fi

