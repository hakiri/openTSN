/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: ARM.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// ARM:Report cmd array and state array.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/08/10
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module ARM#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input				clk,
input				rst_n,

input               cnt_rst,

input               in_arm_test_start,

//receive from PHU
input	      [7:0]  in_arm_pkt_hdr_seq,
//receive from LAU
input	      [3:0]  in_arm_gcl_array_seq,

//receive from PGM
input	      [11:0] in_arm_pkt_1_len,
input	      [11:0] in_arm_pkt_2_len,
input	      [11:0] in_arm_pkt_3_len,
input	      [11:0] in_arm_pkt_4_len,
input	      [11:0] in_arm_pkt_5_len,
input	      [11:0] in_arm_pkt_6_len,
input	      [11:0] in_arm_pkt_7_len,
input	      [11:0] in_arm_pkt_8_len,

input	 	  [15:0] in_arm_tb_1_size, 
input	      [15:0] in_arm_tb_1_rate,
input	 	  [15:0] in_arm_tb_2_size, 
input	      [15:0] in_arm_tb_2_rate,
input	 	  [15:0] in_arm_tb_3_size, 
input	      [15:0] in_arm_tb_3_rate,
input	 	  [15:0] in_arm_tb_4_size, 
input	      [15:0] in_arm_tb_4_rate,
input	 	  [15:0] in_arm_tb_5_size, 
input	      [15:0] in_arm_tb_5_rate,
input	 	  [15:0] in_arm_tb_6_size, 
input	      [15:0] in_arm_tb_6_rate,
input	 	  [15:0] in_arm_tb_7_size, 
input	      [15:0] in_arm_tb_7_rate,
input	 	  [15:0] in_arm_tb_8_size, 
input	      [15:0] in_arm_tb_8_rate,

//receive from FSM
input	      [103:0]in_arm_rule_5tuple_1,
input	      [103:0]in_arm_mask_1,
input	      [103:0]in_arm_rule_5tuple_2,
input	      [103:0]in_arm_mask_2,
input	      [103:0]in_arm_rule_5tuple_3,
input	      [103:0]in_arm_mask_3,
input	      [103:0]in_arm_rule_5tuple_4,
input	      [103:0]in_arm_mask_4,
input	      [103:0]in_arm_rule_5tuple_5,
input	      [103:0]in_arm_mask_5,
input	      [103:0]in_arm_rule_5tuple_6,
input	      [103:0]in_arm_mask_6,
input	      [103:0]in_arm_rule_5tuple_7,
input	      [103:0]in_arm_mask_7,
input	      [103:0]in_arm_rule_5tuple_8,
input	      [103:0]in_arm_mask_8,
//receive from SSM
input	      [15:0] in_arm_samp_freq,

//receive from PGM
input	      [31:0] in_pgm2arm_pkt_1_cnt,
input	      [31:0] in_pgm2arm_pkt_2_cnt,
input	      [31:0] in_pgm2arm_pkt_3_cnt,
input	      [31:0] in_pgm2arm_pkt_4_cnt,
input	      [31:0] in_pgm2arm_pkt_5_cnt,
input	      [31:0] in_pgm2arm_pkt_6_cnt,
input	      [31:0] in_pgm2arm_pkt_7_cnt,
input	      [31:0] in_pgm2arm_pkt_8_cnt,
//receive from FSM
input	      [31:0] in_fsm2arm_pkt_1_cnt,
input	      [31:0] in_fsm2arm_pkt_2_cnt,
input	      [31:0] in_fsm2arm_pkt_3_cnt,
input	      [31:0] in_fsm2arm_pkt_4_cnt,
input	      [31:0] in_fsm2arm_pkt_5_cnt,
input	      [31:0] in_fsm2arm_pkt_6_cnt,
input	      [31:0] in_fsm2arm_pkt_7_cnt,
input	      [31:0] in_fsm2arm_pkt_8_cnt,
//receive from SSM
input	      [31:0] in_ssm2arm_pkt_cnt,
//receive from top module
input	             in_arm_test_stop,
input	      [19:0] in_arm_gcl_time_slot_cycle,
input	      [47:0] timestamp,

