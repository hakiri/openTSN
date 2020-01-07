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
output    wire    [133:0]out_esw_data,
output    wire    out_esw_data_wr,
output    wire    out_esw_valid,
output    wire    out_esw_valid_wr,
output    wire    [23:0]out_esw_tsn_md,
output    wire    out_esw_tsn_md_wr,
input     wire    [4:0]bufm_ID_count,

////////pkt to GOE //////////////////
output    wire    [133:0]out_esw2port_data2,
output    wire    out_esw2port_data_wr2,
output    wire    out_esw2port_valid2,
output    wire    out_esw2port_valid_wr2,

output    wire    [133:0]out_esw2port_data3,
output    wire    out_esw2port_data_wr3,
output    wire    out_esw2port_valid3,
output    wire    out_esw2port_valid_wr3,

//////reg from or to LCM ////////////
input     wire    direction,
input     wire    [47:0]local_mac_addr,
input     wire    [47:0]direct_mac_addr,
output    wire    [63:0]esw_pktin_cnt,
output    wire    [63:0]esw_pktout_cnt,
output    wire    [7:0]bufm_ID_cnt
);

wire       [133:0]pke2pfw_data;
wire       pke2pfw_data_wr;
wire       pke2pfw_valid;
wire       pke2pfw_valid_wr;
wire       [2:0]pke2pfw_pkttype;
wire       [101:0]pke2pfw_key;

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

/////////pkt and action to pac ///////////////
.out_pfw_data(pfw2pac_data),
.out_pfw_data_wr(pfw2pac_data_wr),
.out_pfw_valid(pfw2pac_valid),
.out_pfw_valid_wr(pfw2pac_valid_wr),
.out_pfw_action(pfw2pac_action),
.out_pfw_action_wr(pfw2pac_action_wr),

/////////reg from lcm ////////////////////////
.local_mac_addr(local_mac_addr),
.direct_mac_addr(direct_mac_addr),
.direction(direction)
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

/////////pkt and out_esw_tsn_md to ibm///////  
.out_pac_data(out_esw_data),
.out_pac_data_wr(out_esw_data_wr),
.out_pac_valid(out_esw_valid),
.out_pac_valid_wr(out_esw_valid_wr),
.out_pac_tsn_md(out_esw_tsn_md),
.out_pac_tsn_md_wr(out_esw_tsn_md_wr),
.bufm_ID_count(bufm_ID_count),

////////pkt to goe ///////////////
.out_pac2port_data2(out_esw2port_data2),
.out_pac2port_data_wr2(out_esw2port_data_wr2),
.out_pac2port_valid2(out_esw2port_valid2),
.out_pac2port_valid_wr2(out_esw2port_valid_wr2),

.out_pac2port_data3(out_esw2port_data3),
.out_pac2port_data_wr3(out_esw2port_data_wr3),
.out_pac2port_valid3(out_esw2port_valid3),
.out_pac2port_valid_wr3(out_esw2port_valid_wr3),

//////reg from or to lcm /////////////
.esw_pktout_cnt(esw_pktout_cnt),
.bufm_ID_cnt(bufm_ID_cnt)
);
endmodule