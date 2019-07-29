/////////////////////////////////////////////////////////////////
// MIT License
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx/Intel
//Filename: LUpdate.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)LUpdate: generate beacon report message;
//        process beacon update message.
//*************************************************************
//                     Revision List
//*************************************************************
//      date:  2019/05/13
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 
`timescale 1ns / 1ps

module lupdate #(
	parameter   LMID=8'd12
	)(
	input clk,
	input rst_n,

//lreport signal 
	input [133:0] in_lu_data,
	input in_lu_data_wr,
	input in_lu_data_valid,
	input in_lu_data_valid_wr,
	
	input [47:0] in_local_mac_id,

//esw signal 
	output reg [133:0] out_lu_data,
	output reg out_lu_data_wr,
	output reg out_lu_data_valid,
	output reg out_lu_data_valid_wr,
	output reg out_local_mac_id,

	output reg beacon_update_master,

//changeable registers and counters
	output reg [31:0] time_slot_period,
	output reg direction,
	output reg [31:0] token_bucket_para,
	output reg [47:0] direct_mac_addr
);

//***************************************************
//        Intermediate variable Declaration
//***************************************************
//all wire/reg/parameter variable 
//should be declare below here 
localparam msg_type_update = 4'hf; //beacon update message

//delay regs, we need to delay for 2 cycles to identify an update message.
reg [133:0] lu_data_1;
reg lu_data_wr_1;
reg lu_data_valid_1;
reg lu_data_valid_wr_1;

reg [133:0] lu_data_2;
reg lu_data_wr_2;
reg lu_data_valid_2;
reg lu_data_valid_wr_2;


always @(posedge clk) begin
	lu_data_1 <= in_lu_data;
	lu_data_wr_1 <= in_lu_data_wr;
	lu_data_valid_1 <= in_lu_data_valid;
	lu_data_valid_wr_1 <= in_lu_data_valid_wr;

	lu_data_2 <= lu_data_1;
	lu_data_wr_2 <= lu_data_wr_1;
	lu_data_valid_2 <= lu_data_valid_1;
	lu_data_valid_wr_2 <= lu_data_valid_wr_1;

end

//lupdate control state machine
reg [4:0] update_pkt_cnt;
reg [2:0] lupdate_state;

localparam IDLE_S = 3'b001,
		   UPDATE_S = 3'b010,
		   TRAN_S = 3'b011,
		   DISC_S = 3'b100;



always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
		// reset
		lupdate_state <= 3'b0;
		update_pkt_cnt <= 5'b0;
		direction <= 1'b0;
		token_bucket_para <= 32'd10;
		direct_mac_addr <= 48'b0;
		time_slot_period <= 32'h7a12;  //reset as 250us

		out_lu_data <= 134'b0;
		out_lu_data_wr <= 1'b0;
		out_lu_data_valid <= 1'b0;
		out_lu_data_valid_wr <= 1'b0;
		beacon_update_master <= 1'b0;

		lupdate_state <= IDLE_S;

   	end

	else begin
		case(lupdate_state)
			IDLE_S:begin
				update_pkt_cnt <= 5'b0;
				//the cycle that the dst_mac can be obtained by lupdate
				if(lu_data_wr_2 == 1'b1 && lu_data_2[133:132] == 2'b01)begin

					if(in_lu_data[127:80] == in_local_mac_id && in_lu_data[11:8] == msg_type_update) begin
						out_lu_data <= 134'b0;
						out_lu_data_wr <= 1'b0;
						out_lu_data_valid <= 1'b0;
						out_lu_data_valid_wr <= 1'b0;

						lupdate_state <= UPDATE_S;
					end
					//if this is a pkt from LOCAL LCM but comes from outside, need to drop it.
					else if(in_lu_data[79:32] == in_local_mac_id && lu_data_2[127] == 1'b0)begin
						out_lu_data <= 134'b0;
						out_lu_data_wr <= 1'b0;
						out_lu_data_valid <= 1'b0;
						out_lu_data_valid_wr <= 1'b0;

						lupdate_state <= DISC_S;
					end

					else begin
						out_lu_data <= lu_data_2;
						out_lu_data_wr <= lu_data_wr_2;
						out_lu_data_valid <= lu_data_valid_2;
						out_lu_data_valid_wr <= lu_data_valid_wr_2;

						lupdate_state <= TRAN_S;
					end
				end

				else begin
					out_lu_data <= 134'b0;
					out_lu_data_wr <= 1'b0;
					out_lu_data_valid <= 1'b0;
					out_lu_data_valid_wr <= 1'b0;

					lupdate_state <= IDLE_S;
				end
			end

			DISC_S:begin
				if(in_lu_data_wr == 1'b1 && in_lu_data[133:132] == 2'b10)begin
					out_lu_data <= 134'b0;
					out_lu_data_wr <= 1'b0;
					out_lu_data_valid <= 1'b0;
					out_lu_data_valid_wr <= 1'b0;

					lupdate_state <= IDLE_S;
				end

				else begin
					out_lu_data <= 134'b0;
					out_lu_data_wr <= 1'b0;
					out_lu_data_valid <= 1'b0;
					out_lu_data_valid_wr <= 1'b0;

				end
			end

			UPDATE_S:begin
				update_pkt_cnt <= update_pkt_cnt + 5'b1;

				out_lu_data <= 134'b0;
				out_lu_data_wr <= 1'b0;
				out_lu_data_valid <= 1'b0;
				out_lu_data_valid_wr <= 1'b0;

				case(update_pkt_cnt)
					//5th cycle is the executable field. 
					5'd5:begin
						direction <= lu_data_2[79];
						token_bucket_para <= lu_data_2[63:32];
						direct_mac_addr <= lu_data_2[127:80];
						time_slot_period <= lu_data_2[31:0];
					end

					5'd11:begin
						beacon_update_master <= ~beacon_update_master;
						lupdate_state <= IDLE_S;
					end
				endcase
			end

			TRAN_S:begin
				if(lu_data_wr_2==1'b1 && lu_data_2[133:132]==2'b10) begin
					out_lu_data <= lu_data_2;
					out_lu_data_wr <= lu_data_wr_2;
					out_lu_data_valid <= lu_data_valid_2;
					out_lu_data_valid_wr <= lu_data_valid_wr_2;

					lupdate_state <= IDLE_S;
				end

				else begin
					out_lu_data <= lu_data_2;
					out_lu_data_wr <= lu_data_wr_2;
					out_lu_data_valid <= lu_data_valid_2;
					out_lu_data_valid_wr <= lu_data_valid_wr_2;

					lupdate_state <= TRAN_S;
				end
			end
		endcase
	end
end



endmodule