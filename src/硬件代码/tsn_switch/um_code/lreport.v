/////////////////////////////////////////////////////////////////
// MIT License
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx/Intel
//Filename: lreport.v
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
module lreport #(
	parameter LMID = 8'd11
	)(
	input clk,
	input rst_n,

//um signal 
	input in_lr_data_wr,
	input [133:0] in_lr_data,
	input in_lr_data_valid,
	input in_lr_data_valid_wr,

	output reg pktin_ready,
	input [47:0] precision_time,
	input [47:0] in_local_mac_id,   //should be changed to [47:0], represents a mac addr.

//lupdate signal 
	output reg out_lr_data_wr,
	output reg [133:0] out_lr_data,
	output reg out_lr_data_valid,
	output reg out_lr_data_valid_wr,

	output [47:0] out_local_mac_id,  //should be changed to [47:0], represents a mac addr.

	input beacon_update_master,

//readable & changeable registers and counters

	input direction,
	input [15:0] token_bucket_para,
	input [15:0] token_bucket_depth,
	input [47:0] direct_mac_addr,
	input [31:0]time_slot_period,

	//input from esw
	input [63:0] esw_pktin_cnt,
	input [63:0] esw_pktout_cnt,
	input [7:0] bufm_id_cnt,

	//input from eos
	input [7:0] eos_q0_used_cnt,
	input [7:0] eos_q1_used_cnt,
	input [7:0] eos_q2_used_cnt,
	input [7:0] eos_q3_used_cnt,

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

//local_mac_addr = 00-06-06-02-00-00-00-ID

localparam cnc_mac_addr = 48'h010203040506;  //CNC鑺傜偣mac鍦板�?
//parameter time_inteval = 20'hFFFFF;  //鍚屾鍛ㄦ湡�??8ms宸﹀�?  2^20 us


reg [47:0] time_stamp_rec; //record the accurate timestamp.
reg report_flag_master;
reg report_flag_slave;

reg beacon_update_slave;

reg [4:0] beacon_report_cycle;

//restore the in_data when needed
reg [133:0] lr_data;
reg lr_data_wr;
reg lr_data_valid;
reg lr_data_valid_wr;

//lreport state machine
reg [2:0] lreport_state;
reg [15:0] ptp_seq;

assign out_local_mac_id = in_local_mac_id;
//=========================== beacon report ========================//
localparam IDLE_S = 3'b001,
		TRAN_S = 3'b010,
		BTRAN_S = 3'b011,
		Set1_S = 3'b110,
		Set2_S = 3'b111,
		Set3_S = 3'b100;
always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
		// reset
		out_lr_data <= 134'b0;
		out_lr_data_wr <= 1'b0;
		out_lr_data_valid <= 1'b0;
		out_lr_data_valid_wr <= 1'b0; 
		report_flag_slave <= 1'b0; //set to 1'b1 while debug
		pktin_ready <= 1'b1;
		//time_stamp_rec <= 48'b0;
		ptp_seq <= 16'b0;

		beacon_update_slave <= 1'b0;
		/***********reg for 1 cycle***********/
		lr_data <= 134'b0;
		lr_data_wr <= 1'b0;
		lr_data_valid <= 1'b0;
		lr_data_valid_wr <= 1'b0;
		/***********reg for 1 cycle***********/

		beacon_report_cycle <= 5'b0;
		lreport_state <= IDLE_S;
	end
	else begin
		case(lreport_state)
			IDLE_S:begin
				//need to reverse
				if(report_flag_slave == ~report_flag_master && in_lr_data_wr == 1'b0) begin
					out_lr_data <= 134'b0;
                    out_lr_data_wr <= 1'b0;
                    out_lr_data_valid <= 1'b0;
                    out_lr_data_valid_wr <= 1'b0; 
					pktin_ready <= 1'b0;
					//time_stamp_rec <= precision_time;
					lreport_state <= Set1_S;
				end
				else begin
					
					if(in_lr_data_wr == 1'b1)begin
						out_lr_data <= {in_lr_data[133:88], 8'b1, in_lr_data[79:0]};
						out_lr_data_wr <= in_lr_data_wr;
						out_lr_data_valid <= in_lr_data_valid;
						out_lr_data_valid_wr <= in_lr_data_valid_wr;
						
						pktin_ready <= 1'b1;

						beacon_report_cycle <= 5'b0;
						lreport_state <= TRAN_S;
					end
					else begin
						report_flag_slave <= report_flag_master;
						out_lr_data <= 134'b0;
						out_lr_data_wr <= 1'b0;
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0; 
						pktin_ready <= 1'b1;

						beacon_report_cycle <= 5'b0;
						lreport_state <= IDLE_S;
					end
				end
			end

			Set1_S:begin
				if(in_lr_data_wr == 1'b0) begin
					//pktin_ready success, need to transfer the beacon packet
					lreport_state <= BTRAN_S;
				end

				//need to delay for 1 cycle.
				else begin
					lr_data <= in_lr_data;
					lr_data_wr <= in_lr_data_wr;
					lr_data_valid <= in_lr_data_valid;
					lr_data_valid_wr <= in_lr_data_valid_wr;

					pktin_ready <= 1'b1;
					lreport_state <= Set2_S;
				end
			end

			Set2_S:begin

				if(in_lr_data_wr == 1'b1) begin
					out_lr_data <= lr_data;
					out_lr_data_wr <= lr_data_wr;
					out_lr_data_valid <= lr_data_valid;
					out_lr_data_valid_wr <= lr_data_valid_wr;

					lr_data <= in_lr_data;
					lr_data_wr <= in_lr_data_wr;
					lr_data_valid <= in_lr_data_valid;
					lr_data_valid_wr <= in_lr_data_valid_wr;

					if(in_lr_data[133:132] == 2'b10)begin
						lreport_state <= Set3_S;
					end

				end

				else begin
					out_lr_data <= lr_data;
					out_lr_data_wr <= lr_data_wr;
					out_lr_data_valid <= lr_data_valid;
					out_lr_data_valid_wr <= lr_data_valid_wr;

					lreport_state <= TRAN_S;
				end
				
			end

			Set3_S: begin
				out_lr_data <= lr_data;
				out_lr_data_wr <= lr_data_wr;
				out_lr_data_valid <= lr_data_valid;
				out_lr_data_valid_wr <= lr_data_valid_wr;
				
				lreport_state <= IDLE_S;
			end

			TRAN_S: begin
				if(in_lr_data[133:132] == 2'b10)begin
					out_lr_data<= in_lr_data;
					out_lr_data_wr <= in_lr_data_wr;
					out_lr_data_valid <= in_lr_data_valid;
					out_lr_data_valid_wr <= in_lr_data_valid_wr;

					//ready to send beacon report
					lreport_state <= IDLE_S;
				end

				else begin
					out_lr_data<= in_lr_data;
					out_lr_data_wr <= in_lr_data_wr;
					out_lr_data_valid <= in_lr_data_valid;
					out_lr_data_valid_wr <= in_lr_data_valid_wr;

					lreport_state <= TRAN_S;
				end
			end

			//send a beacon report message
			BTRAN_S:begin
				beacon_report_cycle <= beacon_report_cycle + 4'b1;
				
				case(beacon_report_cycle)
					4'd0:begin
						out_lr_data_wr <= 1'b1;
						//smid == 8'd128;
						out_lr_data <= {2'b01,4'b0,1'b1,1'b0,6'b0,2'b0,6'b0,16'd208, 8'd128, 8'd1, 32'b0, time_stamp_rec};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end

					4'd1:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,1'b0,1'b0,6'b0,2'b0,6'b0,112'b0};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					
					4'd2:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
						if(beacon_update_slave != beacon_update_master) begin
							out_lr_data <= {2'b11, 4'b0, cnc_mac_addr, in_local_mac_id, 16'h88f7, 4'b0, 4'he, 8'b0};
							beacon_update_slave <= beacon_update_master;
						end
						else begin
							out_lr_data <= {2'b11, 4'b0, cnc_mac_addr, in_local_mac_id, 16'h88f7, 4'b0, 4'hf, 8'b0};
						end
					end
					4'd3:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,16'd176,112'b0}; //contains length field
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					4'd4:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,96'b0, ptp_seq, 16'b0};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					4'd5:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,32'b0,time_stamp_rec,48'b0};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					//beacon field
					4'd6:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,direct_mac_addr, direction, 15'b0, token_bucket_depth,token_bucket_para,time_slot_period};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					//ESW module
					4'd7:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,esw_pktin_cnt, esw_pktout_cnt};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					4'd8:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,in_local_mac_id[7:0], bufm_id_cnt, 112'b0};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					//EOS module
					4'd9:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,eos_mdin_cnt, eos_mdout_cnt};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					4'd10:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,eos_q0_used_cnt, eos_q1_used_cnt, eos_q2_used_cnt, eos_q3_used_cnt, 96'b0};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
					end
					//GOE module
					4'd11:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b11,4'b0,goe_pktin_cnt,goe_port0out_cnt};
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;
						
					end
					4'd12:begin
						out_lr_data_wr <= 1'b1;
						out_lr_data <= {2'b10,4'b0,goe_port1out_cnt, goe_discard_cnt};
						out_lr_data_valid <= 1'b1;
						out_lr_data_valid_wr <= 1'b1;

						ptp_seq <= ptp_seq + 16'b1;

						//report_flag_slave <= report_flag_master;
						
					end       
					4'd13:begin
						out_lr_data_wr <= 1'b0;
						out_lr_data <= 134'b0;
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;

						
					end  
					4'd14:begin
						out_lr_data_wr <= 1'b0;
						out_lr_data <= 134'b0;
						out_lr_data_valid <= 1'b0;
						out_lr_data_valid_wr <= 1'b0;

						report_flag_slave <= report_flag_master;
						pktin_ready <= 1'b1;
						lreport_state <= IDLE_S;
						
					end                                  
				endcase
			end
		endcase
	end
end

//=========================== beacon report ========================//

always @(posedge clk or negedge rst_n) begin
	if (!rst_n) begin
		// reset
		report_flag_master <= 1'b0;
		time_stamp_rec  <= 48'h0;
	end
	else begin
		if(precision_time[21:0] == 22'hff) begin
		    time_stamp_rec <= precision_time;
			report_flag_master <= ~report_flag_master;
		end
		else begin
		    time_stamp_rec <= time_stamp_rec;
			report_flag_master <= report_flag_master;
		end
	end
end

endmodule