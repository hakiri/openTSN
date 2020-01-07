/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor:FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: data_ctrl.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)receive pkt from ibm
// 2)transmit pkt to ebm
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/15
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

`timescale 1ns / 1ps

module data_cache#(
    parameter    PLATFORM = "xilinx"
)(
    input  wire  clk,
    input  wire  rst_n,
	
//pkt from gpp
    input  wire  in_data_cache_data_wr,
    input  wire  [133:0] in_data_cache_data,
    input  wire  in_data_cache_valid_wr,
    input  wire  in_data_cache_valid,
	output wire  [7:0]out_data_cache_ID,
	output wire  [4:0]out_data_cache_ID_count,

//transport to gda module     
    output wire  out_data_cache_data_wr,
    output wire  [133:0] out_data_cache_data,
    output wire  out_data_cache_valid_wr,
    output wire  out_data_cache_valid,
	input  wire  [7:0]in_data_cache_ID,
	input  wire  in_data_cache_ID_wr
);
wire         [10:0]addr2data_waddr;
wire         addr2data_waddr_wr;
wire         [10:0]addr2data_raddr;
wire         addr2data_raddr_wr;

addr_mgmt  addr_mgmt(            
. clk(clk),
. rst_n(rst_n),

. in_addr_mgmt_data(in_data_cache_data),
. in_addr_mgmt_data_wr(in_data_cache_data_wr),
. in_addr_mgmt_valid(in_data_cache_valid),
. in_addr_mgmt_valid_wr(in_data_cache_valid_wr),
. out_addr_mgmt_ID(out_data_cache_ID), 
. out_addr_mgmt_ID_count(out_data_cache_ID_count),   
. addr2data_waddr(addr2data_waddr), 
. addr2data_waddr_wr(addr2data_waddr_wr),

. addr2data_raddr(addr2data_raddr),
. addr2data_raddr_wr(addr2data_raddr_wr),
. in_addr_mgmt_ID(in_data_cache_ID),
. in_addr_mgmt_ID_wr(in_data_cache_ID_wr),
. in_ram2addr_valid(out_data_cache_valid)
);   

data_ctrl  data_ctrl(
. clk(clk), 
. rst_n(rst_n),
. in_data_ctrl_data(in_data_cache_data),
. in_data_ctrl_data_wr(in_data_cache_data_wr),

. addr2data_waddr(addr2data_waddr),
. addr2data_raddr(addr2data_raddr),
. addr2data_waddr_wr(addr2data_waddr_wr),
. addr2data_raddr_wr(addr2data_raddr_wr),

. out_data_cache_valid(out_data_cache_valid),
. out_data_cache_valid_wr(out_data_cache_valid_wr),
. out_data_cache_data(out_data_cache_data),
. out_data_cache_data_wr(out_data_cache_data_wr)
);
endmodule
/**************************
data_cache data_cache_inst(
.clk(),
.rst_n(),

.in_data_cache_data_wr(),
.in_data_cache_data(),
.in_data_cache_valid_wr(),
.in_data_cache_valid(),
.out_data_cache_ID(),
.out_data_cache_ID_count(),
   
.out_data_cache_data_wr(),
.out_data_cache_data(),
.out_data_cache_valid_wr(),
.out_data_cache_valid(),
.in_data_cache_ID(),
.in_data_cache_ID_wr()
);
*************************/