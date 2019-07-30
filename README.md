	一、OpenTSN简介
　　OpenTSN是基于FAST架构设计的TSN集成验证环境，主要设计目标包括：	
		（1）基于FPGA实现支持802.1AS 、802.1Qbv，802.1Qch，802.1Qci等TSN核心功能的交换机原型；
　　（2）基于FPGA实现支持802.1AS 、802.1Qbv，802.1Qci等功能的TSN接口适配器，同时为时间敏感应用提供编程接口；
　　（3）基于上述FPGA交换机和适配器原型，搭建TSN实验网络；
　　（4）实现TSN网络CNC控制器原型，支持对TSN网络的离线规划和配置；
　　（5）实现支持802.1AS 、802.1Qci和802.1Qbv的TSN测试仪原型，支持TSN数据流、Best Effort背景流的发送、接收和性能统计；
　　（6）实现TSN网络远程遥测功能，可微观观测TSN网络时间同步状态，交换机内部队列状态等，为TSN核心实现机制评估提供关键数据；
　　（7）实现TSN网络与IP网络的网关功能；
　　OpenTSN环境是包含6个TSN交换机的环形拓扑，以及连接在拓扑上的TSN测试仪，TSN CNC，TSN-Insigt，网络摄像头和TSN网关组成。

二、OpenTSN目录结构说明如下。
　　1. bin目录
　　该目录主要用于存放编译软硬件代码中生成的文件，目前OpenTSN环境用到的文件有tsn_CNC，tsn_switch和tsn_insight，用户可以参考OpenTSN项目简介文章中的拓扑图进行了解。
　　tsn_CNC：包含topology_info拓扑文件和可执行文件tsn_CNC，用于配置交换节点，该可执行文件需要运行在linux设备中，可以通过修改拓扑文件中的sw_mac和host_mac的值即可对交换机的mac和主机的mac进行配置。
　　tsn_switch：包含硬件代码生成的固化文件BOOT.bin，用户只需要把该文件替换掉原来openbox中的BOOT.bin文件，即可作为TSN交换节点使用。
　　tsn_insight：存放的是可执行文件tsn_insight，该文件需要运行在安装Qt环境中的linux设备中，用于界面的显示。
　　2. doc目录
　　该目录主要用于存放文档，包含操作手册，设计文档和配置文件。
　　设计文档：包含软硬件的设计文档，软件包括tsc_CNC和tsn_insight的设计文档，硬件包含了FTS硬件详细设计文档。
　　操作手册：包含OpenTSN操作手册，介绍了该开源代码的架构，便于用户快速了解OpenTSN的结构层次。
　　配置文档：包含OpenTSN配置文档，介绍如何对环境进行搭建，介绍如何对设备进行配置等，能够帮助用户搭建OpenTSN环境。
　　3. src目录
　　src目录用户存放用于存放软件和硬件的源码。
　　软件代码：包含tsn_CNC和tsn_insight的源代码，tsn_CNC源代码是使用c语言进行实现，需要在linux设备中进行编译，生成可执行文件tsn_CNC。tsn_insight是使用C++实现的，需要在安装Qt的linux设备中，进行编译生成可执行文件tsn_insight。软件代码中生成的可执行文件都与拷贝到bin目录下。
　　硬件代码：硬件代码中有tsn交换节点的um逻辑代码和bit流文件，um代码是tsn交换节点的硬件核心部分，与FPGA-OS组成了整个FPGA硬件逻辑。bit流是根据整个FPGA硬件逻辑编译生成的二进制文件，可以通过该bit流生成tsn交换节点使用的BOOT.bin。
　　4. tool目录
　　包含OpenTSN环境使用到的工具，包括ANT测试仪和流量分析器。
　　ANT测试仪：运行平台是openbox，该工具主要用于发包和网络延迟，即可发送TSN流（VLAN头的优先级为6/7），又能发送带宽预约流（VLAN头的优先级3,4,5），还可以发送尽力转发流。需要进行回环连接用于测试流在网络中的延迟。用户只需把该目录下的BOOT.bin替换掉原来openbox中的BOOT.bin，运行可执行文件ant就可以进行流的测试。GUI是测试仪的界面，可以动态折现显示出流延迟的具体情况，需要运行在安装Qt软件的linux设备中。
　　流量分析器：运行平台是openbox，该工具的功能是接收报文并对报文进行分析，然后把分析的结果通过socket发送给tsn_insight。通过不同的端口进行区分接收的报文，例如beacon报文，接收后计算出时间同步的精度，然后发送给tsn_insight。用户只需把该目录下的BOOT.bin替换掉原来openbox中的BOOT.bin，然后运行tsn_NA即可。
　　5. sys目录
　　该目下主要存放与系统相关的文件，包括fast系统和openbox设备中使用到的TF卡中的内容。
　　fast：包含openbox设备平台的开发文件。主要包含fast_lib和fast_os。
　　FPGA OS 实现的是平台相关的FPGA的处理逻辑，为用户逻辑提出基础的服务,服务包括：1）分组接收与发送；2）高性能DMA的支持；3）分组的输出控制；4）管理控制通控制。
　　FAST_LIB实现FAST UA 与FAST流水线模块之间分组的虚拟交换。向FAST UA 屏蔽硬件平台的差异，并提供编程的API。功能包括：1）FAST分组与SK_Buf的转换；2）内核与用户空间的通信；3）FPGA OS配置管理；4）FAST流水线配置管理等。
　　TF卡：标准openbox运行需要用的文件，用户只需要替换BOOT.bin文件然后重启即可完成硬件逻辑的更换。

