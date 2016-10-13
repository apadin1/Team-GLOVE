#!/bin/sh
die() { echo >&2 "\nERROR: $@\n"; exit 1; }
run() { "$@"; code=$?; [ $code -ne 0 ] && die "command [$*] failed with error code $code"; }

pre="[fla.sh]"
show_help() { echo "$pre A shell script for Team GLOVE by Tim Schumacher";
			echo "Compliments mbed-cli for 'flashing' the NRF51 dev board with compiled mbed hex files";
			echo " By default, mounts the board (can attempt to find it automatically),";
			echo "and copies over the compiled hex file.";
			echo " Usage:\n      fla.sh [-h] [-v] [-n] [-c] [-s] [-u] [-d <dev_file>]";
			echo "      -h Show this help and exit";
			echo "      -v Verbose output for debugging this script";
			echo "      -n Dry-run, won't copy the hex file to the board";
			echo "      -c Compile, will also run the default compile command";
			echo "      -s Screen, drop into a screen on the serial debug port";
			echo "      -u Do Not Unmount, will not unmount the board when finished";
			echo "      -d <sda|sdb|sdc|...> Device file that is the board's JLINK USB filesystem (no /dev/ needed)";
}

# static config options (defaults should work if running from prog root)
mountdir="JLINK"
build_dir="BUILD"
target="NRF51_DK"
toolchain="GCC_ARM"
prog_dir=${PWD##*/}
program="$prog_dir".hex
serial_dev=/dev/ttyACM0

OPTIND=1 # A POSIX variable
opt_v= # unset is falsy for [ $opt_v ]
opt_n=
opt_c=
opt_s=
opt_u=
dev_file=
while getopts "h?vncsud:" opt; do
	case "$opt" in
		h|\?)
			show_help
			exit 0
			;;
		v)  opt_v=0
			;;
		n)  opt_n=0
			;;
		c)  opt_c=0
			;;
		s)  opt_s=0
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

# compile option
if [ $opt_c ]; then
	run mbed compile -t $toolchain -m $target
    echo "$pre Compilation successful"
fi

# mount point for the device
if [ ! -d $mountdir ]; then
	run mkdir $mountdir
    echo "$pre Created board mount point at $mountdir"
fi

# mount in the board if needbe
if ! mount | grep $mountdir > /dev/null; then

	# since we beed to mount the board, use the supplied device file
	# or attempt to automatically determine the device file
	dev=
	if [ $dev_file ]; then
		dev=/dev/"$dev_file"
		if mount | grep $dev /dev/null; then
			echo "$pre $dev is already mounted, aborting"
			exit 1
		fi

	else
		# auto-determine disk
		disks=$(ls /dev/sd?)
		for d in $disks; do
			if [ -b $d ] && ! mount | grep $d > /dev/null; then
				dev=$d
				echo "$pre $d unmounted, attempting to use"
				break
			fi
		done
	fi
	if [ ! $dev ]; then
		echo "$pre No device specified or found!"
		exit 1
	fi

	# actually mount the device to the mount point
	run sudo mount -t vfat -o uid=$USER,gid=users $dev $mountdir
	echo "$pre Mounted $dev to $mountdir"
fi

# copy over the hex file
if [ ! $opt_n ]; then
	run cp "$build_dir/$target/$toolchain/$program" $mountdir
	echo "$pre Copied $build_dir/$target/$toolchain/$program to device"
fi

# unmount the board (if requested)
if [ ! $opt_u ]; then
    if mount | grep $mountdir > /dev/null; then
        sync
        run sudo umount $mountdir
        echo "$pre Un-mounted $mountdir"
	fi
fi

if [ $opt_s ]; then
	echo "$pre Dropping into serial debug session in screen\n         Use Ctrl-A then :quit to exit\n"
	sleep 1
    echo "$pre Connecting....."
	until [ -c $serial_dev ]; do
		sleep 1
	done
	screen $serial_dev
fi

