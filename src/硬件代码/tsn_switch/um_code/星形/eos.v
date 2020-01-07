/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: eos.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// EOS:achieve cyclic queuing and forwarding(CQF);
//     do traffic shaping for bandwidth reservation traffic;
//     priority scheduling.
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/16
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module eos#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input	wire			clk,
input	wire			rst_n,
    
//receive from LCM
input	wire			 in_eos_time_slot_flag,
input	wire     [15:0] in_eos_rate_limit, 
input	wire     [15:0] in_eos_depth_limit,              
//transmit to LCM
output	wire     [7:0]  out_eos_q0_used_cnt,
output	wire     [7:0]  out_eos_q1_used_cnt,
output	wire     [7:0]  out_eos_q2_used_cnt, 
output	wire     [7:0]  out_eos_q3_used_cnt,
output	reg      [63:0] out_eos_mdin_cnt, 
output	reg      [63:0] out_eos_mdout_cnt, 

//receive from IBM
input	wire	 [23:0] in_eos_md,
input	wire            in_eos_md_wr,  

//receive from UDO
input	wire     [7:0]  pktout_usedw,         

//receive from EBM
input	wire	         in_eos_pkt_valid,
//transmit to EBM
output	wire            out_eos_bandwidth_discard,
output	wire      [7:0] out_eos_md, 
output	wire            out_eos_md_wr

);

//***************************************************
//        Intermediate variable Declaration
//***************************************************

//QS to MB
wire [8:0]	qs2mb_md0;  
wire 		qs2mb_md0_wr;
wire [8:0]	qs2mb_md1; 
wire 		qs2mb_md1_wr;
wire [19:0]	qs2mb_md2; 
wire 		qs2mb_md2_wr;
wire [8:0]	qs2mb_md3; 
wire    	qs2mb_md3_wr;

//MB to GC
wire [3:0]  mb2gc_md_outport;
wire [3:0]  mb2gc_fifo_empty;
wire [10:0]  mb2gc_pkt_len;

//MB to TS
wire        ts2mb_q0_rden;
wire        ts2mb_q1_rden;
wire        ts2mb_q2_rden;
wire        ts2mb_q3_rden;
wire [7:0]  mb2ts_md;
wire        mb2ts_md_wr;

//GC to TS
wire [3:0]  gc2ts_schedule_valid;


//*******************************************
//              count md
//*******************************************
always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
	    out_eos_mdin_cnt <= 64'b0;
        out_eos_mdout_cnt <= 64'b0;		
    end 
    else begin 
	    if(in_eos_md_wr == 1'b1) begin
		    out_eos_mdin_cnt <= out_eos_mdin_cnt + 64'd1;
		end
		else begin
		    out_eos_mdin_cnt <= out_eos_mdin_cnt;
		end
		
		if(out_eos_md_wr == 1'b1) begin
		    out_eos_mdout_cnt <= out_eos_mdout_cnt + 64'd1;
		end	
		else begin
		    out_eos_mdout_cnt <= out_eos_mdout_cnt;
		end		
	end	
end

//***************************************************
//                  Module Instance
//***************************************************
qs qs_inst(
.clk(clk),
.rst_n(rst_n), 
//receive md from IBM
.in_qs_md(in_eos_md),
.in_qs_md_wr(in_eos_md_wr),
//receive time_slot_flag from LCM
.in_qs_time_slot_flag(in_eos_time_slot_flag),
//transmit md to MB
.out_qs_md0(qs2mb_md0),   
.out_qs_md0_wr(qs2mb_md0_wr),
.out_qs_md1(qs2mb_md1),   
.out_qs_md1_wr(qs2mb_md1_wr),
.out_qs_md2(qs2mb_md2),   
.out_qs_md2_wr(qs2mb_md2_wr),
.out_qs_md3(qs2mb_md3),   
.out_qs_md3_wr(qs2mb_md3_wr)
);

mb mb_inst(
.clk(clk),
.rst_n(rst_n),
//receive md from qs 
.in_mb_md0(qs2mb_md0),  
.in_mb_md0_wr(qs2mb_md0_wr),
.in_mb_md1(qs2mb_md1),  
.in_mb_md1_wr(qs2mb_md1_wr),
.in_mb_md2(qs2mb_md2),  
.in_mb_md2_wr(qs2mb_md2_wr),
.in_mb_md3(qs2mb_md3),  
.in_mb_md3_wr(qs2mb_md3_wr),
//transmit to GC
.out_mb_fifo_empty(mb2gc_fifo_empty),
.out_mb_pkt_len(mb2gc_pkt_len),
//receive from TS 
.in_mb_q0_rden(ts2mb_q0_rden),
.in_mb_q1_rden(ts2mb_q1_rden),
.in_mb_q2_rden(ts2mb_q2_rden),
.in_mb_q3_rden(ts2mb_q3_rden),
//transmit mb to TS
.out_mb_md(mb2ts_md),
.out_mb_md_wr(mb2ts_md_wr),
//transmit cnt to LCM
.out_mb_q0_used_cnt(out_eos_q0_used_cnt),
.out_mb_q1_used_cnt(out_eos_q1_used_cnt),
.out_mb_q2_used_cnt(out_eos_q2_used_cnt),
.out_mb_q3_used_cnt(out_eos_q3_used_cnt)

);

gc gc_inst(
.clk(clk),
.rst_n(rst_n),
//receive from LCM 
.in_gc_time_slot_flag(in_eos_time_slot_flag),
.in_gc_rate_limit(in_eos_rate_limit),  
.TB_size(in_eos_depth_limit), 
//receive from MB 
.in_gc_fifo_empty(mb2gc_fifo_empty),
.in_gc_pkt_len(mb2gc_pkt_len),
//receive from EBM
.in_gc_pkt_valid(in_eos_pkt_valid),
//transmit to EBM
.out_gc_bandwidth_discard(out_eos_bandwidth_discard),
//receive from UDO
.pktout_usedw(pktout_usedw),
//receive from TS 
.in_gc_q2_rden(ts2mb_q2_rden),
//transmit to TS
.out_gc_schedule_valid(gc2ts_schedule_valid)

);

ts ts_inst(
.clk(clk),
.rst_n(rst_n),
//receive from MB 
.in_ts_md(mb2ts_md), 
.in_ts_md_wr(mb2ts_md_wr),
//transmit to MB
.out_ts_q0_rden(ts2mb_q0_rden),
.out_ts_q1_rden(ts2mb_q1_rden),
.out_ts_q2_rden(ts2mb_q2_rden),
.out_ts_q3_rden(ts2mb_q3_rden),
//transmit to EBM
.out_ts_md(out_eos_md), 
.out_ts_md_wr(out_eos_md_wr),
//receive from GC
.in_ts_schedule_valid(gc2ts_schedule_valid)

);

endmodule



