# openTSN

#### 介绍
开源的TSN环境，在硬件中实现1588协议进行时间同步，实现高优先级流量的精确延时。

#### 文件说明

文件夹介绍
整体分为四个文件夹，APP，document，hw_code和sw_code
1. APP：包含BOOT文件夹、CNC文件夹和可执行文件tsn_insight，BOOT文件夹中有用到的BOOT.bin，CNC文件夹包含CNC设备的可执行文件和配置文件
2. document：包含软件文档和硬件文档，其中软件文档又分为TSN_insight软件开发、软件系统设计文档
3. hw_code：包含硬件中um的代码
4. sw_code：包括tsn_controller和tsn_insight代码，tsn_countroller需要在交叉编译中进行编译，tsn_insight需要在Qt环境中运行，目前在APP文件夹下有生成的可执行文件

#### 环境配置

1. 使用openbox设备连接成一个环形，设备0号端口与另一台设备1号端口相连接，依次类推，形成一个环。
2. 使用一台openbox当作CNC使用，该设备的0号端口与环中任意一台设备的2号端口相连接。
3. 配置环形设备的交换节点，需要把原来openbox中BOOT.bin替换成openTSN\APP\BOOT\tsn_switch_boot_bin目录下的BOOT.bin文件，并修改设备中/mnt/openbox/config目录下的deviceID文件，修改为0代表为主时钟，同时mac地址为00:06:06:00:00:00，修改为其他代表为从时钟，并且代表mac地址的最后一位。
4. 配置作为CNC的openbox设备，把原来openbox中BOOT.bin替换成openTSN\APP\BOOT\tsn_controller_boot_bin目录下的BOOT.bin文件。
5. 把openTSN\APP\CNC\目录下的所有文件放到作为CNC的openbox设备的/mnt/目录下，并且修改topology_info文件，其中sw_mac代表在第二步修改的交换机的mac地址，host_mac代表需要配置的与2号端口相连的主机的mac地址。
6. 把openTSN\APP\目录下的tsn_insight拷贝到linux主机或者虚拟机中，并且需要安装Qt的环境，该主机的网口与CNC设备的管理口相连接。
7. 修改CNC中的controller_info文件，把controller_ip改成为linux机器中的ip地址，为了建立socket连接。

#### 使用说明

1. 在linux主机中执行./tsn_insight
2. 在CNC中执行./tsn_countroller
3. linux主机中会出现动态的时间同步的折线，每秒钟刷新一下，可以观察打时间同步的精度
4. 可以通过在一个交换节点的2号端口连接摄像头，另外一个交换节点的2号端口连接一个主机当作显示器用与验证环的连通性

#### 参与贡献

1. FAST开发组