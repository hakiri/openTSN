/////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2025 FAST Group, Inc.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: FAST Group.
//Xperis URL://www.xperis.com.cn
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: pac.v
//Version: 1.0
//Author : FAST Group
//*************************************************************
//                     Module Description
//*************************************************************
// 1)transmit according to different purposes of the pkt
// 2)modify FAST_MD
// 3)structure TSN_MD
// 4)traffic regulation
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:  2019/05/11
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////

module pac(
input     wire    clk,
input     wire    rst_n,

//////////pkt and in_pac_action from pfw /////
input     wire    [133:0]in_pac_data,
input     wire    in_pac_data_wr,
input     wire    in_pac_valid,
input     wire    in_pac_valid_wr,
input     wire    [10:0]in_pac_action,
input     wire    in_pac_action_wr,

/////////pkt and tsn_md to ibm///////  
output    reg     [133:0]out_pac_data,
output    reg     out_pac_data_wr,
output    reg     out_pac_valid,
output    reg     out_pac_valid_wr,
output    reg     [23:0]out_pac_tsn_md,
output    reg     out_pac_tsn_md_wr,
input     wire    [4:0]bufm_ID_count,

////////pkt to goe ///////////////
output    reg     [133:0]out_pac2port_data2,
output    reg     out_pac2port_data_wr2,
output    reg     out_pac2port_valid2,
output    reg     out_pac2port_valid_wr2,

output    reg    [133:0]out_pac2port_data3,
output    reg    out_pac2port_data_wr3,
output    reg    out_pac2port_valid3,
output    reg    out_pac2port_valid_wr3,

//////reg from or to lcm /////////////
output    reg     [63:0]esw_pktout_cnt,
output    wire    [7:0]bufm_ID_cnt
);
/////////register///////////////////
reg     [133:0]delay0;
reg     [10:0]reg_action;
/////////state machine/////////////


/*************ping_cnt*****************/
reg [15:0] out_ping_cnt;

/*************ping_cnt*****************/

reg     [1:0]pac_state;

localparam  IDLE_S   = 2'd0,
			TRANS_S  = 2'd1,
			DIC_S    = 2'd2;

