/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: mb.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
//1) cache metadata.
//  
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/16
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
`timescale 1ns/1ps

module mb#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input	wire			clk,
input	wire			rst_n,

//receive from QS
input	wire     [8:0]  in_mb_md0,       
input	wire            in_mb_md0_wr,                       
input	wire     [8:0]  in_mb_md1,       
input	wire            in_mb_md1_wr,               
input	wire     [19:0] in_mb_md2,       
input	wire            in_mb_md2_wr,                 
input	wire     [8:0]  in_mb_md3,       
input	wire            in_mb_md3_wr,

//transmit to GC
output	wire     [3:0]  out_mb_fifo_empty,     
output	wire     [10:0] out_mb_pkt_len,

//receive from TS
input	wire            in_mb_q0_rden, 
input	wire            in_mb_q1_rden,
input	wire            in_mb_q2_rden, 
input	wire            in_mb_q3_rden,  

//transmit to TS
output	reg      [7:0]  out_mb_md,
output	reg             out_mb_md_wr,    

//transmit to LCM 
(*MARK_DEBUG="TRUE"*)output	wire      [7:0]  out_mb_q0_used_cnt,
(*MARK_DEBUG="TRUE"*)output	wire      [7:0]  out_mb_q1_used_cnt,
(*MARK_DEBUG="TRUE"*)output	wire      [7:0]  out_mb_q2_used_cnt, 
(*MARK_DEBUG="TRUE"*)output	wire      [7:0]  out_mb_q3_used_cnt
);

wire      [4:0]  mb_q0_used_cnt;
wire      [4:0]  mb_q1_used_cnt;
wire      [4:0]  mb_q2_used_cnt; 
wire      [4:0]  mb_q3_used_cnt;
assign out_mb_q0_used_cnt = {3'b0,mb_q0_used_cnt};
assign out_mb_q1_used_cnt = {3'b0,mb_q1_used_cnt};
assign out_mb_q2_used_cnt = {3'b0,mb_q2_used_cnt};
assign out_mb_q3_used_cnt = {3'b0,mb_q3_used_cnt};

//fifo dout
wire   [8:0]  q0fifo_rmd;
wire   [8:0]  q1fifo_rmd;
wire   [19:0] q2fifo_rmd;
wire   [8:0]  q3fifo_rmd;

assign out_mb_pkt_len = q2fifo_rmd[19:9];

always@(posedge clk or negedge rst_n) begin
   if(rst_n == 1'b0) begin
        out_mb_md <= 8'b0;  
        out_mb_md_wr <= 1'b0;
   end
   else begin
        if(in_mb_q0_rden == 1'b1)begin
		      out_mb_md <= q0fifo_rmd[7:0];
              out_mb_md_wr <= 1'b1;		 
         end
         else if(in_mb_q1_rden == 1'b1)begin
 		     out_mb_md <= q1fifo_rmd[7:0];
             out_mb_md_wr <= 1'b1;            
         end
         else if(in_mb_q2_rden == 1'b1) begin
 		     out_mb_md <= q2fifo_rmd[7:0];
             out_mb_md_wr <= 1'b1;            
         end
         else if(in_mb_q3_rden == 1'b1) begin
              out_mb_md <= q3fifo_rmd[7:0];
              out_mb_md_wr <= 1'b1;            
         end
         else begin
             out_mb_md <= 8'b0;    
             out_mb_md_wr <= 1'b0;      
         end
    end
end

 q0fifo_9_16  q0fifo_9_16_inst(                     //cache even-time-slot TSN metadata
	.srst(~rst_n),
	.clk(clk),
	.din(in_mb_md0),
	.rd_en(in_mb_q0_rden),
	.wr_en(in_mb_md0_wr),
	.dout(q0fifo_rmd),
	.data_count(mb_q0_used_cnt),
	.empty(out_mb_fifo_empty[0]),
	.full()
	);
	
 q1fifo_9_16  q1fifo_9_16_inst(                   //cache odd-time-slot TSN metadata
	.srst(~rst_n),
	.clk(clk),
	.din(in_mb_md1),
	.rd_en(in_mb_q1_rden),
	.wr_en(in_mb_md1_wr),
	.dout(q1fifo_rmd),
	.data_count(mb_q1_used_cnt),
	.empty(out_mb_fifo_empty[1]),
	.full()
	);

 q2fifo_20_16  q2fifo_20_16_inst(                 //cache bandwidth reservation metadata and PTP metadata
	.srst(~rst_n),
	.clk(clk),
	.din(in_mb_md2),
	.rd_en(in_mb_q2_rden),
	.wr_en(in_mb_md2_wr),
	.dout(q2fifo_rmd),
	.data_count(mb_q2_used_cnt),
	.empty(out_mb_fifo_empty[2]),
	.full()
	);	

 q3fifo_9_16  q3fifo_9_16_inst(                  //cache best effort metadata
	.srst(~rst_n),
	.clk(clk),
	.din(in_mb_md3),
	.rd_en(in_mb_q3_rden),
	.wr_en(in_mb_md3_wr),
	.dout(q3fifo_rmd),
	.data_count(mb_q3_used_cnt),
	.empty(out_mb_fifo_empty[3]),
	.full()
	);		
endmodule
    