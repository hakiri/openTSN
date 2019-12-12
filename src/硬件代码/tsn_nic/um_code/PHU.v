/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: PHU.v
//Version: 1.0
//date: 2019/08/10
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// PHU(PKT Header Update):Update 8 PKT headers.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module PHU#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input		        clk,
input		    	rst_n,

//receive from FPGA OS
input	     [133:0]in_phu_data,
input	            in_phu_data_wr,

//transmit to PKT_HDR_RAM
output	reg         out_phu_pkt_hdr_wr,
output	reg  [5:0]  out_phu_pkt_hdr_addr,
output	reg  [127:0]out_phu_pkt_hdr,

//transmit to ARM
output	reg  [7:0]  out_phu_pkt_hdr_seq,

//transmit to PGM
output	reg         out_phu_addr_shift,

//transmit to LCM
input               in_phu_test_stop,
output	reg         out_phu_update_finish
);

//************************************************************
//               Update 8 PKT headers
//************************************************************
(*mark_debug="TRUE"*)reg [2:0] phu_state;
localparam IDLE_S = 3'd0,
           WAIT1_S = 3'd1,
		   JUDGE_TYPE_S = 3'd2,
		   WAIT2_S = 3'd3,
           WAIT3_S = 3'd4,
		   READ_PKT_HDR1_S = 3'd5,
		   READ_PKT_HDR_S = 3'd6;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_phu_pkt_hdr_wr <= 1'b0;
		out_phu_pkt_hdr_addr <= 6'b0;
		out_phu_pkt_hdr <= 128'b0;
        out_phu_pkt_hdr_seq <= 8'b0;
		out_phu_addr_shift <= 1'b1;
		out_phu_update_finish <= 1'b0;
		phu_state <= IDLE_S;
    end
    else begin
	    case(phu_state)
            IDLE_S:begin
                out_phu_pkt_hdr_wr <= 1'b0;
                out_phu_pkt_hdr_addr <= 6'b0;				
                out_phu_pkt_hdr <= 128'b0;
                if(in_phu_test_stop == 1'b0)begin			
   			        if(in_phu_data_wr == 1'b1 && in_phu_data[133:132] == 2'b01)begin
				        phu_state <= WAIT1_S;   
				    end
				    else begin
				        phu_state <= IDLE_S; 
				    end	
				end
				else begin
				    out_phu_update_finish <= 1'b0;
				    phu_state <= IDLE_S; 
				end		
			end
			WAIT1_S:begin   //metadata1
				phu_state <= JUDGE_TYPE_S;  
		    end
			JUDGE_TYPE_S:begin
                if(in_phu_data[31:16] == 16'hff01 && in_phu_data[15:12] == 4'h1)begin
                    out_phu_pkt_hdr_seq <= in_phu_data[7:0];				
				    phu_state <= WAIT2_S; 
                end
                else begin
                    phu_state <= IDLE_S; 
                end				
		    end
			WAIT2_S:begin   //encapsulated metadata0
				phu_state <= WAIT3_S;  
		    end		
			WAIT3_S:begin   //encapsulated metadata1
				phu_state <= READ_PKT_HDR1_S;  
		    end				
			READ_PKT_HDR1_S:begin
                out_phu_pkt_hdr_wr <= 1'b1;
                out_phu_pkt_hdr_addr <= {~out_phu_addr_shift,5'd0};				
                out_phu_pkt_hdr <= in_phu_data[127:0];	
				phu_state <= READ_PKT_HDR_S; 
		    end
			READ_PKT_HDR_S:begin
                 out_phu_pkt_hdr_wr <= 1'b1;
                 out_phu_pkt_hdr_addr <= out_phu_pkt_hdr_addr + 6'd1;				
                 out_phu_pkt_hdr <= in_phu_data[127:0];	
			    if(in_phu_data[133:132] == 2'b10)begin
					out_phu_addr_shift <= ~out_phu_addr_shift;
					out_phu_update_finish <= 1'b1;
				    phu_state <= IDLE_S; 
				end
				else begin
				    phu_state <= READ_PKT_HDR_S; 				
				end
		    end
	    endcase
    end     
end 
endmodule 
