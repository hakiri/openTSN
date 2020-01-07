/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 Xperis, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: Hunan Xperis Network Technology Co.,Ltd.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: goe.v
//Version: 2.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)Transmit pkt to port or cpu
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2018/08/24
//      modifier: 
//      description: 
///////////////////////////////////////////////////////////////// 

module goe #(
	parameter PLATFORM = "xilinx",
			  LMID = 8'd5
	)(

    input clk,
    input rst_n,
	
//uda pkt waiting for transmit
    input in_goe_data_wr_0,
    input [133:0] in_goe_data_0,
    input in_goe_valid_wr_0,
    input in_goe_valid_0,
    
    input in_goe_data_wr_1,
    input [133:0] in_goe_data_1,
    input in_goe_valid_wr_1,
    input in_goe_valid_1,
    
    input in_goe_data_wr_2,
    input [133:0] in_goe_data_2,
    input in_goe_valid_wr_2,
    input in_goe_valid_2,
    
    input in_goe_data_wr_3,
    input [133:0] in_goe_data_3,
    input in_goe_valid_wr_3,
    input in_goe_valid_3,
	
//pkt waiting for transmit
    output wire pktout_data_wr_0,
    output wire [133:0] pktout_data_0,
    output wire pktout_data_valid_wr_0,
    output wire pktout_data_valid_0,

    //pkt waiting for transmit
    output wire pktout_data_wr_1,
    output wire [133:0] pktout_data_1,
    output wire pktout_data_valid_wr_1,
    output wire pktout_data_valid_1,
    
    output wire pktout_data_wr_2,
    output wire [133:0] pktout_data_2,
    output wire pktout_data_valid_wr_2,
    output wire pktout_data_valid_2,    
           
    output wire pktout_data_wr_3,
    output wire [133:0] pktout_data_3,
    output wire pktout_data_valid_wr_3,
    output wire pktout_data_valid_3
);
    assign pktout_data_wr_0        =  in_goe_data_wr_0;    
    assign pktout_data_0           =  in_goe_data_0;
    assign pktout_data_valid_wr_0  =  in_goe_valid_wr_0; 
    assign pktout_data_valid_0     =  in_goe_valid_0;  
    
    assign pktout_data_wr_1        =  in_goe_data_wr_1;    
    assign pktout_data_1           =  in_goe_data_1;
    assign pktout_data_valid_wr_1  =  in_goe_valid_wr_1; 
    assign pktout_data_valid_1     =  in_goe_valid_1;  
    
    assign pktout_data_wr_2        =  in_goe_data_wr_2;    
    assign pktout_data_2           =  in_goe_data_2;
    assign pktout_data_valid_wr_2  =  in_goe_valid_wr_2; 
    assign pktout_data_valid_2     =  in_goe_valid_2;  
    
    assign pktout_data_wr_3        =  in_goe_data_wr_3;    
    assign pktout_data_3           =  in_goe_data_3;
    assign pktout_data_valid_wr_3  =  in_goe_valid_wr_3; 
    assign pktout_data_valid_3     =  in_goe_valid_3;  

endmodule			