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
// 1) to detect and record ping pkts in UM
// 2) to set PST value to a specific one when the pkt is ICMP
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/15
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////

module ping_cnt(
	input clk,
	input rst_n,
	input [133:0]in_ping_data,
	input in_ping_data_wr,
	input in_ping_data_valid,
	input in_ping_data_valid_wr,

	output reg [133:0]out_ping_data,
	output reg out_ping_data_wr,
	output reg out_ping_data_valid,
	output reg out_ping_data_valid_wr

);

reg [15:0] in_ping_cnt;
reg [1:0] ping_cnt_state;

//used to restore the packets 
reg [133:0] in_ping_data_dly[2:0];
reg in_ping_data_wr_dly[2:0];
reg in_ping_data_valid_wr_dly[2:0];
reg in_ping_data_valid_dly[2:0];

wire [133:0]in_ping_md;

localparam IDLE_S = 2'b00,
		   Cnt_S = 2'b01;

assign in_ping_md = in_ping_data_dly[2];

//delay the packet for 3 cycles
always @(posedge clk) begin
	in_ping_data_dly[0] <= in_ping_data;
	in_ping_data_dly[1] <= in_ping_data_dly[0];
	in_ping_data_dly[2] <= in_ping_data_dly[1];

	in_ping_data_wr_dly[0] <= in_ping_data_wr;
	in_ping_data_wr_dly[1] <= in_ping_data_wr_dly[0];
	in_ping_data_wr_dly[2] <= in_ping_data_wr_dly[1];

	in_ping_data_valid_dly[0] <= in_ping_data_valid;
	in_ping_data_valid_dly[1] <= in_ping_data_valid_dly[0];
	in_ping_data_valid_dly[2] <= in_ping_data_valid_dly[1];

	in_ping_data_valid_wr_dly[0] <= in_ping_data_valid_wr;
	in_ping_data_valid_wr_dly[1] <= in_ping_data_valid_wr_dly[0];
	in_ping_data_valid_wr_dly[2] <= in_ping_data_valid_wr_dly[1];
end




always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
		// reset
		in_ping_cnt <= 16'b0;

		out_ping_data <= 134'b0;
		out_ping_data_wr <= 1'b0;
		out_ping_data_valid <= 1'b0;
		out_ping_data_valid_wr <= 1'b0;

		ping_cnt_state <= IDLE_S;

	end
	else begin
		case(ping_cnt_state)
			IDLE_S:begin
				if(in_ping_data_wr_dly[2] == 1'b1)begin
					out_ping_data_wr <= in_ping_data_wr_dly[2];
					out_ping_data_valid <= 1'b0;
					out_ping_data_valid_wr <= 1'b0;

					//determine whether this is a ping message
					if((in_ping_data_dly[0][31:16] == 16'h0800)&&(in_ping_data[71:64] == 8'h01)) begin
						in_ping_cnt <= in_ping_cnt + 16'b1;
						out_ping_data <= {in_ping_md[133:80], 8'hff, in_ping_md[71:0]};
					end

					else begin
						in_ping_cnt <= in_ping_cnt;
						out_ping_data <= in_ping_data_dly[2];
					end
					ping_cnt_state <= Cnt_S;
				end

				else begin
					out_ping_data <= 134'b0;
					out_ping_data_wr <= 1'b0;
					out_ping_data_valid <= 1'b0;
					out_ping_data_valid_wr <= 1'b0;

					ping_cnt_state <= IDLE_S;
				end
			end

			Cnt_S:begin

				if(in_ping_data_valid_dly[2] == 1'b1)begin
					out_ping_data <= in_ping_data_dly[2];
					out_ping_data_wr <= in_ping_data_wr_dly[2];
					out_ping_data_valid <= 1'b1;
					out_ping_data_valid_wr <= 1'b1;
					ping_cnt_state <= IDLE_S;

				end

				else begin
					out_ping_data <= in_ping_data_dly[2];
					out_ping_data_wr <= in_ping_data_wr_dly[2];
					out_ping_data_valid <= 1'b0;
					out_ping_data_valid_wr <= 1'b0;
					ping_cnt_state <= Cnt_S;
				end

				
			end

		endcase
	end
end

endmodule