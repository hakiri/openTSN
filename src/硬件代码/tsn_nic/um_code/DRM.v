/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: DRM.v
//Version: 1.0
//date: 2019/08/09
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// DRM(Data Reading Module):read the selected ethernet header from PKT_HDR_RAM.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date: 
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module DRM#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input			clk,
input			rst_n,

//receive from LCM
input           in_drm_addr_shift,

//receive from TSM
input    [7:0]  in_drm_selected,

//receive from PKT_HDR_RAM
output	reg         out_drm_pkt_hdr_rd,
output	reg  [5:0]  out_drm_pkt_hdr_addr,
input	     [127:0]in_drm_pkt_hdr,

//transmit to PHE
output	reg  [127:0]out_drm_pkt_hdr,
output	reg         out_drm_pkt_hdr_wr
);
//************************************************************
//     read the selected ethernet header from PKT_HDR_RAM
//************************************************************
(*mark_debug="TRUE"*)reg [2:0] drm_state;
localparam IDLE_S = 3'd0,
           WAIT1_S = 3'd1,
		   WAIT2_S = 3'd2,
		   READ_DATA1_S = 3'd3,
           READ_DATA2_S = 3'd4,
		   READ_DATA3_S = 3'd5,
		   READ_DATA4_S = 3'd6;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_drm_pkt_hdr_rd <= 1'b0;
		out_drm_pkt_hdr_addr <= 6'd0;
		
		out_drm_pkt_hdr <= 128'b0;
		out_drm_pkt_hdr_wr <= 1'b0;
		
		drm_state <= IDLE_S;
    end
    else begin
	    case(drm_state)
            IDLE_S:begin
		        out_drm_pkt_hdr <= 128'b0;
		        out_drm_pkt_hdr_wr <= 1'b0;
			    if(in_drm_selected != 8'd0)begin
				    drm_state <= WAIT1_S; 
                    out_drm_pkt_hdr_rd <= 1'b1;
		            case(in_drm_selected)
		                8'b0000_0001:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd0};
		                8'b0000_0010:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd4};
                        8'b0000_0100:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd8};
                        8'b0000_1000:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd12};
		                8'b0001_0000:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd16};
		                8'b0010_0000:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd20};
		                8'b0100_0000:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd24};
		                8'b1000_0000:out_drm_pkt_hdr_addr <= {in_drm_addr_shift,5'd28};					
		                default:out_drm_pkt_hdr_addr <= 6'd0;	
		            endcase 					  
				end
				else begin
                    out_drm_pkt_hdr_rd <= 1'b0;
		            out_drm_pkt_hdr_addr <= 6'd0;
				    drm_state <= IDLE_S; 
				end
			end
			WAIT1_S:begin
			    out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= WAIT2_S;
		    end
			WAIT2_S:begin
			    out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= READ_DATA1_S;
		    end
            READ_DATA1_S:begin
		        out_drm_pkt_hdr <= in_drm_pkt_hdr;
		        out_drm_pkt_hdr_wr <= 1'b1;			
			    out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= READ_DATA2_S;
			end
            READ_DATA2_S:begin		        		        
		        out_drm_pkt_hdr <= in_drm_pkt_hdr;
		        out_drm_pkt_hdr_wr <= 1'b1;			
	            out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= READ_DATA3_S;
			end
            READ_DATA3_S:begin
                out_drm_pkt_hdr_rd <= 1'b0;
		        out_drm_pkt_hdr_addr <= 6'd0;
		        
		        out_drm_pkt_hdr <= in_drm_pkt_hdr;
		        out_drm_pkt_hdr_wr <= 1'b1;			
			//    out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= READ_DATA4_S;
			end
            READ_DATA4_S:begin
		        out_drm_pkt_hdr <= in_drm_pkt_hdr;
		        out_drm_pkt_hdr_wr <= 1'b1;			
			//    out_drm_pkt_hdr_addr <= out_drm_pkt_hdr_addr + 6'd1;
		        drm_state <= IDLE_S;
			end
	    endcase
    end     
end 
endmodule 
