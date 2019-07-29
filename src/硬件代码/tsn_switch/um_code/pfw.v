/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: pfw.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)judge pkt should be discard or transmit
// 2)judge where the pkt is going
// 3)transmit pkt and action to pac. 
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/15
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module  pfw(
input     wire    clk,
input     wire    rst_n,

//////////pkt and in_pfw_pkttype and in_pfw_key forom pke///
input     wire    [133:0]in_pfw_data,
input     wire    in_pfw_data_wr,
input     wire    in_pfw_valid,
input     wire    in_pfw_valid_wr,
input     wire    [2:0]in_pfw_pkttype,
input     wire    [101:0]in_pfw_key,

/////////pkt and out_pfw_action to pac ///////////////
output    reg     [133:0]out_pfw_data,
output    reg     out_pfw_data_wr,
output    reg     out_pfw_valid,
output    reg     out_pfw_valid_wr,
output    reg     [10:0]out_pfw_action,
output    reg     out_pfw_action_wr,

/////////reg from lcm ////////////////////////
input     wire    [47:0]local_mac_addr,
input     wire    [47:0]direct_mac_addr,
input     wire    direction
);
/////////register///////////////////
reg       [1:0]flag;//pkt from LCM or Local direct
reg       [133:0]delay0;
reg       [133:0]delay1;
/////////state machine/////////////
reg       [2:0]pfw_state;

localparam  IDLE_S   = 3'd0,
            S_COM_S  = 3'd1,
            D_COM_S  = 3'd2,
			TRANS_S  = 3'd3,
			DIC_S    = 3'd4;
			
