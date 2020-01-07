/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: esw.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)
// 2)
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/15
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////

module esw(
input     wire    clk,
input     wire    rst_n,

//////////pkt from LCM ////////////////
input     wire    [133:0]in_esw_data,
input     wire    in_esw_data_wr,
input     wire    in_esw_valid,
input     wire    in_esw_valid_wr,
  
/////////pkt and out_esw_tsn_md to IBM ///////  
output    wire    [133:0]out_esw_data_0,
output    wire    out_esw_data_wr_0,
output    wire    out_esw_valid_0,
output    wire    out_esw_valid_wr_0,
output    wire    [23:0]out_esw_tsn_md_0,
output    wire    out_esw_tsn_md_wr_0,
input     wire    [4:0]bufm_ID_count_0,

output    wire    [133:0]out_esw_data_1,
output    wire    out_esw_data_wr_1,
output    wire    out_esw_valid_1,
output    wire    out_esw_valid_wr_1,
output    wire    [23:0]out_esw_tsn_md_1,
output    wire    out_esw_tsn_md_wr_1,
input     wire    [4:0]bufm_ID_count_1,

output    wire    [133:0]out_esw_data_2,
output    wire    out_esw_data_wr_2,
output    wire    out_esw_valid_2,
output    wire    out_esw_valid_wr_2,
output    wire    [23:0]out_esw_tsn_md_2,
output    wire    out_esw_tsn_md_wr_2,
input     wire    [4:0]bufm_ID_count_2,

output    wire    [133:0]out_esw_data_3,
output    wire    out_esw_data_wr_3,
output    wire    out_esw_valid_3,
output    wire    out_esw_valid_wr_3,
output    wire    [23:0]out_esw_tsn_md_3,
output    wire    out_esw_tsn_md_wr_3,
input     wire    [4:0]bufm_ID_count_3,

//////reg from or to LCM ////////////
input     wire    in_reg_tap,
input     wire    [47:0]in_MAC_0,
input     wire    [47:0]in_MAC_1,
input     wire    [47:0]in_MAC_2,
input     wire    [47:0]in_MAC_3,
input     wire    [47:0]in_MAC_4,
input     wire    [47:0]in_MAC_5,
input     wire    [47:0]in_MAC_6,
input     wire    [47:0]in_MAC_7,
input     wire    [47:0]in_MAC_8,
input     wire    [47:0]in_MAC_9,
input     wire    [47:0]in_MAC_10,
input     wire    [47:0]in_MAC_11,

input     wire    [15:0]in_port_0,
input     wire    [15:0]in_port_1,
input     wire    [15:0]in_port_2,
input     wire    [15:0]in_port_3,
input     wire    [15:0]in_port_4,
input     wire    [15:0]in_port_5,
input     wire    [15:0]in_port_6,
input     wire    [15:0]in_port_7,
input     wire    [15:0]in_port_8,
input     wire    [15:0]in_port_9,
input     wire    [15:0]in_port_10,
input     wire    [15:0]in_port_11,

output    wire    [63:0]esw_pktin_cnt,
output    wire    [63:0]esw_pktout_cnt_0,
output    wire    [63:0]esw_pktout_cnt_1,
output    wire    [63:0]esw_pktout_cnt_2,
output    wire    [63:0]esw_pktout_cnt_3,

output    wire    [7:0]bufm_ID_cnt_0,
output    wire    [7:0]bufm_ID_cnt_1,
output    wire    [7:0]bufm_ID_cnt_2,
output    wire    [7:0]bufm_ID_cnt_3
);

wire       [133:0]pke2pfw_data;
wire       pke2pfw_data_wr;
wire       pke2pfw_valid;
wire       pke2pfw_valid_wr;
wire       [2:0]pke2pfw_pkttype;
wire       [53:0]pke2pfw_key;

wire       [133:0]pfw2pac_data;
wire       pfw2pac_data_wr;
wire       pfw2pac_valid;
wire       pfw2pac_valid_wr;
wire       [10:0]pfw2pac_action;
wire       pfw2pac_action_wr;

pke pke(
.clk(clk),
.rst_n(rst_n),

//////////pkt from LCM ////////////////
.in_pke_data(in_esw_data),
.in_pke_data_wr(in_esw_data_wr),
.in_pke_valid(in_esw_valid),
.in_pke_valid_wr(in_esw_valid_wr),
  
/////////pkt and action to PAC ///////  
.out_pke_data(pke2pfw_data),
.out_pke_data_wr(pke2pfw_data_wr),
.out_pke_valid(pke2pfw_valid),
.out_pke_valid_wr(pke2pfw_valid_wr),
.out_pke_pkttype(pke2pfw_pkttype),
.out_pke_key(pke2pfw_key),

/////////reg  to LCM /////////////////
.esw_pktin_cnt(esw_pktin_cnt)
);

