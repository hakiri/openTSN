/////////////////////////////////////////////////////////////////
// MIT License
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx/Intel
//Filename: lcm.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)LCM: generate beacon report message;
//        process beacon update message.
//*************************************************************
//                     Revision List
//*************************************************************
//      date:  2019/05/13
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 
`timescale 1ns / 1ps

module lcm #(
	parameter    LMID = 8'd1
	)(
	input clk,
	input rst_n,
//um signals
	input [133:0] in_lcm_data,
	input in_lcm_data_wr,
	input in_lcm_data_valid,
	input in_lcm_data_valid_wr,
	output pktin_ready,
	(*mark_debug="TRUE"*)input [47:0] precision_time,

	input [47:0] in_local_mac_id,

//esw signals 
	output out_lcm_data_wr,
	output [133:0] out_lcm_data,
	output out_lcm_data_valid,
	output out_lcm_data_valid_wr,

//readable & changeable registers and counters
	output wire out_direction,
	output wire [31:0] out_token_bucket_para,
	output wire [47:0] out_direct_mac_addr,


	//input from esw
	input [63:0] esw_pktin_cnt,
	input [63:0] esw_pktout_cnt,
	input [7:0] bufm_id_cnt,

	//input from eos
	input [7:0] eos_q0_used_cnt,
	input [7:0] eos_q1_used_cnt,
	input [7:0] eos_q2_used_cnt,
	input [7:0] eos_q3_used_cnt,

	(*mark_debug="TRUE"*)output reg time_slot_flag,
	input [63:0] eos_mdin_cnt,
	input [63:0] eos_mdout_cnt,

	//input from goe
	
	input [63:0] goe_pktin_cnt,
	input [63:0] goe_port0out_cnt,
	input [63:0] goe_port1out_cnt,
	input [63:0] goe_discard_cnt

);


//***************************************************
//        Intermediate variable Declaration
//***************************************************
//all wire/reg/parameter variable 
//should be declare below here 
wire [133:0] lr2lu_data;
wire lr2lu_data_wr;
wire lr2lu_data_valid;
wire lr2lu_data_valid_wr;

wire [47:0] lr2lu_local_mac_id;

wire lr2lu_direction;
wire [47:0] lr2lu_direc_mac_addr;
wire [31:0] lr2lu_token_bucket_para;
wire beacon_update_master;
(*mark_debug="TRUE"*)wire [31:0] time_slot_period;

//parameter time_slot = 16'h7a12;  //counting for 250us
//(*mark_debug="TRUE"*)reg [15:0] time_slot_cnt;  //used to reset every 0x7a12 cycles.

//pkt transmit cnt
reg [31:0] lcm_pkt_recv_cnt;
reg [31:0] lcm_pkt_sent_cnt;


//---------------reverse time slot for CQF---------------//
always @(posedge clk or negedge rst_n) begin
	//濡備綍鍒ゆ柇0xH7A12鐨勬暣鏁�???
	if(!rst_n) begin
		time_slot_flag <= 17'h0;
	end
	else begin
	    if(precision_time[16:0] == 17'h0)begin
	        time_slot_flag <= 17'h0;
	    end
		else if(precision_time[16:0] == time_slot_period ) begin
			time_slot_flag <= ~time_slot_flag;   //reverse time_slot
		end
		else if(precision_time[16:0] == time_slot_period << 1) begin
			time_slot_flag <= ~time_slot_flag; 
		end
		else if(precision_time[16:0] == (time_slot_period << 1) + time_slot_period ) begin
		    time_slot_flag <= ~time_slot_flag; 
		end
		else begin
		    time_slot_flag <= time_slot_flag; 
		end
	end
end
//---------------reverse time slot for CQF---------------//



//---------------packets transmit count---------------//
always @(posedge clk or negedge rst_n) begin
	if(!rst_n)begin
		lcm_pkt_recv_cnt <= 32'b0;
		lcm_pkt_sent_cnt <= 32'b0;
	end

	else begin
		if(in_lcm_data_valid == 1'b1) begin
			lcm_pkt_recv_cnt <= lcm_pkt_recv_cnt + 32'b1;
		end

		if(out_lcm_data_valid == 1'b1) begin
			lcm_pkt_sent_cnt <= lcm_pkt_sent_cnt + 32'b1;
		end
	end
end
//---------------packet transmit count---------------//

//sub-modules
lreport #(
	)lreport(

.clk(clk),
.rst_n(rst_n),
.in_lr_data_wr(in_lcm_data_wr),
.in_lr_data(in_lcm_data),
.in_lr_data_valid(in_lcm_data_valid),
.in_lr_data_valid_wr(in_lcm_data_valid_wr),
.pktin_ready(pktin_ready),
.precision_time(precision_time),

.out_lr_data(lr2lu_data),
.out_lr_data_wr(lr2lu_data_wr),
.out_lr_data_valid(lr2lu_data_valid),
.out_lr_data_valid_wr(lr2lu_data_valid_wr),
.beacon_update_master(beacon_update_master),

.direction(out_direction),
.token_bucket_para(out_token_bucket_para),
.direct_mac_addr(out_direct_mac_addr),
.time_slot_period(time_slot_period),

.esw_pktin_cnt(esw_pktin_cnt),
.esw_pktout_cnt(esw_pktout_cnt),
.bufm_id_cnt(bufm_id_cnt),


.eos_q0_used_cnt(eos_q0_used_cnt),
.eos_q1_used_cnt(eos_q1_used_cnt),
.eos_q2_used_cnt(eos_q2_used_cnt),
.eos_q3_used_cnt(eos_q3_used_cnt),

.eos_mdin_cnt(eos_mdin_cnt),
.eos_mdout_cnt(eos_mdout_cnt),

.goe_pktin_cnt(goe_pktin_cnt),
.goe_port0out_cnt(goe_port0out_cnt),
.goe_port1out_cnt(goe_port1out_cnt),
.goe_discard_cnt(goe_discard_cnt),

.in_local_mac_id(in_local_mac_id),
.out_local_mac_id(lr2lu_local_mac_id)

);


lupdate #(
	)lupdate(

.clk(clk),
.rst_n(rst_n),

.in_lu_data(lr2lu_data),
.in_lu_data_wr(lr2lu_data_wr),
.in_lu_data_valid(lr2lu_data_valid),
.in_lu_data_valid_wr(lr2lu_data_valid_wr),
.in_local_mac_id(in_local_mac_id),

.beacon_update_master(beacon_update_master),

.out_lu_data(out_lcm_data),
.out_lu_data_wr(out_lcm_data_wr),
.out_lu_data_valid(out_lcm_data_valid),
.out_lu_data_valid_wr(out_lcm_data_valid_wr),

.direction(out_direction),
.token_bucket_para(out_token_bucket_para),
.direct_mac_addr(out_direct_mac_addr),
.time_slot_period(time_slot_period)
);

endmodule