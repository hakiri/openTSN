#!/bin/bash
#upload.sh
#password = 970904
#user = joejiang
#ip = 192.168.1.110

cd /mnt/sshpass-1.06
#./sshpass -p 970904 scp /mnt/latency_out joejiang@192.168.1.30:/home/joejiang/Desktop/ant-data
#./sshpass -p 970904 scp /mnt/statistic_out joejiang@192.168.1.30:/home/joejiang/Desktop/ant-data

./sshpass -p 970904 scp /mnt/latency_out joejiang@192.168.1.30:/home/joejiang/Desktop/ANT_test/
./sshpass -p 970904 scp /mnt/statistic_out joejiang@192.168.1.30:/home/joejiang/Desktop/ANT_test/
