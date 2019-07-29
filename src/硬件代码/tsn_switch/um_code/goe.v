/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 Xperis, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: Hunan Xperis Network Technology Co.,Ltd.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: goe.v
//Version: 2.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)Transmit pkt to port or cpu
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2018/08/24
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

module goe #(
	parameter PLATFORM = "xilinx",
			  LMID = 8'd5
	)(

    input clk,
    input rst_n,
	
//uda pkt waiting for transmit
    input in_goe_data_wr,
    input [133:0] in_goe_data,
    input in_goe_valid_wr,
    input in_goe_valid,
	
//pkt waiting for transmit
    output reg pktout_data_wr_0,
    output reg [133:0] pktout_data_0,
    output reg pktout_data_valid_wr_0,
    output reg pktout_data_valid_0,

    //pkt waiting for transmit
    output reg pktout_data_wr_1,
    output reg [133:0] pktout_data_1,
    output reg pktout_data_valid_wr_1,
    output reg pktout_data_valid_1
);
reg [15:0]port_ping_cnt;

always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
	   port_ping_cnt <= 16'h0;
	end
	else begin
	   if((in_goe_data[133:132] == 2'b01)&&(in_goe_data[79:72] == 8'hff)&&(in_goe_data_wr == 1'b1))begin
	       port_ping_cnt <= port_ping_cnt + 16'h1;
	   end
	   else begin
	       port_ping_cnt <= port_ping_cnt;
	   end
	end
end	


reg [1:0] goe_state;
localparam  IDLE_S = 2'b0,
			Port0_S = 2'b10,
			Port1_S = 2'b11;

always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
		pktout_data_0 <= 134'b0;
		pktout_data_wr_0 <= 1'b0;
		pktout_data_valid_0 <= 1'b0;
		pktout_data_valid_wr_0 <= 1'b0;


		pktout_data_1 <= 134'b0;
		pktout_data_wr_1 <= 1'b0;
		pktout_data_valid_1 <= 1'b0;
		pktout_data_valid_wr_1 <= 1'b0;

		goe_state <= IDLE_S;
	end
	else begin
		case(goe_state)
			IDLE_S:begin
				if(in_goe_data_wr == 1'b1 && in_goe_data[117:112] == 6'b0)begin
					pktout_data_0 <= in_goe_data;
					pktout_data_wr_0 <= in_goe_data_wr;
					pktout_data_valid_0 <= in_goe_data_wr;
					pktout_data_valid_wr_0 <= in_goe_valid_wr;

					goe_state <= Port0_S;
				end

				else if(in_goe_data_wr == 1'b1 && in_goe_data[117:112] == 6'b1)begin
					pktout_data_1 <= in_goe_data;
					pktout_data_wr_1 <= in_goe_data_wr;
					pktout_data_valid_1 <= in_goe_data_wr;
					pktout_data_valid_wr_1 <= in_goe_valid_wr;

					goe_state <= Port1_S;
				end

				else begin
					pktout_data_0 <= 134'b0;
					pktout_data_wr_0 <= 1'b0;
					pktout_data_valid_0 <= 1'b0;
					pktout_data_valid_wr_0 <= 1'b0;


					pktout_data_1 <= 134'b0;
					pktout_data_wr_1 <= 1'b0;
					pktout_data_valid_1 <= 1'b0;
					pktout_data_valid_wr_1 <= 1'b0;

					goe_state <= IDLE_S;
				end
			end

			Port0_S: begin
				if(in_goe_data_wr == 1'b1 && in_goe_data[133:132] == 2'b10)begin
					pktout_data_0 <= in_goe_data;
					pktout_data_wr_0 <= in_goe_data_wr;
					pktout_data_valid_0 <= in_goe_data_wr;
					pktout_data_valid_wr_0 <= in_goe_valid_wr;

					goe_state <= IDLE_S;
				end

				else begin
					pktout_data_0 <= in_goe_data;
					pktout_data_wr_0 <= in_goe_data_wr;
					pktout_data_valid_0 <= in_goe_data_wr;
					pktout_data_valid_wr_0 <= in_goe_valid_wr;

					goe_state <= Port0_S;
				end
			end

			Port1_S: begin
				if(in_goe_data_wr == 1'b1 && in_goe_data[133:132] == 2'b10)begin
					pktout_data_1 <= in_goe_data;
					pktout_data_wr_1 <= in_goe_data_wr;
					pktout_data_valid_1 <= in_goe_data_wr;
					pktout_data_valid_wr_1 <= in_goe_valid_wr;

					goe_state <= IDLE_S;
				end

				else begin
					pktout_data_1 <= in_goe_data;
					pktout_data_wr_1 <= in_goe_data_wr;
					pktout_data_valid_1 <= in_goe_data_wr;
					pktout_data_valid_wr_1 <= in_goe_valid_wr;

					goe_state <= Port1_S;
				end
			end
		endcase
		
	end
end
endmodule			