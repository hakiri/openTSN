#!/bin/sh
deviceID=`cat /mnt/openbox/config/deviceID`
echo "deviceID:"${deviceID}
cd /mnt/openbox/bin
./mem_rw 43c80000 1000 0 $deviceID
