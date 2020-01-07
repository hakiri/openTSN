/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor:FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: data_ctrl.v
//Version: 3.1
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)memory pkt to RAM
// 2)receive address from addr_mgmt
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/04/01
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

`timescale 1ns / 1ps

module data_ctrl (
   input wire       clk,
   input wire       rst_n,
///////pkt form ibm ////////////////////   
   input wire       [133:0]in_data_ctrl_data,
   input wire       in_data_ctrl_data_wr,
//////write address from addr_mgmt ////   
   input wire       [10:0]addr2data_waddr,
   input wire       addr2data_waddr_wr,
//////read  address from addr_mgmt ////      
   input wire       [10:0]addr2data_raddr,
   input wire       addr2data_raddr_wr,
/////transmit pkt to ebm//////////////   
   output reg       [133:0]out_data_cache_data,
   output reg       out_data_cache_data_wr,
   output reg       out_data_cache_valid,
   output reg       out_data_cache_valid_wr
);
reg           [10:0]data2ram_waddr;
reg           ram_wr;
reg           [133:0]ram_din_data;

reg           [10:0]data2ram_raddr;
reg           ram_rd;
wire          [133:0]ram_dout_data;

reg           flag;
/////////write//////////////////////
reg           [1:0]pkt_write_state;
localparam     W_IDLE_S = 2'd0,
               FIRST_S  = 2'd1,
			   WRITE_S  = 2'd2;			   
always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
       ram_wr           <= 1'b0;
	   
	   pkt_write_state  <= W_IDLE_S;
    end
	else begin
	   case(pkt_write_state)		  		  
		  W_IDLE_S:begin
		      ram_wr           <= 1'b0;
		      if(addr2data_waddr_wr == 1'b0)begin				   
				   pkt_write_state  <= W_IDLE_S;
			  end
			  else if(in_data_ctrl_data_wr == 1'b1)begin //data_wr and addr_wr come togeter,tansmit pkt
			       data2ram_waddr   <= addr2data_waddr ;
			       ram_din_data     <= in_data_ctrl_data;
                   ram_wr           <= 1'b1;
                   flag             <= 1'b1;//data_wr and addr_wr come togeter
			       pkt_write_state  <= FIRST_S;	
			  end
			  else begin
			       flag             <= 1'b0;
			       pkt_write_state  <= FIRST_S;			       
			  end
		  end		  
		  FIRST_S:begin//write pkt to RAM and address is write base_addr
		       if(flag == 1'b1)begin
		          if(in_data_ctrl_data_wr == 1'b1)begin
                       ram_din_data     <= in_data_ctrl_data;
                       ram_wr           <= 1'b1;
                       data2ram_waddr   <= data2ram_waddr + 10'h1;
                    
                       pkt_write_state  <= WRITE_S;
                   end
                   else begin
                       ram_wr           <= 1'b0;
                    
                       pkt_write_state  <= FIRST_S;    			
		          end
		       end
		       else begin
		          if(in_data_ctrl_data_wr == 1'b1)begin
                       ram_din_data     <= in_data_ctrl_data;
                       ram_wr           <= 1'b1;
                       data2ram_waddr   <= addr2data_waddr ;
                    
                       pkt_write_state  <= WRITE_S;
                  end
                  else begin
                       ram_wr           <= 1'b0;
                    
                       pkt_write_state  <= FIRST_S;    		          
		          end
		       end
		  end		  
		  WRITE_S:begin//write pkt to RAM and address is address+1
		       ram_wr           <= 1'b1;
               ram_din_data     <= in_data_ctrl_data;
		       if(in_data_ctrl_data[133:132] == 2'b10)begin
                    data2ram_waddr   <= data2ram_waddr + 10'h1;
                    					
					pkt_write_state  <= W_IDLE_S;
			   end
			   else begin
					data2ram_waddr   <= data2ram_waddr + 10'h1;
					
					pkt_write_state  <= WRITE_S;
			   end
		  end	  
		  default:begin    
               ram_wr           <= 1'b0;
			   
               pkt_write_state         <= W_IDLE_S;
		 end
	  endcase
   end
end
//////////////read/////////////////////////////
reg           [3:0]pkt_read_state;
localparam     R_IDLE_S = 4'd0,
               WAIT0_S  = 4'd1,
			   WAIT1_S  = 4'd2,
			   READ_S   = 4'd3;		   
always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
       ram_rd           <= 1'b0;
	   
	   out_data_cache_data    <= 134'b0;
	   out_data_cache_data_wr <= 1'b0;
	   out_data_cache_valid   <= 1'b0;
	   out_data_cache_valid_wr<= 1'b0;
	   
	   pkt_read_state  <= R_IDLE_S;
    end
	else begin
	   case(pkt_read_state)		  		  
		  R_IDLE_S:begin
		      out_data_cache_data    <= 134'b0;
		      out_data_cache_data_wr <= 1'b0;
			  out_data_cache_valid   <= 1'b0;
			  out_data_cache_valid_wr<= 1'b0;
 
		      if(addr2data_raddr_wr == 1'b1)begin//read pkt from RAM and address is read base_addr
			       data2ram_raddr   <= addr2data_raddr;
				   ram_rd           <= 1'b1;
				   
				   pkt_read_state  <= WAIT0_S;
			  end
			  else begin
			       ram_rd           <= 1'b0;
				   
			       pkt_read_state  <= R_IDLE_S;			       
			  end
		  end		  
		  WAIT0_S:begin//dealy,because RAM read out data need delay two frame
              ram_rd           <= 1'b1;
			  data2ram_raddr   <= data2ram_raddr + 10'h1;
			  
			  pkt_read_state   <= WAIT1_S;
		  end		  
		  WAIT1_S:begin
		  
              ram_rd              <= 1'b1;
			  data2ram_raddr      <= data2ram_raddr + 10'h1;			 			  
			  
			  pkt_read_state  <= READ_S;
		  end		  		  
		  READ_S:begin//ensure all pkt read through len
		      out_data_cache_data    <= ram_dout_data;			  
		      if(out_data_cache_data[133:132] == 2'b10)begin		  
				  ram_rd           <= 1'b0;
				  data2ram_raddr   <= 10'h0;
				  out_data_cache_valid   <= 1'b0;
				  out_data_cache_valid_wr<= 1'b0;
				  out_data_cache_data_wr <= 1'b0;
				  
				  pkt_read_state   <= R_IDLE_S;
			  end
			  else if(ram_dout_data[133:132] == 2'b10)begin
				  ram_rd           <= 1'b1;
                  data2ram_raddr   <= data2ram_raddr + 10'h1;
                  out_data_cache_valid   <= 1'b1;
                  out_data_cache_valid_wr<= 1'b1;
                  out_data_cache_data_wr <= 1'b1;  
                  
                  pkt_read_state   <= READ_S;   			  
			  end
			  else begin
				  ram_rd           <= 1'b1;
				  data2ram_raddr   <= data2ram_raddr + 10'h1;
				  out_data_cache_valid   <= 1'b0;
				  out_data_cache_valid_wr<= 1'b0;
				  out_data_cache_data_wr <= 1'b1;				  
				  
				  pkt_read_state   <= READ_S;
			  end
		  end		                       			  
		  default:begin    
              ram_rd           <= 1'b0;
	    
	          out_data_cache_data    <= 134'b0;
	          out_data_cache_data_wr <= 1'b0;
	          out_data_cache_valid   <= 1'b0;
	          out_data_cache_valid_wr<= 1'b0;
	   
	          pkt_read_state  <= R_IDLE_S;
		  end
	  endcase
   end
end
ram_134_2048 data_ram
(      
    .clka(clk),
	.ena(ram_wr),
    .dina(ram_din_data),
    .wea(1'b1),
    .addra(data2ram_waddr),
    .douta(),
	
    .clkb(clk),
	.enb(ram_rd),
    .web(1'b0),
    .addrb(data2ram_raddr),
    .dinb(32'b0),
    .doutb(ram_dout_data)    
);
endmodule
/*********************
.clk(),
.rst_n(),
///////pkt form ibm ////////////////////   
.in_data_ctrl_data(),
.in_data_ctrl_data_wr(),
//////write address from addr_mgmt ////   
.addr2data_waddr(),
.addr2data_waddr_wr(),
//////read  address from addr_mgmt ////     
.addr2data_raddr(),
.addr2data_raddr_wr(),
/////transmit pkt to ebm//////////////   
.out_data_cache_data(),
.out_data_cache_data_wr(),
.out_data_cache_valid(),
.out_data_cache_valid_wr()
********************/