always @(posedge clk or negedge rst_n) begin
      if(rst_n == 1'b0)begin
			out_pfw_data     <= 134'h0;
	        out_pfw_data_wr  <= 1'h0;
	        out_pfw_valid    <= 1'h0;
	        out_pfw_valid_wr <= 1'h0;
	        out_pfw_action           <= 11'h0;
	        out_pfw_action_wr        <= 1'h0;
			
			delay0           <= 134'h0;
			delay1           <= 134'h0;
	  
			pfw_state        <= IDLE_S;
	  end
	  else begin
			case(pfw_state)
				IDLE_S:begin
					out_pfw_data     <= 134'h0;
					out_pfw_data_wr  <= 1'h0;
					out_pfw_valid    <= 1'h0;
					out_pfw_valid_wr <= 1'h0;
					out_pfw_action           <= 11'h0;
					out_pfw_action_wr        <= 1'h0;	

					delay1           <= 134'h0;
					if(in_pfw_data_wr == 1'b1)begin
						delay0       <= in_pfw_data;
						
						pfw_state    <= S_COM_S;
						if(in_pfw_data[95:88] == 8'd4)begin// smid = 4 :pkt form PTP
							flag    <= 2'h2;
						end
						else if(in_pfw_data[95:88] == 8'd128)begin//smid = 128 :pkt form LCM
						   flag    <= 2'h1;
						end
						else begin
							flag    <= 2'h0;
						end
					end
					else begin
						delay0  <= 134'h0;
						
						pfw_state        <= IDLE_S;
					end
				end
				S_COM_S:begin
					if(in_pfw_data_wr == 1'b1)begin
						delay0       <= in_pfw_data;
						delay1       <= delay0;
						if(in_pfw_key[53:6] == direct_mac_addr)begin
							flag     <= 1'h1;
							if(in_pfw_key[5:0] == 6'h2)begin//pkt form Local direct
								pfw_state    <= D_COM_S;
							end
							else begin                      //updata
								pfw_state    <= DIC_S;
							end							
						end
						else begin
							flag             <= flag;
							if((in_pfw_key[5:0] == 6'h2)||(in_pfw_key[5:0] == 6'h3))begin//pkt from port3 and pkt from port2 but SMAC is not port2 local MAC
							  pfw_state        <= DIC_S;
							end
							else begin
						      pfw_state        <= D_COM_S;
							end
						end
					end
					else begin
					end
				end
				D_COM_S:begin
					if(in_pfw_data_wr == 1'b1)begin
						out_pfw_data     <= delay1;
						out_pfw_data_wr  <= 1'h1;
						out_pfw_valid    <= 1'h0;
						out_pfw_valid_wr <= 1'h0;	

						delay0       <= in_pfw_data;
						delay1       <= delay0;
						
						pfw_state    <= TRANS_S;
						if(in_pfw_key[101:54] == direct_mac_addr)begin //pkt transmit to port 2
							out_pfw_action         <= {2'b00,in_pfw_pkttype,6'h2};
							out_pfw_action_wr      <= 1'h1;
						end
						else if(in_pfw_key[101:54] == 48'hffffffffffff)begin
							if(flag == 2'h0)begin
								out_pfw_action         <= {2'b10,in_pfw_pkttype,5'h0,~in_pfw_key[0]};
								out_pfw_action_wr      <= 1'h1;
							end
							else begin
								out_pfw_action         <= {2'b10,in_pfw_pkttype,5'h0,direction};
								out_pfw_action_wr      <= 1'h1;
							end
						end
						else begin
							if(flag == 2'h1)begin //pkt from LCM or Local direct transmit to port direction
								out_pfw_action    <= {2'b00,in_pfw_pkttype,5'h0,direction};
								out_pfw_action_wr <= 1'h1;
							end
							else if(flag == 2'h2)begin
							    out_pfw_action    <= {2'b00,in_pfw_pkttype,5'h0,in_pfw_key[0]};
								out_pfw_action_wr <= 1'h1;
							end
							else begin  //pkt from port transmit to ~inport
								out_pfw_action    <= {2'b00,in_pfw_pkttype,5'h0,~in_pfw_key[0]};
								out_pfw_action_wr <= 1'h1;
							end
						end
					end
					else begin
						out_pfw_action    <= 11'h0;
						out_pfw_action_wr <= 1'h0;
						
						pfw_state <= D_COM_S;
					end
				end
				TRANS_S:begin
					out_pfw_data     <= delay1;
					out_pfw_data_wr  <= 1'h1;

					delay0       <= in_pfw_data;
					delay1       <= delay0;
					if(delay1[133:132] == 2'b10)begin
						out_pfw_valid    <= 1'h1;
						out_pfw_valid_wr <= 1'h1;
						if(in_pfw_data[133:132] == 2'b01)begin
						      pfw_state <= S_COM_S;
						end
						else if(delay0[133:132] == 2'b01)begin
						      pfw_state <= S_COM_S;
						end
						else begin
						      pfw_state <= IDLE_S;						      
						end		
					end
					else begin
						out_pfw_valid    <= 1'h0;
						out_pfw_valid_wr <= 1'h0;	
							
						pfw_state <= TRANS_S;	
					end
				end
				DIC_S:begin
					out_pfw_data     <= 134'h0;
					out_pfw_data_wr  <= 1'h0;
					out_pfw_valid    <= 1'h0;
					out_pfw_valid_wr <= 1'h0;
					out_pfw_action   <= 11'h0;
					out_pfw_action_wr<= 1'h0;
					delay0           <= 134'h0;
					delay1           <= 134'h0;
					
					if(in_pfw_data[133:132] == 2'b10)begin
						pfw_state <= IDLE_S;
					end
					else begin
						pfw_state <= DIC_S;
					end
				end
				default:begin
					out_pfw_data     <= 134'h0;
					out_pfw_data_wr  <= 1'h0;
					out_pfw_valid    <= 1'h0;
					out_pfw_valid_wr <= 1'h0;
					out_pfw_action           <= 11'h0;
					out_pfw_action_wr        <= 1'h0;
					delay0           <= 134'h0;
					delay1           <= 134'h0;
					
					pfw_state        <= IDLE_S;				
				end
			endcase
	  end
end	  
endmodule
/****************************************
pfw pfw(
.clk(),
.rst_n(),

//////////pkt and in_pfw_pkttype and in_pfw_key forom pke///
.in_pfw_data(),
.in_pfw_data_wr(),
.in_pfw_valid(),
.in_pfw_valid_wr(),
.in_pfw_pkttype(),
.in_pfw_key(),

/////////pkt and out_pfw_action to pac ///////////////
.out_pfw_data(),
.out_pfw_data_wr(),
.out_pfw_valid(),
.out_pfw_valid_wr(),
.out_pfw_action(),
.out_pfw_action_wr(),

/////////reg from lcm ////////////////////////
.local_mac_addr(),
.direct_mac_addr(),
.direction()
);
****************************************/