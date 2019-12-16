版本说明：
FPGA_OS_1in_1out：此FPGA_OS的版本与UM的接口为1组输入1组输出的版本，其中CPU与端口的输入分组数据先经过MUX进行汇聚后输入给UM，输出时输出分组通过DMUX模块分配给CPU或不同端口。

FPGA_OS_1in_4out：此FPGA_OS的版本与UM的接口为1组输入4组输出的版本，无CPU的通路，端口的输入分组数据先经过MUX进行汇聚后输入给UM，输出时输出分组由UM控制输出给不同端口。

FPGA_OS_5in_5out：此FPGA_OS的版本与UM的接口为5组输入5组输出的版本，其中5组接口分别为1组CPU2UM/UM2CPU，4组PORTx2UM/UM2PORTx（x为0-3）。

FPGA_OS_5in_5out_mac：此FPGA_OS的版本与UM的接口为5组输入5组输出的版本，其中5组接口分别为1组CPU2UM/UM2CPU，4组PORTx2UM/UM2PORTx（x为0-3）,新增设备MAC地址配置功能。

FPGA_OS_5in_5out_tsnnic：此FPGA_OS版本为TSN_NIC接口定制版本 , UM的接口为5组输入5组输出，其中5组接口分别为1组CPU2UM/UM2CPU，4组PORTx2UM/UM2PORTx（x为0-3，其中1号口输出报文第4拍中包含时间戳标识。

开发环境：
五个FPGA OS的网表文件均在vivado 2018.3版本上生成。