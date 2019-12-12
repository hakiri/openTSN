/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: TGR.v
//Version: 1.0
//date: 2019/08/08
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// TGR(Traffic Generation Request):achieve rate limiting based on Token-Bucket algorithm;
//
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date: 
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module TGR#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input			clk,
input			rst_n,
input           test_stop,

input           lau_update_finish,

//receive from LCM
(*mark_debug="TRUE"*)input	 [11:0] in_tgr_pkt_len,
(*mark_debug="TRUE"*)input         in_tgr_slot_shift, 
(*mark_debug="TRUE"*)input	 [15:0] in_tgr_tb_size,       //committed burst size is 4095 Byte.
(*mark_debug="TRUE"*)input   [15:0] in_tgr_tb_rate,       //add number of token per slot.  
//receive from TSM
input	        in_tgr_selected,    
         
//transmit to GCM
output	reg     out_tgr_req
);

wire [15:0] pkt_len;
assign pkt_len = {4'b0,in_tgr_pkt_len+12'd4};
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
		if(in_tgr_selected == 1'b1)begin
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
	                RT <= in_tgr_tb_rate;
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
	               if(slot_shift_slave != in_tgr_slot_shift) begin
	                   slot_shift_slave <= in_tgr_slot_shift;
	                   if(RT + in_tgr_tb_rate - CT <= in_tgr_tb_size)begin
	                       RT <= RT + in_tgr_tb_rate - CT;
	                   end
	                   else begin
	                       RT <= in_tgr_tb_size;
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
        out_tgr_req <= 1'b0;
    end
    else begin
	    if(RT >= pkt_len)begin
		    out_tgr_req <= 1'b1;
		end
		else begin
		    out_tgr_req <= 1'b0;
		end
    end     
end        
endmodule



