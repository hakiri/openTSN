    /////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor:FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: addr_mgmt.v
//Version: 3.1
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)manage address of data_cache
// 2)transmit address to data_ctrl
// 3)transmit id to gpp
//*************************************************************
//                     Revision List
//*************************************************************
//    rn1: 
//      date:  2019/04/01
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

`timescale 1ns / 1ps

module addr_mgmt#(
    parameter    PLATFORM = "xilinx"
)(
    input  wire  clk,
    input  wire  rst_n,

//write
    input  wire  [133:0]in_addr_mgmt_data,
	input  wire  in_addr_mgmt_data_wr,
	input  wire  in_addr_mgmt_valid,
    input  wire  in_addr_mgmt_valid_wr,
    output reg   [7:0]out_addr_mgmt_ID,  
    output wire  [4:0]out_addr_mgmt_ID_count,      
    output wire  [10:0]addr2data_waddr, 
    output reg   addr2data_waddr_wr, 

//read   
    output reg   [10:0]addr2data_raddr,
    output reg   addr2data_raddr_wr,
    input  wire  [7:0]in_addr_mgmt_ID,
    input  wire  in_addr_mgmt_ID_wr,
    input  wire  in_ram2addr_valid

);
///////reg fifo /////////
reg     [7:0]cfifo_wdata; 
reg     cfifo_wr;
wire    [7:0]cfifo_rdata;
reg     cfifo_rd;
wire    cfifo_empty;   
wire    cfifo_full; 
wire    [4:0]cfifo_data_count;
////////reg////////////
reg     fifo_flag;     //FIFO initialization completed
reg     [4:0]fifo_cnt; //FIFO initialization completed,RAM have 16 id 

assign  out_addr_mgmt_ID_count = cfifo_data_count;
assign  addr2data_waddr = {cfifo_rdata[3:0], 7'h0};  
//////////////write/////////////////////
reg [1:0]   write_state;
localparam  W_IDLE_S = 2'd0,
			WBEGIN_S = 2'd1,
            WRITE_S  = 2'd2,
            W_WITE_S = 2'd3;            

always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
       cfifo_rd            <= 1'b0;
       
       write_state         <= W_IDLE_S;
    end
    else begin
       case(write_state)                    
          W_IDLE_S:begin
             cfifo_rd            <= 1'b0;
             if((fifo_flag == 1'b1)&&(cfifo_empty == 1'b0))begin//transmit write base_ID to data_ctrl
                 addr2data_waddr_wr <= 1'b1;                                
                 write_state <= WBEGIN_S;
             end
             else begin
				 addr2data_waddr_wr <= 1'b0; 
                 write_state <= W_IDLE_S;
             end
          end     
		  
		  WBEGIN_S:begin
			  addr2data_waddr_wr <= 1'b0; 
			  if((in_addr_mgmt_data[133:132] == 2'b01)&&(in_addr_mgmt_data_wr == 1'b1))begin
				cfifo_rd <= 1'b1;
                out_addr_mgmt_ID <= cfifo_rdata;
				write_state <= WRITE_S;
			  end
			  else begin
				cfifo_rd <= 1'b0;
				write_state <= WBEGIN_S;
			  end
		  end
		  
          WRITE_S:begin
			  cfifo_rd <= 1'b0;
              addr2data_waddr_wr <= 1'b0; 
              case({in_addr_mgmt_valid,in_addr_mgmt_valid_wr})//waiting data_ctrl transmit pkt completed
                 2'b11:begin//read out the ID used   
                      write_state <= W_IDLE_S;
                 end
                 2'b01:begin
                      write_state <= W_IDLE_S;
                 end
                 default:begin
                      write_state <= WRITE_S;
                 end
              endcase
          end          
          W_WITE_S:begin
              cfifo_rd <= 1'b0;
            // addr2data_waddr <= {cfifo_rdata[3:0], 7'h0};  
            // addr2data_waddr_wr <= 1'b1; 
              write_state <= W_IDLE_S;          
          end          
         default:begin
           cfifo_rd            <= 1'b0;
         
           write_state         <= W_IDLE_S;     
         end
      endcase
   end
end
////////////////read//////////////////
(*MARK_DEBUG="TRUE"*)reg [2:0]   read_state;
localparam  FIFO_S     = 3'd0,
            R_IDLE_S   = 3'd1,
            READ_S     = 3'd2,
            FINISH_S   = 3'd3,
            R_WITE_S   = 3'd4;    

always @(posedge clk or negedge rst_n) begin 
    if(rst_n == 1'b0) begin 
      addr2data_raddr_wr <= 1'b0;
      cfifo_wr   <= 1'b0;
      fifo_cnt   <= 5'd0;
       
       read_state <= FIFO_S;
    end
    else begin
       case(read_state)   
          FIFO_S:begin   // FIFO initialization,writing idle ID 
              if(fifo_cnt < 5'd16)begin
                fifo_cnt <= fifo_cnt + 5'h1;
                cfifo_wr <= 1'b1;
                fifo_flag <= 1'b0;
                cfifo_wdata <= fifo_cnt;
                
                read_state <= FIFO_S;
               end
              else begin
                fifo_cnt <= fifo_cnt; 
                fifo_flag <= 1'b1;
                cfifo_wr <= 1'b0;
                
                read_state <= R_IDLE_S;           
              end
          end
		  
          R_IDLE_S:begin  
             addr2data_raddr_wr <= 1'b0;
             cfifo_wr           <= 1'b0;
             if((cfifo_data_count[4] == 1'b1)&&(fifo_flag == 1'b1))begin
                 read_state <= R_IDLE_S;
             end
             else begin
                 read_state <= READ_S;
             end
          end  
          READ_S:begin
             if(in_addr_mgmt_ID_wr == 1'b1)begin//transmit read base_ID to data_ctrl
                 addr2data_raddr <= {in_addr_mgmt_ID[3:0],7'h0};
                 addr2data_raddr_wr <= 1'b1;
                 cfifo_wr    <= 1'b1;
                 cfifo_wdata <= in_addr_mgmt_ID;
                 read_state      <= FINISH_S;
             end
             else begin
                 read_state      <= READ_S;
             end
          end  
          FINISH_S:begin
              addr2data_raddr_wr <= 1'b0;
              cfifo_wr <= 1'b0;
              if(in_ram2addr_valid == 1'b1)begin//write the returned ID
                  read_state      <= R_WITE_S;
              end
              else begin
                  read_state      <= FINISH_S;
              end
          end 
          R_WITE_S:begin			
              cfifo_wr    <= 1'b0;          
              if(in_addr_mgmt_ID_wr == 1'b0)begin
                   read_state      <= R_IDLE_S;
              end
              else begin
                   read_state      <= R_WITE_S;
              end
          end
          default:begin
              fifo_flag  <= 1'b0;
              cfifo_wr   <= 1'b0;
              addr2data_raddr_wr <= 1'b0;
          
              read_state <= R_IDLE_S;
          end
        endcase
    end
end

 fifo_8_16  fifo_8_16_inst(
    .srst(~rst_n),
    .clk(clk),
    .din(cfifo_wdata),
    .rd_en(cfifo_rd),
    .wr_en(cfifo_wr),
    .dout(cfifo_rdata),
    .data_count(cfifo_data_count),
    .empty(cfifo_empty),
    .full(cfifo_full)

    );
endmodule
/**********************
.addr_mgmt  addr_mgmt(
              
. clk(),
. rst_n(),


. in_addr_mgmt_valid(),
. in_addr_mgmt_valid_wr(),
. out_addr_mgmt_ID(),    
. out_addr_mgmt_ready(), 
. addr2data_waddr(),
. addr2data_waddr_wr(), 


. addr2data_raddr(),
. addr2data_raddr_wr(),
. in_addr_mgmt_ID(),
. in_addr_mgmt_ready(),
. in_ram2addr_valid()

);
***********************/
