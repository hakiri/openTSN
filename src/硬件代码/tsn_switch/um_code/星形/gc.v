/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: gc.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
//1) Judge whether four queues can be scheduled.
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

module gc#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input	wire			clk,
input	wire			rst_n,

//receive from MB
input	wire     [3:0]  in_gc_fifo_empty,     
input	wire     [10:0] in_gc_pkt_len,

//receive from LCM
input	wire            in_gc_time_slot_flag,
input	wire     [15:0] in_gc_rate_limit,       // =100*8ns(per cycle)*(rate(bps)/8)*(10^-9)= rate/(10Mbps);dd number of token per 800ns(100 cycles); the minimum committed information rate 10Mbps is allowed.
input	wire     [15:0] TB_size,
//receive from EBM
input	wire            in_gc_pkt_valid,
//transmit to EBM
output	reg             out_gc_bandwidth_discard,       //Judge whether bandwidth reservation traffic is discarded in EBM; high active.

//receive from UDO
input	wire      [7:0]  pktout_usedw,

//transmit to TS
output	reg      [3:0]  out_gc_schedule_valid,
//receive from TS
input	wire            in_gc_q2_rden

);

wire [11:0] pkt_len;
assign pkt_len = {1'b0,in_gc_pkt_len};

//token bucken parameter
reg  [15:0] RT;         //remaining tokens
reg  [15:0] CT;         //consume tokens,1Byte consumes 1 token.
reg  [11:0]  TB_cnt;    //timer of token bucken
//************************************************************
//                   Consume tokens
//************************************************************
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        CT <= 16'd0;
        out_gc_bandwidth_discard <= 1'b0;		
    end
    else begin
		if((in_gc_q2_rden == 1'b1) && (RT >= pkt_len))begin
		    CT <= pkt_len;
		    out_gc_bandwidth_discard <= 1'b0;
		end
		else if((in_gc_q2_rden == 1'b1) && (RT < pkt_len))begin
		    CT <= 16'd0;
		    out_gc_bandwidth_discard <= 1'b1;             //discard
		end
		else begin
		    CT <= 16'd0;
		    out_gc_bandwidth_discard <= 1'b0;
		end
    end
end 
//************************************************************
//                    Count token
//************************************************************
always@(negedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        RT <= 16'd0;
		TB_cnt <= 12'd0;
    end
    else begin
	    if(TB_cnt >= 12'd999)begin
		    TB_cnt <= 12'd0;
			if(RT + in_gc_rate_limit - CT <= TB_size)begin
			    RT <= RT + in_gc_rate_limit - CT;
			end
			else begin
			    RT <= TB_size;
			end
		end
		else begin
		    TB_cnt <= TB_cnt + 12'd1;
			RT <= RT - CT;
		end
    end
end

//************************************************************
//        Judge whether four queues can be scheduled
//************************************************************
reg [1:0] state;
reg       init_flag;   
localparam IDLE_S = 2'd0,
           JUDGE_QUEUE_S = 2'd1;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_gc_schedule_valid <= 4'b0;
		init_flag <= 1'b1;
		state <= IDLE_S;
    end
    else begin
	    case(state)
            IDLE_S:begin
			    if(init_flag || in_gc_pkt_valid)begin
				    state <= JUDGE_QUEUE_S;    
				end
				else begin
				    state <= IDLE_S; 
				end
			end
            JUDGE_QUEUE_S:begin
			    if(|out_gc_schedule_valid[3:0])begin
				    out_gc_schedule_valid <= 4'b0;
					init_flag <= 1'b0;
					state <= IDLE_S;
                end					
                else begin
				    state <= JUDGE_QUEUE_S;
				    if((in_gc_time_slot_flag) && (!in_gc_fifo_empty[0]) && (pktout_usedw <= 20))begin    //Judge whether Q0 queue can be scheduled
		                out_gc_schedule_valid[0] <= 1'b1;					
                    end
		            else begin
		                out_gc_schedule_valid[0] <= 1'b0;
		            end
					
				    if((!in_gc_time_slot_flag) && (!in_gc_fifo_empty[1]) && (pktout_usedw <= 20))begin    //Judge whether Q1 queue can be scheduled
		                out_gc_schedule_valid[1] <= 1'b1;					
                    end
		            else begin
		                out_gc_schedule_valid[1] <= 1'b0;
		            end
					
					if((!in_gc_fifo_empty[2]) && ((pktout_usedw <= 20)))begin        //Judge whether Q2 queue can be scheduled
		                out_gc_schedule_valid[2] <= 1'b1;  					
                    end
		            else begin
		                out_gc_schedule_valid[2] <= 1'b0;
		            end
					
					if((!in_gc_fifo_empty[3]) && ((pktout_usedw <= 20)))begin                          //Judge whether Q3 queue can be scheduled
		                out_gc_schedule_valid[3] <= 1'b1;			
                    end
		            else begin
		                out_gc_schedule_valid[3] <= 1'b0;
		            end
				end
		    end
	    endcase
    end     
end        

endmodule