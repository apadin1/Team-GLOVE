#!/bin/sh
die() { echo >&2 -e "\nERROR: $@\n"; exit 1; }
run() { "$@"; code=$?; [ $code -ne 0 ] && die "command [$*] failed with error code $code"; }
# This is to flash a the new hex file on our dev board

pre="[fla.sh]"

# static config options (defaults should work if running from prog root)
mountdir="JLINK"
build_dir="BUILD"
target="NRF51_DK"
toolchain="GCC_ARM"
prog_dir=${PWD##*/}
program="$prog_dir".hex

OPTIND=1 # A POSIX variable
opt_v= # unset is falsy for [ $opt_v ]
opt_u=
dev_file=sdb
while getopts "h?vud:" opt; do
	case "$opt" in
		h|\?)
			show_help
			exit 0
			;;
		v)  opt_v=0
			;;
		u)  opt_u=0
			;;
		d)  dev_file=$OPTARG
			;;
	esac
done

shift $((OPTIND-1))
[ "$1" = "--" ] && shift
if [ $opt_v ]; then set -x; fi
# End of getopts

# mount point for the device
if [ ! -d $mountdir ]; then
	mkdir $mountdir
fi

# try to determine a device file

# mount in the board if needbe
if ! mount | grep $mountdir > /dev/null; then
	run sudo mount -t vfat -o uid=$USER,gid=users /dev/"$dev" $mountdir
	echo "$pre Mounted /dev/$dev to $mountdir"
fi

# copy over the hex file
run cp "$build_dir/$target/$toolchain/$program" $mountdir
echo "$pre Copied $build_dir/$target/$toolchain/$program to device"

# unmount the board (if requested)
if [ $opt_u ]; then
    if mount | grep $mountdir > /dev/null; then
        sync
        run sudo umount $mountdir
        echo "$pre Un-mounted $mountdir"
	fi
fi