assign bufm_ID_cnt = {3'h0,bufm_ID_count};//bufm idle ID count
always @(posedge clk or negedge rst_n) begin
      if(rst_n == 1'b0)begin
        reg_action <= 11'h0;
      end
      else begin
        if(in_pac_action_wr == 1'h1)begin
            reg_action  <=  in_pac_action;
        end
        else begin
            reg_action  <= reg_action;
        end
      end
end

always @(posedge clk or negedge rst_n) begin
      if(rst_n == 1'b0)begin
		   out_pac_data      <= 134'h0;
	       out_pac_data_wr   <= 1'h0;
	       out_pac_valid     <= 1'h0;
	       out_pac_valid_wr  <= 1'h0;	  
		   out_pac_tsn_md    <= 24'h0;
		   out_pac_tsn_md_wr <= 1'h0;
	  
		   out_pac2port_data2    <= 134'h0;
	       out_pac2port_data_wr2 <= 1'h0;
	       out_pac2port_valid2   <= 1'h0;
	       out_pac2port_valid_wr2<= 1'h0;	

	       out_ping_cnt <= 16'b0;

		   pac_state	 	 <= IDLE_S;
	  end
	  else begin
		   case(pac_state)
				IDLE_S:begin//traffic regulation and TSN_MD and FAST_MD
					out_pac_data      <= 134'h0;
					out_pac_data_wr   <= 1'h0;
					out_pac_valid     <= 1'h0;
					out_pac_valid_wr  <= 1'h0;	  
					out_pac_tsn_md    <= 24'h0;
					out_pac_tsn_md_wr <= 1'h0;
					
					out_pac2port_data2    <= 134'h0;
					out_pac2port_data_wr2 <= 1'h0;
					out_pac2port_valid2   <= 1'h0;
					out_pac2port_valid_wr2<= 1'h0;
					out_pac_tsn_md 	  <= {in_pac_action[8:6],in_pac_data[107:96],in_pac_action[0],8'h0};
					if(in_pac_data_wr == 1'b1)begin	
						delay0            <= {in_pac_data[133:118],in_pac_action[5:0],in_pac_data[111:0]};	
						if( bufm_ID_count == 5'h3)begin
							if(in_pac_action[8:6] == 3'h0)begin
								pac_state      <= DIC_S;
							end
							else begin
								pac_state      <= TRANS_S;
							end
						end
						else if((bufm_ID_count == 5'h2)||(bufm_ID_count == 5'h1))begin
							if((in_pac_action[8:6] == 3'h0)||(in_pac_action[8:6] == 3'h1))begin
								pac_state      <= DIC_S;
							end
							else begin
								pac_state      <= TRANS_S;
							end				 
						end
						else if(bufm_ID_count == 5'h0)begin
							pac_state      <= DIC_S;
						end
						else begin
							pac_state      <= TRANS_S;
						end
					end
					else begin
						pac_state      <= IDLE_S;
					end
					end
				TRANS_S:begin//pkt transmit to port 0 or port 1(next module) or port 2(Local direct)
					delay0    <=  in_pac_data;
					if(reg_action[10:9] == 2'b10)begin
						out_pac_data      <= delay0;
						out_pac_data_wr   <= 1'h1;
						
						out_pac2port_data2    <= delay0;
						out_pac2port_data_wr2 <= 1'h1;
						if(delay0[133:132] == 2'b10)begin
							out_pac_valid     <= 1'h1;
							out_pac_valid_wr  <= 1'h1;
							out_pac_tsn_md_wr <= 1'h0;
							
							out_pac2port_valid2   <= 1'h1;
							out_pac2port_valid_wr2<= 1'h1;
							
							pac_state      <= IDLE_S;
						end
						else begin
							out_pac_valid     <= 1'h0;
							out_pac_valid_wr  <= 1'h0;
							out_pac_tsn_md_wr <= 1'h1;
							
							out_pac2port_valid2   <= 1'h0;
							out_pac2port_valid_wr2<= 1'h0;
							
							pac_state      <= TRANS_S;						
						end
					end
					else begin
						if(reg_action[5:0] == 6'h2)begin
							out_pac_data      <= 134'h0;
							out_pac_data_wr   <= 1'h0;
							out_pac_valid     <= 1'h0;
							out_pac_valid_wr  <= 1'h0;
							out_pac_tsn_md_wr <= 1'h0;
							
							out_pac2port_data2    <= delay0;
							out_pac2port_data_wr2 <= 1'h1;


							/**************cnt ping****************/
							if((delay0[133:132] == 2'b01)&&(delay0[79:72] == 8'hff))begin
								out_ping_cnt <= out_ping_cnt + 1'b1;
							end

							else begin
								out_ping_cnt <= out_ping_cnt;
							end

							/**************cnt ping****************/
							
							if(delay0[133:132] == 2'b10)begin															
								out_pac2port_valid2   <= 1'h1;
								out_pac2port_valid_wr2<= 1'h1;
								
								pac_state      <= IDLE_S;
							end
							else begin
								out_pac2port_valid2   <= 1'h0;
								out_pac2port_valid_wr2<= 1'h0;	

								pac_state      <= TRANS_S;
							end
						end
						else begin
							out_pac2port_data2      <= 134'h0;
							out_pac2port_data_wr2   <= 1'h0;
							out_pac2port_valid2     <= 1'h0;
							out_pac2port_valid_wr2  <= 1'h0;
							
							out_pac_data    		<= delay0;
							out_pac_data_wr 		<= 1'h1;
							if(delay0[133:132] == 2'b10)begin															
								out_pac_valid     	<= 1'h1;
								out_pac_valid_wr  	<= 1'h1;
								
								out_pac_tsn_md_wr 	<= 1'h0;
								
								pac_state      <= IDLE_S;
							end
							else begin
								out_pac_valid     	<= 1'h0;
								out_pac_valid_wr  	<= 1'h0;

								out_pac_tsn_md_wr 	<= 1'h1;	

								pac_state      <= TRANS_S;								
							end							
						end
					end
				end
				DIC_S:begin//discard pkt to port 0/1 and transmit pkt to port 2
					out_pac_data      <= 134'h0;
					out_pac_data_wr   <= 1'h0;
					out_pac_valid     <= 1'h0;
					out_pac_valid_wr  <= 1'h0;	  
					out_pac_tsn_md    <= 24'h0;
					out_pac_tsn_md_wr <= 1'h0;
					
					delay0            <= in_pac_data;
					if((reg_action[10:9] == 2'b10)||(reg_action[5:0] == 6'h2))begin
						out_pac2port_data2    <= delay0;
						out_pac2port_data_wr2 <= 1'h1;						
					end
					else begin
						out_pac2port_data2    <= 134'h0;
						out_pac2port_data_wr2 <= 1'h0;					
					end
					if(delay0[133:132] == 2'b10)begin
						out_pac2port_valid2     <= out_pac2port_data_wr2;
					    out_pac2port_valid_wr2  <= out_pac2port_data_wr2;
					
						pac_state <= IDLE_S;
					end
					else begin
						out_pac2port_valid2     <= 1'h0;
					    out_pac2port_valid_wr2  <= 1'h0;
						
						pac_state <= DIC_S;
					end
				end
		   endcase
	  end
end		

always @(posedge clk or negedge rst_n) begin
      if(rst_n == 1'b0)begin
	       out_pac2port_data3      <= 134'h0;
		   out_pac2port_data_wr3   <= 1'h0;
		   out_pac2port_valid3     <= 1'h0;
		   out_pac2port_valid_wr3  <= 1'h0;	   
	  end
	  else begin
		   case({out_pac_data_wr,out_pac2port_data_wr2})
		   2'b01:begin
	              out_pac2port_data3      <= out_pac2port_data2;
		          out_pac2port_data_wr3   <= out_pac2port_data_wr2;
		          out_pac2port_valid3     <= out_pac2port_valid2;
		          out_pac2port_valid_wr3  <= out_pac2port_valid_wr2;			          
		   end		   
		   2'b10:begin
		          out_pac2port_data3      <= out_pac_data;
				  out_pac2port_data_wr3   <= out_pac_data_wr;
				  out_pac2port_valid3     <= out_pac_valid;
		          out_pac2port_valid_wr3  <= out_pac_valid_wr;	
		   end	
		   2'b11:begin
		          out_pac2port_data3      <= out_pac2port_data2;
				  out_pac2port_data_wr3   <= out_pac2port_data_wr2;
				  out_pac2port_valid3     <= out_pac2port_valid2;
		          out_pac2port_valid_wr3  <= out_pac2port_valid_wr2;	
		   end			   
		   default:begin
	              out_pac2port_data3      <= 134'h0;
		          out_pac2port_data_wr3   <= 1'h0;
		          out_pac2port_valid3     <= 1'h0;
		          out_pac2port_valid_wr3  <= 1'h0;	   		   
		   end
		   endcase
	  end
end
////////////////pkt out count //////////////////////
always @(posedge clk or negedge rst_n) begin
      if(rst_n == 1'b0)begin
	       esw_pktout_cnt <= 64'h0;
	  end
	  else begin
	       if(out_pac2port_valid_wr3 == 1'b1)begin
		        esw_pktout_cnt <= esw_pktout_cnt + 64'h1;
		   end
		   else begin
		        esw_pktout_cnt <= esw_pktout_cnt;
		   end
	  end
end
endmodule
/**************************
pac  pac{
.clk(),
.rst_n(),

//////////pkt and in_pac_action from pfw /////
.in_pac_data(),
.in_pac_data_wr(),
.in_pac_valid(),
.in_pac_valid_wr(),
.in_pac_action(),
.in_pac_action_wr(),

/////////pkt and tsn_md to ibm///////  
.out_pac_data(),
.out_pac_data_wr(),
.out_pac_valid(),
.out_pac_valid_wr(),
.out_pac_tsn_md(),
.out_pac_tsn_md_wr(),
.bufm_ID_count(),

////////pkt to goe ///////////////
.out_pac2port_data2(),
.out_pac2port_data_wr2(),
.out_pac2port_valid2(),
.out_pac2port_valid_wr2(),

.out_pac2port_data3(),
.out_pac2port_data_wr3(),
.out_pac2port_valid3(),
.out_pac2port_valid_wr3(),

//////reg from or to lcm /////////////
.esw_pktout_cnt(),
.bufm_ID_cnt()
};
*************************/