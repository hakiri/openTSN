/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 Xperis, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: Hunan Xperis Network Technology Co.,Ltd.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: PTP.v
//Version: 2.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)user define module 
// 2)a sdn network demo
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2018/08/24
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

module PTP #(
    parameter    PLATFORM = "Xilinx"
)(
    input clk,
    input rst_n,   
//cpu or port
   input  pktin_data_wr,
   input  [133:0] pktin_data,
   input  pktin_data_valid,
   input  pktin_data_valid_wr,
   output pktin_ready,//pktin_ready = um2port_alf
    
   output ptp2lcm_data_wr,
   output [133:0] ptp2lcm_data,
   output ptp2lcm_data_valid,
   output ptp2lcm_data_valid_wr,
   input ptp2lcm_ready,//ptp2lcm_ready = port2um_alf    
    
    input cfg_cs_n, //low active
	output wire cfg_ack_n, //low active
	input cfg_rw, //0 :write, 1 :read
	input [31:0] cfg_addr,
	input [31:0] cfg_wdata,
	output wire [31:0] cfg_rdata, 
	output wire cyc_init,
    output wire [47:0]temp_cnt,
   output wire [47:0]precision_time,
   output wire [47:0]device_id
 
);

wire mux2gpp_data_wr;
wire [133:0] mux2gpp_data;
wire mux2gpp_data_valid;
wire mux2gpp_data_valid_wr;
wire gpp2mux_data_alf;

wire parser2mux_data_wr;
wire [133:0] parser2mux_data;
wire parser2mux_data_valid;
wire parser2mux_data_valid_wr;
wire mux2parser_data_alf;	

wire inptp_data_wr;
wire [133:0]	inptp_data;
wire inptp_valid_wr;
wire inptp_valid;
wire inptp_ready;

wire outtx_data_wr;
wire [133:0]outtx_data;
wire outtx_valid_wr;
wire outtx_valid;
wire outtx_ready;

wire gac2cfg_ack;
 wire [47:0]device_mac;
wire [1:0]device_role;

//***************************************************
//                  Module Instance
//***************************************************
//likely fifo/ram/async block.... 
//should be instantiated below here 
DMUX DMUX_INST(
.clk(clk),
.rst_n(rst_n),
.device_mac(device_mac),
.device_role(device_role),
.pktin_data_wr(pktin_data_wr),
.pktin_data(pktin_data),
.pktin_data_valid(pktin_data_valid),
.pktin_data_valid_wr(pktin_data_valid_wr),
.pktin_ready(pktin_ready),
	
.parser2rx_data_wr(inptp_data_wr),
.parser2rx_data(inptp_data),
.parser2rx_data_valid(inptp_valid),
.parser2rx_data_valid_wr(inptp_valid_wr),
.rx2parser_data_alf(~inptp_ready),

.parser2mux_data_wr(parser2mux_data_wr),
.parser2mux_data(parser2mux_data),
.parser2mux_data_valid(parser2mux_data_valid),
.parser2mux_data_valid_wr(parser2mux_data_valid_wr),
.mux2parser_data_alf(mux2parser_data_alf)	
);


PTP_1588  PTP_1588_INST(
.clk(clk),
.rst_n(rst_n),

.cfg_cs_n(cfg_cs_n), //low active
.cfg_ack_n(cfg_ack_n), //low active
.cfg_rw(cfg_rw), //0 :write, 1 :read
.cfg_addr(cfg_addr),
.cfg_wdata(cfg_wdata),
.cfg_rdata(cfg_rdata),

.cyc_init(cyc_init),
.temp_cnt(temp_cnt),
.device_mac(device_mac),
.device_role(device_role),

/////////////////input/////////////

.inptp_data_wr(inptp_data_wr),
.inptp_data(inptp_data),
.inptp_valid_wr(inptp_valid_wr),
.inptp_valid(inptp_valid),
.inptp_ready(inptp_ready),
///////////////output/////////////
.outtx_data_wr(outtx_data_wr),
.outtx_data(outtx_data),
.outtx_valid_wr(outtx_valid_wr),
.outtx_valid(outtx_valid),
.outtx_ready(~outtx_ready),
.precision_time(precision_time),
.device_id(device_id)
);



MUX MUX_INST(
.clk(clk),
.rst_n(rst_n),
	
.tx2mux_data_wr(outtx_data_wr),
.tx2mux_data(outtx_data),
.tx2mux_data_valid(outtx_valid),
.tx2mux_data_valid_wr(outtx_valid_wr),
.mux2tx_data_alf(outtx_ready),
	
.parser2mux_data_wr(parser2mux_data_wr),
.parser2mux_data(parser2mux_data),
.parser2mux_data_valid(parser2mux_data_valid),
.parser2mux_data_valid_wr(parser2mux_data_valid_wr),
.mux2parser_data_alf(mux2parser_data_alf),
	
.mux2gpp_data_wr(ptp2lcm_data_wr),
.mux2gpp_data(ptp2lcm_data),
.mux2gpp_data_valid(ptp2lcm_data_valid),
.mux2gpp_data_valid_wr(ptp2lcm_data_valid_wr),
.gpp2mux_data_alf(~ptp2lcm_ready)
);

endmodule    