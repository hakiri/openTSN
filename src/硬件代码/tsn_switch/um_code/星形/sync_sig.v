/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 Xperis, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: Hunan Xperis Network Technology Co.,Ltd.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: sync_sig.v
//Version: 2.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)synchronize signal to Des Clock filed 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2018/07/17
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

`timescale 1 ns / 1 ps
module sync_sig(
	input clk,
	input rst_n,
	input in_sig,
	output reg out_sig
);
parameter SHIFT_WIDTH = 2;

reg[SHIFT_WIDTH-1:0] sig_dly;

always @(posedge clk or negedge rst_n) begin
	if(~rst_n) begin
		sig_dly <= {SHIFT_WIDTH{1'b0}};
	end
	else begin//Sync signal
		sig_dly[0] <= in_sig;
		sig_dly[SHIFT_WIDTH-1:1] <= sig_dly[SHIFT_WIDTH-2:0];
  end
end

always @(posedge clk or negedge rst_n) begin
	if(~rst_n) begin
		out_sig <= 1'b0;
	end
	else begin//Sync signal
		if((|sig_dly) == 1'b0) begin
            out_sig <= 1'b0;
        end
        else if((&sig_dly) == 1'b1) begin
            out_sig <= 1'b1;
        end
        else begin
            out_sig <= out_sig;
        end
  end
end

endmodule