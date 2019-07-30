#!/bin/sh -e
### BEGIN INIT INFO
# HNXS StartUP
### END INIT INFO

PATH="/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/mnt/openbox/bin:/mnt/openbox/sbin"
# eth0 MAC
ifconfig eth0 down
ifconfig eth0 hw ether 00:0A:00:11:18:88
ifconfig eth0 up
#ifconfig eth0 add 1001::eeee:ffff:0016/64

#route
route add default gw 192.168.1.1

#nfs dir
#mount -t nfs 192.168.1.71:/usr/xilinx/arm-xilinx-linux-gnueabi/openbox /mnt/openbox/remote -o proto=tcp -o nolock &

#PL obx
#insmod /mnt/openbox/openbox-s4.ko
ifconfig obx0 192.168.0.1/24 up
ifconfig obx1 192.168.1.1/24 up
ifconfig obx2 192.168.2.1/24 up
ifconfig obx3 192.168.3.1/24 up

zebra -d -u root -g root
ospfd -d -u root -g root
ripd  -d -u root -g root

sh /mnt/openbox/config/deviceID_config.sh

exit 0

