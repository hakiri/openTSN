版本说明：
FPGA_OS_1in_1out：此FPGA_OS的版本与UM的接口为1组输入1组输出的版本，其中CPU与端口的输入分组数据先经过MUX进行汇聚后输入给UM，输出时输出分组通过DMUX模块分配给CPU或不同端口。

FPGA_OS_1in_4out：此FPGA_OS的版本与UM的接口为1组输入4组输出的版本，无CPU的通路，端口的输入分组数据先经过MUX进行汇聚后输入给UM，输出时输出分组由UM控制输出给不同端口。

FPGA_OS_5in_5out：此FPGA_OS的版本与UM的接口为5组输入5组输出的版本，其中5组接口分别为1组CPU2UM/UM2CPU，4组PORTx2UM/UM2PORTx（x为0-3）。

FPGA_OS_1in_1out/FPGA_OS_1in_4out/FPGA_OS_5in_5out的文件夹包括2个文件，分别为：FPGA_OS.edf、FPGA_OS.v。FPGA_OS.edf为综合后的edf网表文件，FPGA_OS.v为接口声明的文件。

开发环境：
三个FPGA OS的网表文件均在vivado 2018.3版本上生成。