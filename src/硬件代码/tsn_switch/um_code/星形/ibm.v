/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor:FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: ibm.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)receive pkt from port or cpu
// 2)transmit pkt to data_cache
// 3)transmit md  to eos
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/15
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

`timescale 1ns / 1ps

module ibm(
   input  wire     clk,
   input  wire     rst_n,

//receive pkt from cpu or port
   input  wire[133:0]     in_ibm_data,
   input  wire            in_ibm_data_wr,
   input  wire            in_ibm_valid,
   input  wire            in_ibm_valid_wr,
   output reg[4:0]        out_ibm_bufm_ID,
   
   input  wire[23:0]      in_ibm_tsn_md,
   input  wire            in_ibm_tsn_md_wr,  

//transmit pkt to data_cache
   output reg[133:0]      out_ibm_data,
   output reg             out_ibm_data_wr,
   output reg             out_ibm_valid,
   output reg             out_ibm_valid_wr,
   
   input  wire[7:0]       in_ibm_ID,
   input  wire[4:0]       in_ibm_ID_count,

//parse TSN_MD transmit to next module
   output reg[23:0]       out_ibm_md,
   output reg             out_ibm_md_wr
);

reg          out_ibm_valid_delay;
reg          [23:0]tsn_md_reg;
reg          [7:0]in_ibm_ID_count_delay0;
reg          [7:0]in_ibm_ID_count_delay1;

reg [1:0]    data_state;
//***************************************************
//             Retransmit Pkt To Data_Cache
//***************************************************
localparam    IDLE_S   = 2'd0,
              TRANS_S  = 2'd1,
              DISC_S   = 2'd2;     
              
always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
        out_ibm_data_wr <= 1'b0;
        out_ibm_data <= 134'b0;
		out_ibm_valid <= 1'b0;
        out_ibm_valid_wr <= 1'b0;
		
        data_state <= IDLE_S;
    end
    else begin
        case(data_state)
            IDLE_S: begin  
			    out_ibm_valid <= 1'b0;	
                out_ibm_valid_wr <= 1'b0;
                if((in_ibm_data[133:132] == 2'b01)&&(in_ibm_data_wr==1'b1)) begin
                    if((in_ibm_data[87:80]==8'd1)||(in_ibm_data[87:80]>8'd4)) begin
                        out_ibm_data_wr <= 1'b1;
                        out_ibm_data <= in_ibm_data;
						
                        data_state <= TRANS_S;
                    end 
                    else begin 
                        out_ibm_data_wr <= 1'b0;
                        out_ibm_data <= 134'b0;
						
                        data_state <= DISC_S;
                    end
                end
                else begin
                    out_ibm_data_wr <= 1'b0;
                    out_ibm_data <= 134'b0;
					
                    data_state <= IDLE_S;
                end
            end
            TRANS_S: begin				   
                out_ibm_data_wr <= 1'b1;
                out_ibm_data <= in_ibm_data;
			    out_ibm_valid <= in_ibm_valid;
                if(in_ibm_data[133:132] == 2'b10) begin
                    out_ibm_valid_wr <= 1'b1; 
					
                    data_state <= IDLE_S;
                end
                else begin
                    out_ibm_valid_wr <= 1'b0;    
					
                    data_state <= TRANS_S;
                end
            end 
            DISC_S: begin 
					out_ibm_data_wr <= 1'b0;
					out_ibm_valid_wr <= 1'b0;
               if(in_ibm_data[133:132] == 2'b10) begin
                    data_state <= IDLE_S;
               end 
               else begin 
                    data_state <= DISC_S;
               end
              end
          endcase
     end
end

//************************************************
//              Transmit MD
//************************************************
always @(posedge clk or negedge rst_n) begin//wait for in_ibm_ID
    if(rst_n == 1'b0) begin 
        out_ibm_md    <= 24'b0;
    end
    else begin 	 
	    out_ibm_md <= {tsn_md_reg[23:8],in_ibm_ID}; 
	end
end

always @(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin 
		tsn_md_reg <= 24'b0;
    end
    else begin 
        if(in_ibm_tsn_md_wr == 1'h1)begin
	        tsn_md_reg <= in_ibm_tsn_md;
	    end
	    else begin
	        tsn_md_reg <= tsn_md_reg;
	    end
    end
end
always @(posedge clk or negedge rst_n) begin//wait for in_ibm_ID
    if(rst_n == 1'b0) begin 
		   in_ibm_ID_count_delay0    <= 8'h0;
		   in_ibm_ID_count_delay1    <= 8'h0;
    end
    else begin 	 
	       in_ibm_ID_count_delay0 <= in_ibm_ID_count; 
		   in_ibm_ID_count_delay1 <= in_ibm_ID_count_delay0;
		   out_ibm_bufm_ID        <= in_ibm_ID_count_delay1;
	end
end
always @(posedge clk or negedge rst_n) begin//delay onr frame for synchronize with out_ibm_md
    if(rst_n == 1'b0) begin                 //ensure in_ibm_ID in the out_ibm_md
		out_ibm_valid_delay <= 1'b0;
    end
    else begin 
	    out_ibm_valid_delay <= out_ibm_valid;
    end
end

always @(posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin 
		out_ibm_md_wr <= 1'b0;
    end
    else begin 
	    out_ibm_md_wr <= out_ibm_valid_delay;
    end
end

endmodule

/********************************************
ibm ibm_inst(
.clk(),
.rst_n(),

.in_ibm_data(),
.in_ibm_data_wr(),
.in_ibm_valid(),
.in_ibm_valid_wr(),
.out_ibm_bufm_ID(),

.in_ibm_tsn_md(),
.in_ibm_tsn_md_wr(),

.out_ibm_data(),
.out_ibm_data_wr(),
.out_ibm_valid(),
.out_ibm_valid_wr(),

.in_ibm_ID(),
.in_ibm_ID_count(),

.out_ibm_md(),
.out_ibm_md_wr()
);
************************************************/