/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: PHE.v
//Version: 1.0
//date: 2019/08/09
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// PHE(PKT header Extension):Extend the PKT header.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date: 
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module PHE#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input		clk,
input		rst_n,

input       cnt_rst,

//receive from LCM
input    in_phe_test_start,
input    [47:0] timestamp,
input    [3:0]  slot_shift_cnt, 
(*mark_debug="TRUE"*)input    [8:0]  slot_ID,
input    [11:0] in_phe_pkt_1_len,
input    [11:0] in_phe_pkt_2_len,
input    [11:0] in_phe_pkt_3_len,
input    [11:0] in_phe_pkt_4_len,
input    [11:0] in_phe_pkt_5_len,
input    [11:0] in_phe_pkt_6_len,
input    [11:0] in_phe_pkt_7_len,
input    [11:0] in_phe_pkt_8_len,

//transmit to LCM
output	reg  [31:0] out_phe_pkt_1_cnt,
output	reg  [31:0] out_phe_pkt_2_cnt,
output	reg  [31:0] out_phe_pkt_3_cnt,
output	reg  [31:0] out_phe_pkt_4_cnt,
output	reg  [31:0] out_phe_pkt_5_cnt,
output	reg  [31:0] out_phe_pkt_6_cnt,
output	reg  [31:0] out_phe_pkt_7_cnt,
output	reg  [31:0] out_phe_pkt_8_cnt,

//receive from TSM
input   [7:0]  in_phe_selected,
//output	wire            out_phe_outport_free,

//receive from DRM
input    [127:0]in_phe_pkt_hdr,
input           in_phe_pkt_hdr_wr,

//transmit to FPGA OS
output	reg  [133:0]out_phe_data,
output	reg         out_phe_data_wr,
output	reg         out_phe_data_valid,
output	reg         out_phe_data_valid_wr
);
//************************************************************
//               register 2 PKT headers
//************************************************************
//delay regs, we need to delay for 2 cycles to generate 2 metadata.
reg [127:0] reg_ph_data_1;
reg reg_ph_data_1_wr;

reg [127:0] reg_ph_data_2;
reg reg_ph_data_2_wr;

always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        reg_ph_data_1 <= 128'b0;
		reg_ph_data_1_wr <= 1'b0;
		
        reg_ph_data_2 <= 128'b0;
		reg_ph_data_2_wr <= 1'b0;
    end
    else begin
        reg_ph_data_1 <= in_phe_pkt_hdr;
		reg_ph_data_1_wr <= in_phe_pkt_hdr_wr;
		
        reg_ph_data_2 <= reg_ph_data_1;
		reg_ph_data_2_wr <= reg_ph_data_1_wr;
	end
end

