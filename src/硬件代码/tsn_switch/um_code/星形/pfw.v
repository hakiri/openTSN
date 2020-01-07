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
input     wire    [53:0]in_pfw_key,

input     wire    in_reg_tap,
/////////pkt and out_pfw_action to pac ///////////////
(*MARK_DEBUG="TRUE"*)output    reg     [133:0]out_pfw_data,
(*MARK_DEBUG="TRUE"*)output    reg     out_pfw_data_wr,
(*MARK_DEBUG="TRUE"*)output    reg     out_pfw_valid,
(*MARK_DEBUG="TRUE"*)output    reg     out_pfw_valid_wr,
(*MARK_DEBUG="TRUE"*)output    reg     [10:0]out_pfw_action,
(*MARK_DEBUG="TRUE"*)output    reg     out_pfw_action_wr,

/////////reg from lcm ////////////////////////
input     wire    [47:0]MAC_0,
input     wire    [47:0]MAC_1,
input     wire    [47:0]MAC_2,
input     wire    [47:0]MAC_3,
input     wire    [47:0]MAC_4,
input     wire    [47:0]MAC_5,
input     wire    [47:0]MAC_6,
input     wire    [47:0]MAC_7,
input     wire    [47:0]MAC_8,
input     wire    [47:0]MAC_9,
input     wire    [47:0]MAC_10,
input     wire    [47:0]MAC_11,

input     wire    [15:0]port_0,
input     wire    [15:0]port_1,
input     wire    [15:0]port_2,
input     wire    [15:0]port_3,
input     wire    [15:0]port_4,
input     wire    [15:0]port_5,
input     wire    [15:0]port_6,
input     wire    [15:0]port_7,
input     wire    [15:0]port_8,
input     wire    [15:0]port_9,
input     wire    [15:0]port_10,
input     wire    [15:0]port_11

);

/////////register///////////////////
(*MARK_DEBUG="TRUE"*)reg       [133:0]delay0;
/////////state machine/////////////
(*MARK_DEBUG="TRUE"*)reg       [2:0]pfw_state;

localparam  IDLE_S   = 3'd0,
            FIND_S   = 3'd1,
			TRANS_S  = 3'd2,
			DIC_S    = 3'd3;

always @(posedge clk or negedge rst_n) begin	
      if(rst_n == 1'b0)begin
			out_pfw_data     	<= 134'h0;
	        out_pfw_data_wr  	<= 1'h0;
	        out_pfw_valid    	<= 1'h0;
	        out_pfw_valid_wr 	<= 1'h0;
	        out_pfw_action      <= 11'h0;
	        out_pfw_action_wr   <= 1'h0;
			
			delay0           	<= 134'h0;
		
			pfw_state        	<= IDLE_S;
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
				if(in_pfw_data_wr == 1'b1)begin
					if((in_pfw_data[125:120] == 6'd3)&&(in_reg_tap == 1'b1))begin
						delay0       <= in_pfw_data;
						
						pfw_state    <= DIC_S;
					end
					else begin
						delay0       <= in_pfw_data;
						pfw_state    <= FIND_S;
					end
				end
				else begin
					delay0       <= 134'h0;	
					pfw_state    <= IDLE_S;
				end
			end

			FIND_S:begin
				delay0       <= in_pfw_data; 
                out_pfw_valid    <= 1'h0;  
                out_pfw_valid_wr <= 1'h0; 
				if(in_pfw_key[47:0] == 48'hffffffffffff)begin
					out_pfw_data     <= delay0;
					out_pfw_data_wr  <= 1'h1; 
					if(delay0[95:88] == 8'd4)begin
						out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b001111};
						out_pfw_action_wr <= 1'h1;
						pfw_state    <= TRANS_S;
					end
					else begin
						case(in_pfw_key[53:48])
						6'h0:begin
							out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b001110};
							out_pfw_action_wr <= 1'h1;
							pfw_state    <= TRANS_S;
						end
						6'h1:begin
							out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b001101};
							out_pfw_action_wr <= 1'h1;
							pfw_state    <= TRANS_S;
						end
						6'h2:begin
							out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b001011};
							out_pfw_action_wr <= 1'h1;
							pfw_state    <= TRANS_S;
						end
						6'h3:begin
							out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b000111};
							out_pfw_action_wr <= 1'h1;
							pfw_state    <= TRANS_S;
						end
						default:begin
							out_pfw_action    <= {2'b10,in_pfw_pkttype,6'b000000};
							out_pfw_action_wr <= 1'h1;
							pfw_state    <= TRANS_S;
						end
						endcase
					end
				end
				else begin
					if(in_pfw_key[47:24] == 24'h01005e)begin
						out_pfw_action[10:6] <= {2'b11,in_pfw_pkttype};
					end
					else begin
						out_pfw_action[10:6] <=  {2'b00,in_pfw_pkttype};
					end
								
					if(in_pfw_key[47:0] == MAC_0)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_0[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;				
				    end
				    else if(in_pfw_key[47:0] == MAC_1)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_1[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_2)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_2[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_3)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_3[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_4)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_4[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_5)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_5[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_6)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_6[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_7)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_7[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_8)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_8[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_9)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_9[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_10)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_10[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end
				    else if(in_pfw_key[47:0] == MAC_11)begin
				    	out_pfw_data        <= delay0;
				    	out_pfw_data_wr     <= 1'h1; 
				    	out_pfw_action[5:0] <= port_11[5:0];
				    	out_pfw_action_wr   <= 1'h1;	
				    	pfw_state           <= TRANS_S;	
				    end				
				    else begin
				    	out_pfw_data     <= 134'h0;
				    	out_pfw_data_wr  <= 1'h0; 
				    	pfw_state    <= DIC_S;
				    end
				end
			end

			TRANS_S:begin
				out_pfw_data     <= delay0;
				out_pfw_data_wr  <= 1'h1;

				delay0       <= in_pfw_data;
				if(delay0[133:132] == 2'b10)begin
					out_pfw_valid    <= 1'h1;
					out_pfw_valid_wr <= 1'h1;
					if(in_pfw_data[133:132] == 2'b01)begin
					      pfw_state <= FIND_S;
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
				if(in_pfw_data[133:132]==2'b10)begin
					pfw_state    <= IDLE_S;
				end
				else begin
					pfw_state    <= DIC_S;
				end
			end
			
			default begin
				pfw_state    <= IDLE_S;
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