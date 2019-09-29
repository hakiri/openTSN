/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: qs.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
//1) select queue.
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

module qs#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input	wire			clk,
input	wire			rst_n,

//receive from LCM
input	wire			in_qs_time_slot_flag,

//receive from IBM
input	wire	[23:0]	in_qs_md,
input	wire			in_qs_md_wr,

//transmit to MB
output	reg 	[8:0]	out_qs_md0,       //even-time-slot TSN metadata
output	reg 			out_qs_md0_wr,
output	reg 	[8:0]	out_qs_md1,       //odd-time-slot TSN metadata
output	reg 			out_qs_md1_wr,
output	reg 	[19:0]	out_qs_md2,       //bandwidth reservation metadata and PTP metadata
output	reg 			out_qs_md2_wr,
output	reg 	[8:0]	out_qs_md3,       //best effort metadata
output	reg 			out_qs_md3_wr

);

//*******************************************
//         transmit MD to MB
//*******************************************

always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
	    out_qs_md0 <= 9'b0;  
		out_qs_md0_wr <= 1'b0;
		
		out_qs_md1 <= 9'b0;  
		out_qs_md1_wr <= 1'b0;
		
		out_qs_md2 <= 20'b0;  
		out_qs_md2_wr <= 1'b0;
		
		out_qs_md3 <= 9'b0; 
		out_qs_md3_wr <= 1'b0;	
    end 
    else begin 
	    if(in_qs_md_wr == 1'b1)begin
	        if(in_qs_md[23:21] == 3'd3 && in_qs_time_slot_flag == 1'b0) begin
		        out_qs_md0 <= in_qs_md[8:0];  
		    	out_qs_md0_wr <= 1'b1;
		    end
		    else if(in_qs_md[23:21] == 3'd3 && in_qs_time_slot_flag == 1'b1)begin
		        out_qs_md1 <= in_qs_md[8:0];
			    out_qs_md1_wr <= 1'b1;
		    end
		    else if(in_qs_md[23:21] == 3'd2)begin
                out_qs_md2[19:9] <= 11'd0;               //not do traffic shaping in GC module, and not consume tokens
                out_qs_md2[8:0] <= in_qs_md[8:0];
                out_qs_md2_wr <= 1'b1;
            end
			else if(in_qs_md[23:21] == 3'd1)begin
			    out_qs_md2[19:9] <= in_qs_md[20:9] - 12'd32;        //pkt_length(Byte) - 2 cycle's metadata
			    out_qs_md2[8:0] <= in_qs_md[8:0];
				out_qs_md2_wr <= 1'b1;
			end
			else if(in_qs_md[23:21] == 3'd0)begin
			    out_qs_md3 <= in_qs_md[8:0];
				out_qs_md3_wr <= 1'b1;
			end
			else begin
			    out_qs_md0 <= 9'b0;  
				out_qs_md0_wr <= 1'b0;
				
				out_qs_md1 <= 9'b0;  
				out_qs_md1_wr <= 1'b0;
				
				out_qs_md2 <= 20'b0;  
				out_qs_md2_wr <= 1'b0;
				
				out_qs_md3 <= 9'b0; 
				out_qs_md3_wr <= 1'b0;		
			end
	    end
		else begin
		    out_qs_md0 <= 9'b0;  
		    out_qs_md0_wr <= 1'b0;
		    
		    out_qs_md1 <= 9'b0;  
		    out_qs_md1_wr <= 1'b0;
		    
		    out_qs_md2 <= 20'b0;  
		    out_qs_md2_wr <= 1'b0;
		    
		    out_qs_md3 <= 9'b0; 
		    out_qs_md3_wr <= 1'b0;	
		end
	end	
end
endmodule
