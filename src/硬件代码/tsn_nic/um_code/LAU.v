/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: LAU.v
//Version: 1.0
//date: 2019/08/10
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// LAU(List and Array Update):Update gate control list and cmd array.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module LAU#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input				clk,
input				rst_n,

//receive from FPGA OS
input	     [133:0]in_lau_data,
input	            in_lau_data_wr,

//transmit to GCL_RAM
output	reg         out_lau_gc_wr,
output	reg  [4:0]  out_lau_gc_addr,
output	reg  [127:0]out_lau_gc,

//transmit to ARM
output	reg  [3:0]  out_lau_gcl_array_seq,

//transmit to PGM
output	reg         out_lau_test_stop,
output	reg  [19:0] out_lau_gcl_time_slot_cycle,
output	reg  [11:0] out_lau_pkt_1_len,
output	reg  [11:0] out_lau_pkt_2_len,
output	reg  [11:0] out_lau_pkt_3_len,
output	reg  [11:0] out_lau_pkt_4_len,
output	reg  [11:0] out_lau_pkt_5_len,
output	reg  [11:0] out_lau_pkt_6_len,
output	reg  [11:0] out_lau_pkt_7_len,
output	reg  [11:0] out_lau_pkt_8_len,

output	reg  [15:0] out_lau_tb_1_size, 
output	reg  [15:0] out_lau_tb_1_rate,
output	reg  [15:0] out_lau_tb_2_size, 
output	reg  [15:0] out_lau_tb_2_rate,
output	reg  [15:0] out_lau_tb_3_size, 
output	reg  [15:0] out_lau_tb_3_rate,
output	reg  [15:0] out_lau_tb_4_size, 
output	reg  [15:0] out_lau_tb_4_rate,
output	reg  [15:0] out_lau_tb_5_size, 
output	reg  [15:0] out_lau_tb_5_rate,
output	reg  [15:0] out_lau_tb_6_size, 
output	reg  [15:0] out_lau_tb_6_rate,
output	reg  [15:0] out_lau_tb_7_size, 
output	reg  [15:0] out_lau_tb_7_rate,
output	reg  [15:0] out_lau_tb_8_size, 
output	reg  [15:0] out_lau_tb_8_rate,

//transmit to FSM
output	reg  [103:0]out_lau_rule_5tuple_1,
output	reg  [103:0]out_lau_mask_1,
output	reg  [103:0]out_lau_rule_5tuple_2,
output	reg  [103:0]out_lau_mask_2,
output	reg  [103:0]out_lau_rule_5tuple_3,
output	reg  [103:0]out_lau_mask_3,
output	reg  [103:0]out_lau_rule_5tuple_4,
output	reg  [103:0]out_lau_mask_4,
output	reg  [103:0]out_lau_rule_5tuple_5,
output	reg  [103:0]out_lau_mask_5,
output	reg  [103:0]out_lau_rule_5tuple_6,
output	reg  [103:0]out_lau_mask_6,
output	reg  [103:0]out_lau_rule_5tuple_7,
output	reg  [103:0]out_lau_mask_7,
output	reg  [103:0]out_lau_rule_5tuple_8,
output	reg  [103:0]out_lau_mask_8,

//transmit to SSM
output	reg   [15:0] out_lau_samp_freq,

//transmit to LCM
output	reg          out_lau_update_finish
);