pfw pfw(
.clk(clk),
.rst_n(rst_n),

//////////pkt and pkttype and key forom pke///
.in_pfw_data(pke2pfw_data),
.in_pfw_data_wr(pke2pfw_data_wr),
.in_pfw_valid(pke2pfw_valid),
.in_pfw_valid_wr(pke2pfw_valid_wr),
.in_pfw_pkttype(pke2pfw_pkttype),
.in_pfw_key(pke2pfw_key),

.in_reg_tap(in_reg_tap),
/////////pkt and action to pac ///////////////
.out_pfw_data(pfw2pac_data),
.out_pfw_data_wr(pfw2pac_data_wr),
.out_pfw_valid(pfw2pac_valid),
.out_pfw_valid_wr(pfw2pac_valid_wr),
.out_pfw_action(pfw2pac_action),
.out_pfw_action_wr(pfw2pac_action_wr),

/////////reg from lcm ////////////////////////
.MAC_0(in_MAC_0),
.MAC_1(in_MAC_1),
.MAC_2(in_MAC_2),
.MAC_3(in_MAC_3),
.MAC_4(in_MAC_4),
.MAC_5(in_MAC_5),
.MAC_6(in_MAC_6),
.MAC_7(in_MAC_7),
.MAC_8(in_MAC_8),
.MAC_9(in_MAC_9),
.MAC_10(in_MAC_10),
.MAC_11(in_MAC_11),

.port_0(in_port_0),
.port_1(in_port_1),
.port_2(in_port_2),
.port_3(in_port_3),
.port_4(in_port_4),
.port_5(in_port_5),
.port_6(in_port_6),
.port_7(in_port_7),
.port_8(in_port_8),
.port_9(in_port_9),
.port_10(in_port_10),
.port_11(in_port_11)
);

pac  pac(
.clk(clk),
.rst_n(rst_n),

//////////pkt and action from pfw /////
.in_pac_data(pfw2pac_data),
.in_pac_data_wr(pfw2pac_data_wr),
.in_pac_valid(pfw2pac_valid),
.in_pac_valid_wr(pfw2pac_valid_wr),
.in_pac_action(pfw2pac_action),
.in_pac_action_wr(pfw2pac_action_wr),

/////////pkt and out_esw_tsn_md to ibm0///////  
.out_pac_data_0(out_esw_data_0),
.out_pac_data_wr_0(out_esw_data_wr_0),
.out_pac_valid_0(out_esw_valid_0),
.out_pac_valid_wr_0(out_esw_valid_wr_0),
.out_pac_tsn_md_0(out_esw_tsn_md_0),
.out_pac_tsn_md_wr_0(out_esw_tsn_md_wr_0),
.bufm_ID_count_0(bufm_ID_count_0),

/////////pkt and out_esw_tsn_md to ibm1///////  
.out_pac_data_1(out_esw_data_1),
.out_pac_data_wr_1(out_esw_data_wr_1),
.out_pac_valid_1(out_esw_valid_1),
.out_pac_valid_wr_1(out_esw_valid_wr_1),
.out_pac_tsn_md_1(out_esw_tsn_md_1),
.out_pac_tsn_md_wr_1(out_esw_tsn_md_wr_1),
.bufm_ID_count_1(bufm_ID_count_1),

/////////pkt and out_esw_tsn_md to ibm2///////  
.out_pac_data_2(out_esw_data_2),
.out_pac_data_wr_2(out_esw_data_wr_2),
.out_pac_valid_2(out_esw_valid_2),
.out_pac_valid_wr_2(out_esw_valid_wr_2),
.out_pac_tsn_md_2(out_esw_tsn_md_2),
.out_pac_tsn_md_wr_2(out_esw_tsn_md_wr_2),
.bufm_ID_count_2(bufm_ID_count_2),

/////////pkt and out_esw_tsn_md to ibm3///////  
.out_pac_data_3(out_esw_data_3),
.out_pac_data_wr_3(out_esw_data_wr_3),
.out_pac_valid_3(out_esw_valid_3),
.out_pac_valid_wr_3(out_esw_valid_wr_3),
.out_pac_tsn_md_3(out_esw_tsn_md_3),
.out_pac_tsn_md_wr_3(out_esw_tsn_md_wr_3),
.bufm_ID_count_3(bufm_ID_count_3),

//////reg from or to lcm /////////////
.reg_tap(in_reg_tap),
.esw_pktout_cnt_0(esw_pktout_cnt_0),
.esw_pktout_cnt_1(esw_pktout_cnt_1),
.esw_pktout_cnt_2(esw_pktout_cnt_2),
.esw_pktout_cnt_3(esw_pktout_cnt_3),
.bufm_ID_cnt_0(bufm_ID_cnt_0),
.bufm_ID_cnt_1(bufm_ID_cnt_1),
.bufm_ID_cnt_2(bufm_ID_cnt_2),
.bufm_ID_cnt_3(bufm_ID_cnt_3)
);
endmodule