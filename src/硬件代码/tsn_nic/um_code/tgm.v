/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: TGM.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// TGM:achieve rate limiting based on Token-Bucket algorithm;
//
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/08/08
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module TGM#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input			clk,
input			rst_n,
input           test_stop,

input           lau_update_finish,

//receive from LCM
(*mark_debug="TRUE"*)input	 [11:0] in_tgm_pkt_len,
(*mark_debug="TRUE"*)input         in_tgm_slot_shift, 
(*mark_debug="TRUE"*)input	 [15:0] in_tgm_tb_size,       //committed burst size is 4095 Byte.
(*mark_debug="TRUE"*)input   [15:0] in_tgm_tb_rate,       //add number of token per slot.  
//receive from TSM
input	        in_tgm_selected,    
         
//transmit to GCM
output	reg     out_tgm_req

);

wire [15:0] pkt_len;
assign pkt_len = {4'b0,in_tgm_pkt_len};
//token bucket parameter
(*mark_debug="TRUE"*)reg  [15:0] RT;        //remaining tokens
(*mark_debug="TRUE"*)reg  [15:0] CT;        //consume tokens,1Byte consumes 1 token.
//************************************************************
//                   consume tokens
//************************************************************
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        CT <= 16'd0;	
    end
    else begin
		if(in_tgm_selected == 1'b1)begin
		    CT <= pkt_len;
		end
		else begin
		    CT <= 16'd0;
		end
    end
end 

//************************************************************
//                    count token
//************************************************************
(*mark_debug="TRUE"*)reg [3:0] tb_state;
localparam INIT_S = 4'd0,
           TB_UPDATA_S = 4'd1;
reg slot_shift_slave;
always@(negedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        RT <= 16'd0;
        slot_shift_slave <= 1'b0;
        tb_state <= INIT_S;
    end
	else begin
	    case(tb_state)
	        INIT_S:begin
	            if(lau_update_finish == 1'b1)begin
	                RT <= in_tgm_tb_rate;
	                tb_state <= TB_UPDATA_S;
	            end
	            else begin
	                RT <= 16'd0;
	                tb_state <= INIT_S;
	            end   
	        end
	        TB_UPDATA_S:begin
	           if(test_stop == 1'b0)begin
	               tb_state <= TB_UPDATA_S;
	               if(slot_shift_slave != in_tgm_slot_shift) begin
	                   slot_shift_slave <= in_tgm_slot_shift;
	                   if(RT + in_tgm_tb_rate - CT <= in_tgm_tb_size)begin
	                       RT <= RT + in_tgm_tb_rate - CT;
	                   end
	                   else begin
	                       RT <= in_tgm_tb_size;
	                   end
	                end
	                else begin
	                    RT <= RT - CT;
	                end
                end
                else begin
                    RT <= 16'd0;
                    tb_state <= INIT_S;
                end
            end
            default:begin
                tb_state <= INIT_S;
            end
            endcase
        end
end

//************************************************************
//  judge whether the queue has request of traffic generation
//************************************************************
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_tgm_req <= 1'b0;
    end
    else begin
	    if(RT >= pkt_len)begin
		    out_tgm_req <= 1'b1;
		end
		else begin
		    out_tgm_req <= 1'b0;
		end
    end     
end        
endmodule