//************************************************************
//               Update GC list and cmd array
//************************************************************
reg  test_stop_flag;
reg [4:0] reg_gcl_cnt;
reg [3:0] reg_pgm_cnt;
reg [4:0] reg_fsm_cnt;
(*mark_debug="TRUE"*)reg [3:0] lau_state;
localparam IDLE_S = 4'd0,
           WAIT1_S = 4'd1,
		   JUDGE_TYPE_S = 4'd2,
		   WAIT2_S = 4'd3,
           WAIT3_S = 4'd4,
		   READ_GCL_S = 4'd5,
		   READ_PGM_S = 4'd6,
		   READ_FSM_S = 4'd7,
		   READ_SSM_S = 4'd8;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_lau_test_stop <= 1'b1;
        test_stop_flag <= 1'b1;
        
        out_lau_gc_wr <= 1'b0;
		out_lau_gc_addr <= 5'b0;
		out_lau_gc <= 128'b0;
        out_lau_gcl_array_seq <= 4'b0;
		out_lau_update_finish <= 1'b0;
		
        out_lau_pkt_1_len <= 12'd0;
        out_lau_pkt_2_len <= 12'd0;	
        out_lau_pkt_3_len <= 12'd0;	
        out_lau_pkt_4_len <= 12'd0;	
        out_lau_pkt_5_len <= 12'd0;	
        out_lau_pkt_6_len <= 12'd0;	
        out_lau_pkt_7_len <= 12'd0;	
        out_lau_pkt_8_len <= 12'd0;	
		
		out_lau_tb_1_size <= 16'h7fff;
		out_lau_tb_1_rate <= 16'd0;
		out_lau_tb_2_size <= 16'h7fff;
		out_lau_tb_2_rate <= 16'd0;
		out_lau_tb_3_size <= 16'h7fff;
		out_lau_tb_3_rate <= 16'd0;
		out_lau_tb_4_size <= 16'h7fff;
		out_lau_tb_4_rate <= 16'd0;
		out_lau_tb_5_size <= 16'h7fff;
		out_lau_tb_5_rate <= 16'd0;
		out_lau_tb_6_size <= 16'h7fff;
		out_lau_tb_6_rate <= 16'd0;
		out_lau_tb_7_size <= 16'h7fff;
		out_lau_tb_7_rate <= 16'd0;
		out_lau_tb_8_size <= 16'h7fff;
		out_lau_tb_8_rate <= 16'd0;
		
		out_lau_rule_5tuple_1 <= 104'h0;  
		out_lau_mask_1 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;          
		out_lau_rule_5tuple_2 <= 104'h0;   
		out_lau_mask_2 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;             
		out_lau_rule_5tuple_3 <= 104'h0;   
		out_lau_mask_3 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;        
		out_lau_rule_5tuple_4 <= 104'h0;  
		out_lau_mask_4 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;         
		out_lau_rule_5tuple_5 <= 104'h0;   
		out_lau_mask_5 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;          
		out_lau_rule_5tuple_6 <= 104'h0;   
		out_lau_mask_6 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;             
		out_lau_rule_5tuple_7 <= 104'h0;   
		out_lau_mask_7 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;           
		out_lau_rule_5tuple_8 <= 104'h0;   
		out_lau_mask_8 <= 104'hffff_ffff_ffff_ffff_ffff_ffff_ff;       
		
		out_lau_samp_freq <=  16'h1;   //不能置为16'h0;否则在Beacon配置之前在ssm_ sample模块reg_pkt_num==sample_freq-16'd1会出现 16'd0-16'd1，导致 reg_pkt_num计数非常大。      
		
		reg_gcl_cnt <= 5'b0;
		reg_pgm_cnt <= 4'b0;
		reg_fsm_cnt <= 5'b0;
		
		lau_state <= IDLE_S;
    end
    else begin
	    case(lau_state)
            IDLE_S:begin
                out_lau_gc_wr <= 1'b0;
		        out_lau_gc_addr <= 5'b0;
		        out_lau_gc <= 128'b0;

		        reg_gcl_cnt <= 5'b0;
		        reg_pgm_cnt <= 4'b0;
		        reg_fsm_cnt <= 5'b0;

   			    if(in_lau_data_wr == 1'b1 && in_lau_data[133:132] == 2'b01)begin  //Update GC list and cmd array
				    lau_state <= WAIT1_S;   
				end
				else begin
				    lau_state <= IDLE_S; 
				end
			end
			WAIT1_S:begin   //metadata1
				lau_state <= JUDGE_TYPE_S;  
		    end
			JUDGE_TYPE_S:begin
                if(in_lau_data[31:16] == 16'hff01 && in_lau_data[15:12] == 4'h2)begin
                    out_lau_gcl_array_seq <= in_lau_data[11:8];				
				    lau_state <= WAIT2_S; 
                end
                else begin
                    lau_state <= IDLE_S; 
                end				
		    end
			WAIT2_S:begin   //encapsulated metadata0
				lau_state <= WAIT3_S;  
		    end		
			WAIT3_S:begin   //encapsulated metadata1
				lau_state <= READ_GCL_S;  
		    end				
			READ_GCL_S:begin              //32 cycles 
                reg_gcl_cnt <= reg_gcl_cnt + 5'd1;
                out_lau_gc_wr <= 1'b1;
                out_lau_gc <= in_lau_data[127:0];
                if(reg_gcl_cnt == 5'd0)begin
		            out_lau_gc_addr <= 5'd0;
                    lau_state <= READ_GCL_S;                  					
				end
				else begin
		            out_lau_gc_addr <= out_lau_gc_addr + 5'd1;			
			        if(reg_gcl_cnt == 5'd31)begin	        
                        lau_state <= READ_PGM_S; 					
				    end
				    else begin    
                        lau_state <= READ_GCL_S; 					
				    end
				end
		    end
			READ_PGM_S:begin     //6 cycles 
                out_lau_gc_wr <= 1'b0;
                out_lau_gc <= 128'b0;
                out_lau_gc_addr <= 5'd0;
                
                reg_pgm_cnt <= reg_pgm_cnt + 4'd1;
		        case(reg_pgm_cnt)
		            4'd0:begin
					    test_stop_flag <= in_lau_data[32];
						out_lau_gcl_time_slot_cycle <= in_lau_data[19:0];
						lau_state <= READ_PGM_S;
					end
		            4'd1:begin
					    out_lau_tb_1_rate <= in_lau_data[15:0];
						out_lau_tb_1_size <= in_lau_data[31:16];

					    out_lau_tb_2_rate <= in_lau_data[47:32];
						out_lau_tb_2_size <= in_lau_data[63:48];
						
					    out_lau_tb_3_rate <= in_lau_data[79:64];
						out_lau_tb_3_size <= in_lau_data[95:80];
                        lau_state <= READ_PGM_S;						
					end
		            4'd2:begin
					    out_lau_tb_4_rate <= in_lau_data[15:0];
						out_lau_tb_4_size <= in_lau_data[31:16];

					    out_lau_tb_5_rate <= in_lau_data[47:32];
						out_lau_tb_5_size <= in_lau_data[63:48];
						
					    out_lau_tb_6_rate <= in_lau_data[79:64];
						out_lau_tb_6_size <= in_lau_data[95:80];
                        lau_state <= READ_PGM_S;						
					end
		            4'd3:begin
					    out_lau_tb_7_rate <= in_lau_data[15:0];
						out_lau_tb_7_size <= in_lau_data[31:16];

					    out_lau_tb_8_rate <= in_lau_data[47:32];
						out_lau_tb_8_size <= in_lau_data[63:48];
                        lau_state <= READ_PGM_S;						
					end
		            4'd4:begin //pkt length of 12bit; max pkt length is 1518B.
		            //pkt_1_len
		                if(in_lau_data[11:0]>12'd1514)begin
			                out_lau_pkt_1_len <= 12'd1514;
			            end
			            else if(in_lau_data[11:0]<12'd60)begin
			                out_lau_pkt_1_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_1_len <= in_lau_data[11:0];
			            end
		            //pkt_2_len
		                if(in_lau_data[27:16]>12'd1514)begin
			                out_lau_pkt_2_len <= 12'd1514;
			            end
			            else if(in_lau_data[27:16]<12'd60)begin
			                out_lau_pkt_2_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_2_len <= in_lau_data[27:16];
			            end			            
		            //pkt_3_len
		                if(in_lau_data[43:32]>12'd1514)begin
			                out_lau_pkt_3_len <= 12'd1514;
			            end
			            else if(in_lau_data[43:32]<12'd60)begin
			                out_lau_pkt_3_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_3_len <= in_lau_data[43:32];
			            end				            
		            //pkt_4_len
		                if(in_lau_data[59:48]>12'd1514)begin
			                out_lau_pkt_4_len <= 12'd1514;
			            end
			            else if(in_lau_data[59:48]<12'd60)begin
			                out_lau_pkt_4_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_4_len <= in_lau_data[59:48];
			            end		
		            //pkt_5_len
		                if(in_lau_data[75:64]>12'd1514)begin
			                out_lau_pkt_5_len <= 12'd1514;
			            end
			            else if(in_lau_data[75:64]<12'd60)begin
			                out_lau_pkt_5_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_5_len <= in_lau_data[75:64];
			            end	
		            //pkt_6_len
		                if(in_lau_data[91:80]>12'd1514)begin
			                out_lau_pkt_6_len <= 12'd1514;
			            end
			            else if(in_lau_data[91:80]<12'd60)begin
			                out_lau_pkt_6_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_6_len <= in_lau_data[91:80];
			            end	
		            //pkt_7_len
		                if(in_lau_data[107:96]>12'd1514)begin
			                out_lau_pkt_7_len <= 12'd1514;
			            end
			            else if(in_lau_data[107:96]<12'd60)begin
			                out_lau_pkt_7_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_7_len <= in_lau_data[107:96];
			            end	
		            //pkt_8_len
		                if(in_lau_data[123:112]>12'd1514)begin
			                out_lau_pkt_8_len <= 12'd1514;
			            end
			            else if(in_lau_data[123:112]<12'd60)begin
			                out_lau_pkt_8_len <= 12'd60;
			            end
			            else begin
			                out_lau_pkt_8_len <= in_lau_data[123:112];
			            end				
					end
		            4'd5:begin
			            lau_state <= READ_FSM_S;	
					end
		        endcase 
		    end
			READ_FSM_S:begin     //17 cycles 
                reg_fsm_cnt <= reg_fsm_cnt + 5'd1;
		        case(reg_fsm_cnt)
		            5'd0:begin
					    out_lau_rule_5tuple_1 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd1:begin
						out_lau_mask_1 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd2:begin
					    out_lau_rule_5tuple_2 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd3:begin
						out_lau_mask_2 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end	
		            5'd4:begin
					    out_lau_rule_5tuple_3 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd5:begin
						out_lau_mask_3 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd6:begin
					    out_lau_rule_5tuple_4 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd7:begin
						out_lau_mask_4 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd8:begin
					    out_lau_rule_5tuple_5 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd9:begin
						out_lau_mask_5 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd10:begin
					    out_lau_rule_5tuple_6 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd11:begin
						out_lau_mask_6 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd12:begin
					    out_lau_rule_5tuple_7 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd13:begin
						out_lau_mask_7 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end
		            5'd14:begin
					    out_lau_rule_5tuple_8 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;
					end
		            5'd15:begin
						out_lau_mask_8 <= in_lau_data[103:0];
						lau_state <= READ_FSM_S;						
					end					
		            5'd16:begin
						lau_state <= READ_SSM_S;						
					end					
		        endcase 
		    end
			READ_SSM_S:begin
				out_lau_samp_freq <= in_lau_data[15:0];  
				out_lau_test_stop <=  test_stop_flag;
				out_lau_update_finish <= ~test_stop_flag;
                lau_state <= IDLE_S;			
		    end
	    endcase
    end     
end 
endmodule 