//************************************************************
//                selected PKT_len
//************************************************************
reg [11:0] reg_selected_pkt_len;
reg [31:0] reg_selected_pkt_seq;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        reg_selected_pkt_len <= 12'b0;
        reg_selected_pkt_seq <= 32'd0;
        
        out_phe_pkt_1_cnt <= 32'd0; 
        out_phe_pkt_2_cnt <= 32'd0; 
        out_phe_pkt_3_cnt <= 32'd0; 
        out_phe_pkt_4_cnt <= 32'd0; 
        out_phe_pkt_5_cnt <= 32'd0; 
        out_phe_pkt_6_cnt <= 32'd0; 
        out_phe_pkt_7_cnt <= 32'd0; 
        out_phe_pkt_8_cnt <= 32'd0; 
    end
	else begin
	    if(cnt_rst == 1'b0)begin
		    case(in_phe_selected)
		        8'b0000_0001:begin
		            reg_selected_pkt_len <= in_phe_pkt_1_len;
		            out_phe_pkt_1_cnt <= out_phe_pkt_1_cnt + 32'd1; 
		            reg_selected_pkt_seq <= out_phe_pkt_1_cnt + 32'd1; 
		        end
		        8'b0000_0010:begin 
		            reg_selected_pkt_len <= in_phe_pkt_2_len;
		            out_phe_pkt_2_cnt <= out_phe_pkt_2_cnt + 32'd1; 
		            reg_selected_pkt_seq <= out_phe_pkt_2_cnt + 32'd1; 
		        end
                8'b0000_0100:begin 
                    reg_selected_pkt_len <= in_phe_pkt_3_len;
                    out_phe_pkt_3_cnt <= out_phe_pkt_3_cnt + 32'd1; 
                    reg_selected_pkt_seq <= out_phe_pkt_3_cnt + 32'd1; 
                end
                8'b0000_1000:begin 
                    reg_selected_pkt_len <= in_phe_pkt_4_len;
                    out_phe_pkt_4_cnt <= out_phe_pkt_4_cnt + 32'd1;
                    reg_selected_pkt_seq <= out_phe_pkt_4_cnt + 32'd1;  
                end
		        8'b0001_0000:begin 
		            reg_selected_pkt_len <= in_phe_pkt_5_len;
		            out_phe_pkt_5_cnt <= out_phe_pkt_5_cnt + 32'd1; 
		            reg_selected_pkt_seq <= out_phe_pkt_5_cnt + 32'd1; 
		        end
		        8'b0010_0000:begin 
		            reg_selected_pkt_len <= in_phe_pkt_6_len;
		            out_phe_pkt_6_cnt <= out_phe_pkt_6_cnt + 32'd1; 
		            reg_selected_pkt_seq <= out_phe_pkt_6_cnt + 32'd1; 
		        end
		        8'b0100_0000:begin 
		            reg_selected_pkt_len <= in_phe_pkt_7_len;
		            out_phe_pkt_7_cnt <= out_phe_pkt_7_cnt + 32'd1;
		            reg_selected_pkt_seq <= out_phe_pkt_7_cnt + 32'd1; 
		        end 
		        8'b1000_0000:begin 
		            reg_selected_pkt_len <= in_phe_pkt_8_len;
		            out_phe_pkt_8_cnt <= out_phe_pkt_8_cnt + 32'd1; 
		            reg_selected_pkt_seq <= out_phe_pkt_8_cnt + 32'd1; 
		        end	
		        default:begin
		            reg_selected_pkt_len <= reg_selected_pkt_len;
		            out_phe_pkt_1_cnt <= out_phe_pkt_1_cnt;
		            out_phe_pkt_2_cnt <= out_phe_pkt_2_cnt;
		            out_phe_pkt_3_cnt <= out_phe_pkt_3_cnt;
		            out_phe_pkt_4_cnt <= out_phe_pkt_4_cnt;
		            out_phe_pkt_5_cnt <= out_phe_pkt_5_cnt;
		            out_phe_pkt_6_cnt <= out_phe_pkt_6_cnt;
		            out_phe_pkt_7_cnt <= out_phe_pkt_7_cnt;
		            out_phe_pkt_8_cnt <= out_phe_pkt_8_cnt;
		            reg_selected_pkt_seq <= reg_selected_pkt_seq; 
		        end	
		    endcase
		end
		else begin
            reg_selected_pkt_len <= 12'b0;
            reg_selected_pkt_seq <= 32'd0;
            
            out_phe_pkt_1_cnt <= 32'd0; 
            out_phe_pkt_2_cnt <= 32'd0; 
            out_phe_pkt_3_cnt <= 32'd0; 
            out_phe_pkt_4_cnt <= 32'd0; 
            out_phe_pkt_5_cnt <= 32'd0; 
            out_phe_pkt_6_cnt <= 32'd0; 
            out_phe_pkt_7_cnt <= 32'd0; 
            out_phe_pkt_8_cnt <= 32'd0; 	
		end
	end
end
//************************************************************
//               Extend the PKT header
//************************************************************
reg [11:0] reg_remain_pkt_len;
(*mark_debug="TRUE"*)reg [2:0] phe_state;
localparam IDLE_S = 3'd0,
           GENERATE_MD1_S = 3'd1,
		   MODIFY_DATA1_S = 3'd2,
		   MODIFY_DATA2_S = 3'd3,
           MODIFY_DATA3_S = 3'd4,
		   MODIFY_DATA4_S = 3'd5,
		   ADD_SEQ_S = 3'd6,
		   PKT_EXTENSION_S = 3'd7;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_phe_data <= 134'b0;
		out_phe_data_wr <= 1'b0;
		out_phe_data_valid <= 1'b0;
		out_phe_data_valid_wr <= 1'b0;
		reg_remain_pkt_len <= 12'd0;
		phe_state <= IDLE_S;
    end
    else begin
	    case(phe_state)
            IDLE_S:begin
		        out_phe_data_valid <= 1'b0;
		        out_phe_data_valid_wr <= 1'b0;
				reg_remain_pkt_len <= reg_selected_pkt_len;
			    if(in_phe_pkt_hdr_wr == 1'b1)begin
				    out_phe_data <= {2'b01,4'd0,20'd0,reg_selected_pkt_len + 12'd32,96'd0};
				    out_phe_data_wr <= 1'b1;
				    phe_state <= GENERATE_MD1_S;   
				end
				else begin
                    out_phe_data <= 134'b0;
		            out_phe_data_wr <= 1'b0;
				    phe_state <= IDLE_S; 
				end
			end
			GENERATE_MD1_S:begin
				out_phe_data <= {2'b11,4'd0,128'd0};
				out_phe_data_wr <= 1'b1;
				phe_state <= MODIFY_DATA1_S;  
		    end
			MODIFY_DATA1_S:begin
				out_phe_data <= {2'b11,4'd0,reg_ph_data_2};
				out_phe_data_wr <= 1'b1;
				reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;
				phe_state <= MODIFY_DATA2_S; 
		    end
			MODIFY_DATA2_S:begin
				out_phe_data <= {2'b11,4'd0,reg_ph_data_2};
				out_phe_data_wr <= 1'b1;
				reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;
				phe_state <= MODIFY_DATA3_S; 
		    end
			MODIFY_DATA3_S:begin
				out_phe_data <= {2'b11,4'd0,reg_ph_data_2};
				out_phe_data_wr <= 1'b1;
				reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;
				phe_state <= MODIFY_DATA4_S; 
		    end
/*			MODIFY_DATA4_S:begin
				out_phe_data[127:48] <= reg_ph_data_2[127:48];
			//    out_phe_data[127:104] <= {4'b0,slot_shift_cnt,7'b0,slot_ID};
		    //    out_phe_data[103:80] <= reg_ph_data_2[111:80];
		    //    out_phe_data[79:48] <= reg_selected_pkt_seq;
				out_phe_data[47:0] <= timestamp;         //sended timestamp
				out_phe_data_wr <= 1'b1;
				reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;
			    if(reg_remain_pkt_len == 12'd16)begin
			        out_phe_data[133:128] <= {2'b10,4'd0};
					out_phe_data_valid <= 1'b1;
		            out_phe_data_valid_wr <= 1'b1;					
				    phe_state <= IDLE_S; 
				end
				else begin
					out_phe_data[133:128] <= {2'b11,4'd0};
				    phe_state <= ADD_SEQ_S; 				
				end
		    end
*/		    
			MODIFY_DATA4_S:begin
				out_phe_data_wr <= 1'b1;
			    if(reg_remain_pkt_len <= 12'd16)begin
				    out_phe_data[133:48] <= {2'b10,4'd0 - reg_remain_pkt_len[3:0],reg_ph_data_2[127:48]};
				    out_phe_data[47:0] <= timestamp;         //sended timestamp
					out_phe_data_valid <= 1'b1;
		            out_phe_data_valid_wr <= 1'b1;					
				    phe_state <= IDLE_S; 
				end
				else begin
					out_phe_data[133:48] <= {2'b11,4'd0,reg_ph_data_2[127:48]};
				    out_phe_data[47:0] <= timestamp;         //sended timestamp
					reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;
				    phe_state <= ADD_SEQ_S; 				
				end
		    end
		    ADD_SEQ_S:begin
		        if(reg_remain_pkt_len <= 12'd16)begin
				    out_phe_data <= {2'b10,4'd0 - reg_remain_pkt_len[3:0],reg_selected_pkt_seq,76'd0,slot_shift_cnt,7'b0,slot_ID};
				    out_phe_data_wr <= 1'b1;
					out_phe_data_valid <= 1'b1;
		            out_phe_data_valid_wr <= 1'b1;
		            phe_state <= IDLE_S;
				end
				else begin
				    out_phe_data <= {2'b11,4'd0,reg_selected_pkt_seq,76'd0,slot_shift_cnt,7'b0,slot_ID};
				    out_phe_data_wr <= 1'b1;
                    reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;  
		            phe_state <= PKT_EXTENSION_S;				    
				end		    
		    end
            PKT_EXTENSION_S:begin
		        if(reg_remain_pkt_len <= 12'd16)begin
				    out_phe_data <= {2'b10,4'd0 - reg_remain_pkt_len[3:0],128'b0};
				    out_phe_data_wr <= 1'b1;
					out_phe_data_valid <= 1'b1;
		            out_phe_data_valid_wr <= 1'b1;
		            phe_state <= IDLE_S;
				end
				else begin
				    out_phe_data <= {2'b11,4'd0,128'd0};
				    out_phe_data_wr <= 1'b1;
                    reg_remain_pkt_len <= reg_remain_pkt_len - 12'd16;  
		            phe_state <= PKT_EXTENSION_S;				    
				end
			end
	    endcase
    end     
end 
endmodule 