//transmit to FPGA OS
output	reg       [133:0] out_arm_data,
output	reg               out_arm_data_wr,
output	reg               out_arm_data_valid,
output	reg               out_arm_data_valid_wr
);

//*******************************************************************
// Report pkt_hdr_sequence, GCL_sequence, cmd array and state array
//*******************************************************************
(*mark_debug="TRUE"*)reg [3:0] arm_state;
reg [3:0] reg_pgm_ca_cnt;
reg [4:0] reg_fsm_ca_cnt;
reg [3:0] reg_ssm_ca_cnt;
reg [3:0] reg_pgm_sa_cnt;
reg [4:0] reg_fsm_sa_cnt;

reg       report_period_flag;

localparam IDLE_S = 4'd0,
           GENERATE_MD1_S = 4'd1,
		   GENERATE_ETH_HDR_S = 4'd2,
		   GENERATE_ENCAP_MD0_S = 4'd3,
           GENERATE_ENCAP_MD1_S = 4'd4,
		   GENERATE_PGM_CA_S = 4'd5,
		   GENERATE_FSM_CA_S = 4'd6,
		   GENERATE_SSM_CA_S = 4'd7,
		   GENERATE_PGM_SA_S = 4'd8,
		   GENERATE_FSM_SA_S = 4'd9,
		   GENERATE_SSM_SA_S = 4'd10;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_arm_data <= 134'b0;
		out_arm_data_wr <= 1'b0;
		out_arm_data_valid <= 1'b0;
        out_arm_data_valid_wr <= 1'b0;
        
        reg_pgm_ca_cnt <= 4'd0;
        reg_fsm_ca_cnt <= 5'd0;
        reg_ssm_ca_cnt <= 4'd0;
        reg_pgm_sa_cnt <= 4'd0;
        reg_fsm_sa_cnt <= 5'd0;

		arm_state <= IDLE_S;
    end
    else begin
	    case(arm_state)
            IDLE_S:begin
                reg_pgm_ca_cnt <= 4'd0;
                reg_fsm_ca_cnt <= 5'd0;
                reg_ssm_ca_cnt <= 4'd0;
                reg_pgm_sa_cnt <= 4'd0;
                reg_fsm_sa_cnt <= 5'd0;
                
		        out_arm_data_valid <= 1'b0;
                out_arm_data_valid_wr <= 1'b0;

   			    if(report_period_flag == 1'b1)begin  
	                out_arm_data <= {2'b01,4'd0,20'd0,12'd592,48'd0,timestamp};    //len:37 cycles*16B/cycle
		            out_arm_data_wr <= 1'b1;			    
				    arm_state <= GENERATE_MD1_S;   
				end
				else begin
                    out_arm_data <= 134'b0;
		            out_arm_data_wr <= 1'b0;
				    arm_state <= IDLE_S; 
				end
			end
			GENERATE_MD1_S:begin   //metadata1
	            out_arm_data <= {2'b11,4'd0,128'd0};
		        out_arm_data_wr <= 1'b1;
				arm_state <= GENERATE_ETH_HDR_S;  
		    end
			GENERATE_ETH_HDR_S:begin   //ethernet header
	            out_arm_data <= {2'b11,4'd0,48'hffff_ffff_ffff,48'h0,16'hff01,4'h3,in_arm_gcl_array_seq,in_arm_pkt_hdr_seq};
		        out_arm_data_wr <= 1'b1;
				arm_state <= GENERATE_ENCAP_MD0_S;  
		    end			
			GENERATE_ENCAP_MD0_S:begin   //encapsulated metadata0
	            out_arm_data <= {2'b11,4'd0,20'd0,12'd544,48'd0,timestamp};
		        out_arm_data_wr <= 1'b1;
				arm_state <= GENERATE_ENCAP_MD1_S;  
		    end			
			GENERATE_ENCAP_MD1_S:begin   //encapsulated metadata1
	            out_arm_data <= {2'b11,4'd0,128'd0};
		        out_arm_data_wr <= 1'b1;
				arm_state <= GENERATE_PGM_CA_S;  
		    end			
			GENERATE_PGM_CA_S:begin    //6 cycles 
			    out_arm_data_wr <= 1'b1;
				reg_pgm_ca_cnt <= reg_pgm_ca_cnt + 4'd1;
			    case(reg_pgm_ca_cnt)
		            4'd0:begin
	                    out_arm_data <= {2'b11,4'd0,95'b0,in_arm_test_stop,12'b0,in_arm_gcl_time_slot_cycle};
						arm_state <= GENERATE_PGM_CA_S;
					end
		            4'd1:begin
	                    out_arm_data <= {2'b11,4'b0,32'b0,in_arm_tb_3_size,in_arm_tb_3_rate,in_arm_tb_2_size,in_arm_tb_2_rate,in_arm_tb_1_size,in_arm_tb_1_rate};
                        arm_state <= GENERATE_PGM_CA_S;						
					end
		            4'd2:begin
	                    out_arm_data <= {2'b11,4'b0,32'b0,in_arm_tb_6_size,in_arm_tb_6_rate,in_arm_tb_5_size,in_arm_tb_5_rate,in_arm_tb_4_size,in_arm_tb_4_rate};
                        arm_state <= GENERATE_PGM_CA_S;						
					end
		            4'd3:begin
	                    out_arm_data <= {2'b11,4'b0,64'b0,in_arm_tb_8_size,in_arm_tb_8_rate,in_arm_tb_7_size,in_arm_tb_7_rate};
                        arm_state <= GENERATE_PGM_CA_S;						
					end	
		            4'd4:begin
	                    out_arm_data <= {2'b11,4'b0,4'b0,in_arm_pkt_8_len,4'b0,in_arm_pkt_7_len,4'b0,in_arm_pkt_6_len,4'b0,in_arm_pkt_5_len,4'b0,in_arm_pkt_4_len,4'b0,in_arm_pkt_3_len,4'b0,in_arm_pkt_2_len,4'b0,in_arm_pkt_1_len};
                        arm_state <= GENERATE_PGM_CA_S;						
					end	
		            4'd5:begin
	                    out_arm_data <= {2'b11,4'b0,128'b0};
                        arm_state <= GENERATE_FSM_CA_S;						
					end						
		        endcase 
		    end				
			GENERATE_FSM_CA_S:begin   //17 cycles
			    out_arm_data_wr <= 1'b1;
				reg_fsm_ca_cnt <= reg_fsm_ca_cnt + 5'd1;
			    case(reg_fsm_ca_cnt)
		            5'd0:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_1};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd1:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_1};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd2:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_2};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd3:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_2};
                        arm_state <= GENERATE_FSM_CA_S;						
					end					
		            5'd4:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_3};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd5:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_3};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd6:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_4};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd7:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_4};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd8:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_5};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd9:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_5};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd10:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_6};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd11:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_6};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd12:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_7};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd13:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_7};
                        arm_state <= GENERATE_FSM_CA_S;						
					end
		            5'd14:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_rule_5tuple_8};
						arm_state <= GENERATE_FSM_CA_S;
					end
		            5'd15:begin
	                    out_arm_data <= {2'b11,4'd0,24'b0,in_arm_mask_8};
                        arm_state <= GENERATE_FSM_CA_S;						
					end					
		            5'd16:begin
	                    out_arm_data <= {2'b11,4'b0,128'b0};
                        arm_state <= GENERATE_SSM_CA_S;						
					end						
		        endcase 
		    end	
			GENERATE_SSM_CA_S:begin   //2 cycles
			    out_arm_data_wr <= 1'b1;
				reg_ssm_ca_cnt <= reg_ssm_ca_cnt + 5'd1;
			    case(reg_ssm_ca_cnt)
		            4'd0:begin
	                    out_arm_data <= {2'b11,4'd0,112'b0,in_arm_samp_freq};
						arm_state <= GENERATE_SSM_CA_S;
					end
		            4'd1:begin
	                    out_arm_data <= {2'b11,4'd0,128'b0};
                        arm_state <= GENERATE_PGM_SA_S;						
					end					
		        endcase 
		    end	
			GENERATE_PGM_SA_S:begin   //3 cycles
			    out_arm_data_wr <= 1'b1;
				reg_pgm_sa_cnt <= reg_pgm_sa_cnt + 4'd1;
			    case(reg_pgm_sa_cnt)
		            4'd0:begin
	                    out_arm_data <= {2'b11,4'd0,in_pgm2arm_pkt_4_cnt,in_pgm2arm_pkt_3_cnt,in_pgm2arm_pkt_2_cnt,in_pgm2arm_pkt_1_cnt};
						arm_state <= GENERATE_PGM_SA_S;
					end
		            4'd1:begin
	                    out_arm_data <= {2'b11,4'd0,in_pgm2arm_pkt_8_cnt,in_pgm2arm_pkt_7_cnt,in_pgm2arm_pkt_6_cnt,in_pgm2arm_pkt_5_cnt};
						arm_state <= GENERATE_PGM_SA_S;
					end	
		            4'd2:begin
	                    out_arm_data <= {2'b11,4'd0,128'b0};
						arm_state <= GENERATE_FSM_SA_S;
					end						
		        endcase 
		    end	
			GENERATE_FSM_SA_S:begin   //3 cycles
			    out_arm_data_wr <= 1'b1;
				reg_fsm_sa_cnt <= reg_fsm_sa_cnt + 4'd1;
			    case(reg_fsm_sa_cnt)
		            4'd0:begin
	                    out_arm_data <= {2'b11,4'd0,in_fsm2arm_pkt_4_cnt,in_fsm2arm_pkt_3_cnt,in_fsm2arm_pkt_2_cnt,in_fsm2arm_pkt_1_cnt};
						arm_state <= GENERATE_FSM_SA_S;
					end
		            4'd1:begin
	                    out_arm_data <= {2'b11,4'd0,in_fsm2arm_pkt_8_cnt,in_fsm2arm_pkt_7_cnt,in_fsm2arm_pkt_6_cnt,in_fsm2arm_pkt_5_cnt};
						arm_state <= GENERATE_FSM_SA_S;
					end	
		            4'd2:begin
	                    out_arm_data <= {2'b11,4'd0,128'b0};
						arm_state <= GENERATE_SSM_SA_S;
					end						
		        endcase 
		    end	
			GENERATE_SSM_SA_S:begin   
			    out_arm_data_wr <= 1'b1;
	            out_arm_data <= {2'b10,4'd0,96'b0,in_ssm2arm_pkt_cnt};
		        out_arm_data_valid <= 1'b1;
                out_arm_data_valid_wr <= 1'b1;
		        arm_state <= IDLE_S;
		    end				
	    endcase
    end     
end 

//*******************************************************************
//                     report_time_cnt
//*******************************************************************
reg        [31:0] report_time_cnt;  //max time is 34s.
always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
		report_time_cnt <= 32'd0;
	end
	else begin
		if(cnt_rst == 1'b1)begin
		    report_time_cnt <= 32'd0;
		end
	    else if(in_arm_test_start == 1'b1 || in_arm_test_stop == 1'b1)begin
	        if(report_time_cnt == 32'd124_999_999)begin
		        report_time_cnt <= 32'd0;
		    end
		    else begin
		        report_time_cnt <= report_time_cnt + 32'd1;
		    end
		end
		else begin
		    report_time_cnt <= report_time_cnt;
		end
	end
end

//*******************************************************************
//                     time_cnt of report period
//*******************************************************************
reg report_stop_flag;
always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
		report_period_flag <= 1'b0;
		report_stop_flag <= 1'b0;
	end
	else begin
        if(report_time_cnt == 32'd124_999_999)begin  //32'd125_000_000 is 1s.
		    report_period_flag <= 1'b1;
		end
/*		else if(report_stop_flag == 1'b0 && report_stop_time_cnt[31:0] == 32'd624_999_999)begin  //32'd625_000_000 is 5s.
		    report_period_flag <= 1'b1;
		    report_stop_flag <= 1'b1;
		end
*/
		else begin
		    report_period_flag <= 1'b0;
		end
	end
end
endmodule 
 
