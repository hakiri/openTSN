/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: TSM.v
//Version: 1.0
//date: 2019/08/09
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// TSM(Transmitting and Scheduling Module):priority scheduling.
// 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module TSM#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input	   clk,
input      rst_n,

//receive from GCM
input      [7:0] in_tsm_valid,

//receive from PHE
(*mark_debug="TRUE"*)input    in_tsm_outport_free,

//receive from LCM
input    in_tsm_test_start,

//receive from UDO
(*mark_debug="TRUE"*)input     [7:0] in_tsm_fifo_usedw,  

//transmit to DRM,TGM,PHE
(*mark_debug="TRUE"*)output	reg [7:0]  out_tsm_selected
);
//************************************************************
//                priority scheduling
//************************************************************
(*mark_debug="TRUE"*)reg [1:0] tsm_state;
reg       init_flag;   
localparam IDLE_S = 2'd0,
           UDO_FIFO_FREE_S = 2'd1,
           PRIORITY_SCHEDULE_S = 2'd2;
always@(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
        out_tsm_selected <= 8'b0; 
		init_flag <= 1'b1;
		tsm_state <= IDLE_S;
    end
    else begin
	    case(tsm_state)
            IDLE_S:begin
			    if(init_flag || in_tsm_outport_free)begin
				    tsm_state <= UDO_FIFO_FREE_S;    
				end
				else begin
				    tsm_state <= IDLE_S; 
				end
			end
			UDO_FIFO_FREE_S:begin
	            if(in_tsm_fifo_usedw <= 8'd5)begin
		            tsm_state <= PRIORITY_SCHEDULE_S; 
		        end
		        else begin			
			        tsm_state <= UDO_FIFO_FREE_S; 
			    end
			end
            PRIORITY_SCHEDULE_S:begin
			    if(|out_tsm_selected)begin
				    out_tsm_selected <= 8'b0;
					init_flag <= 1'b0;
					tsm_state <= IDLE_S;
                end					
                else begin
				    tsm_state <= PRIORITY_SCHEDULE_S;	
	                casex(in_tsm_valid)
		                8'bxxxx_xxx1:out_tsm_selected <= 8'b0000_0001;
		                8'bxxxx_xx10:out_tsm_selected <= 8'b0000_0010;
                        8'bxxxx_x100:out_tsm_selected <= 8'b0000_0100;
                        8'bxxxx_1000:out_tsm_selected <= 8'b0000_1000;
			            8'bxxx1_0000:out_tsm_selected <= 8'b0001_0000;
			            8'bxx10_0000:out_tsm_selected <= 8'b0010_0000;
			            8'bx100_0000:out_tsm_selected <= 8'b0100_0000;
			            8'b1000_0000:out_tsm_selected <= 8'b1000_0000;
		                default:out_tsm_selected <= 8'b0;
		            endcase                 				
				end
		    end
	    endcase
    end     
end 
endmodule 

