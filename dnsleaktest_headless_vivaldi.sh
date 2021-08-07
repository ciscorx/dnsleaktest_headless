#!/bin/sh
#####################################################################
##            dnsleaktest_headless.sh
#####################################################################
#  Automates the process of going on a browser to www.dnsleaktest.com
#    and finding out if your vpn is actually working.  Useful for
#    headless servers that arent running a display manager.

#  Tested on Raspberry Pi Buster with 64 bit Vivaldi 2.0.1309.37
#        and Odroid N2+ on Ubuntu 20.10 on Xorg with Vivaldi 2.0.1309.37
#  Requirements: linux, xvfb, xautomation, scrot 0.8-18+,
#    vivaldi-stable 

#  If the script doesnt work because it fails recognize the button on
#    the web page then remove the web page button recognition
#    condition and replace it with true.  Chances are it will work

#  It doesnt seem work properly when run as root.  Instead issue su
#    user -c "./dnsleaktest_headless.sh" if you are root.

#  Authors/maintainers: ciscorx@gmail.com
#  License: GNU GPL v3

#DSP=:99
SP1=500000
SP2=2000000
BROWSER="vivaldi-stable --incognito --disk-cache-dir=/tmp" 
TMPDIR=/tmp/dnsleaktest_tmp
STEP_TEMP_DIR=/tmp/${0#./}
SAVEFILE=/tmp/dnsleaktest_results.txt
OUTPUT_SCREENSHOTS=1

rnd() {
python -S -c "import random; print( random.randrange($1,$2))"
}

rnd_offset() {
python -S -c "import random; print(random.randrange($1,$(($1 + $2))))"
}

is_display_free() {
    xdpyinfo -display $1 >/dev/null 2>&1 && echo "in use" || echo "free"
}

find_free_display() {
    DSP_NUM=99
    while [ $(is_display_free ":$DSP_NUM") = "in use" ]; do
	DSP_NUM=$(( $DSP_NUM - 1 ))
    done
    echo ":$DSP_NUM"
}
DSP=$(find_free_display)

       
get_md5 () {
    rm -f /tmp/targ.ppm
    DISPLAY=$DSP scrot -a $1,$2,$3,$4 /tmp/targ.ppm
    tmpmd5=`md5sum /tmp/targ.ppm | awk '{print $1}'`

    if [ $OUTPUT_SCREENSHOTS = 1 ]; then
	cp /tmp/targ.ppm $step_temp_dir/$(printf "%.3d" $step)\ -$tmpmd5.ppm
	rm -f /tmp/fullscreen.ppm
	DISPLAY=$DSP scrot /tmp/fullscreen.ppm
	cp /tmp/fullscreen.ppm $STEP_TEMP_DIR/$step\ -\ $1\ $2\ $3\ $4.ppm
	./draw_a_rectangle_in_a_ppm_file.o $STEP_TEMP_DIR/$step\ -\ $1\ $2\ $3\ $4.ppm $1 $2 $3 $4
	./draw_a_circle_in_a_ppm_file.o $STEP_TEMP_DIR/$step\ -\ $1\ $2\ $3\ $4.ppm $(($1+$3/2)) $(($2+$4/2)) $3
	convert $STEP_TEMP_DIR/$step\ -\ $1\ $2\ $3\ $4.ppm  $step_temp_dir/$(printf "%.3d" $step)\ -\ $1\ $2\ $3\ $4.png
	rm $STEP_TEMP_DIR/$step\ -\ $1\ $2\ $3\ $4.ppm
    fi
    echo $tmpmd5
}

sleep_until_screen_stops_changing ()
{

    rm -f /tmp/screen_stops_changing.ppm
    DISPLAY=$DSP scrot /tmp/screen_stops_changing.ppm
    lastmd5=`md5sum /tmp/screen_stops_changing.ppm | awk '{print $1}'`
 #   echo $md5
    tmpmd5="tmpmd5"
    sleep 2
    
    until [ $tmpmd5 = $lastmd5 ]; do 
	rm -f /tmp/screen_stops_changing.ppm
	DISPLAY=$DSP scrot /tmp/screen_stops_changing.ppm
	lastmd5=$tmpmd5
	tmpmd5=`md5sum /tmp/screen_stops_changing.ppm | awk '{print $1}'`
#	echo $tmpmd5
	echo -n .
	sleep 2
    done
    echo .
    rm -f /tmp/screen_stops_changing.ppm
   }
    

step=0
rm -f $SAVEFILE
rm -rf $TMPDIR
mkdir -p $TMPDIR
PROCEXISTS=`ps -ef | grep $TMPDIR | wc | awk '{print $1}'`
if  [ ! $PROCEXISTS = 1 ]; then
    PROCNUM=`ps -ef | grep $TMPDIR | awk 'NR==1{print $2}'`
    kill -9 $PROCNUM
fi

sleep 1
Xvfb $DSP -fbdir $TMPDIR &
DISPLAY=$DSP $BROWSER "www.dnsleaktest.com" &
sleep 4

sleep_until_screen_stops_changing
## set focus on web page
#DISPLAY=$DSP xte 'mousemove 400 400' 'mouseclick 1' "usleep $SP2"

## reset zoom if applied
 DISPLAY=$DSP xte 'keydown Control_L' 'str 0' 'keyup Control_L' 
 sleep 1

DISPLAY=$DSP scrot /tmp/dnsleaktest1.png
#DISPLAY=$DSP xte 'key Tab' "usleep $SP1"  'key Tab' "usleep $SP1"  'key Tab' "usleep $SP1"  'key Tab' "usleep $SP1"  'key Tab' "usleep $SP1"  'key Tab' "usleep $SP1"

#DISPLAY=$DSP scrot /tmp/dnsleaktest1.5.png
#DISPLAY=$DSP xte 'key Return' "usleep $SP1"

rm -f /tmp/t.ppm
DISPLAY=$DSP scrot -a 665,504,110,14 /tmp/t.ppm
md5=`md5sum /tmp/t.ppm | awk '{print $1}'`

#md5=$(get_md5 533 529 105 16)
step=$(($step + 1)) 
# web page button recognition condition
if [ $md5 = '939c9a09784ce587d655562a95b2331b' ] || [ $md5 = '40db483d3c01b24479b7a1bd7f0b3e50' ]; then
    DISPLAY=$DSP xte 'mousemove 700 511' 'mouseclick 1' "usleep $SP1"

    sleep_until_screen_stops_changing

# set focus on web page
DISPLAY=$DSP xte 'mousemove 400 400' 'mouseclick 1' "usleep $SP2"

    DISPLAY=$DSP scrot /tmp/dnsleaktest2.png
    DISPLAY=$DSP xte 'keydown Control_L' 'str a' "usleep $SP2" 'str c' 'keyup Control_L'
    sleep 2
    
    DISPLAY=$DSP scrot /tmp/dnsleaktest2.5.png
    DISPLAY=$DSP gedit&
    sleep 14
    
    DISPLAY=$DSP scrot /tmp/dnsleaktest2.6.png
    DISPLAY=$DSP xte 'keydown Control_L' 'str v' 'keyup Control_L' "usleep $SP2"

    DISPLAY=$DSP scrot /tmp/dnsleaktest2.7.png
    DISPLAY=$DSP xte 'keydown Control_L' 'str s' 'keyup Control_L' "usleep $SP2" "str $SAVEFILE" 'key Return'
    sleep 1



    
# cat /tmp/savethistmpfile.txt | grep @ >> $SCRIPTDIR/unfollowed.txt
    DISPLAY=$DSP scrot /tmp/dnsleaktest3.png

    sleep 1
    DISPLAY=$DSP xte 'keydown Control_L' 'str q' 'keyup Control_L'
    sleep 1
    DISPLAY=$DSP scrot /tmp/dnsleaktest4.png
else
    echo Error: unable to recognize button on web page in order to click it.
fi


PROCEXISTS=`ps -ef | grep $TMPDIR | wc | awk '{print $1}'`
if  [ ! $PROCEXISTS = 1 ]; then
    PROCNUM=`ps -ef | grep $TMPDIR | awk 'NR==1{print $2}'`
    kill -9 $PROCNUM
fi

cat $SAVEFILE | sed 30q | sed "/What do the results of this test mean/q"


