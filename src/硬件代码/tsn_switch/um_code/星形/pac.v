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
    (*MARK_DEBUG="TRUE"*)input     wire    [133:0]in_pac_data,
    (*MARK_DEBUG="TRUE"*)input     wire    in_pac_data_wr,
    (*MARK_DEBUG="TRUE"*)input     wire    in_pac_valid,
    (*MARK_DEBUG="TRUE"*)input     wire    in_pac_valid_wr,
    (*MARK_DEBUG="TRUE"*)input     wire    [10:0]in_pac_action,
    (*MARK_DEBUG="TRUE"*)input     wire    in_pac_action_wr,
    (*MARK_DEBUG="TRUE"*)
    /////////pkt and tsn_md to ibm0///////  
   (*MARK_DEBUG="TRUE"*)output    reg     [133:0]out_pac_data_0,
   (*MARK_DEBUG="TRUE"*)output    reg     out_pac_data_wr_0,
   (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_0,
   (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_wr_0,
   (*MARK_DEBUG="TRUE"*)output    reg     [23:0]out_pac_tsn_md_0,
   (*MARK_DEBUG="TRUE"*)output    reg     out_pac_tsn_md_wr_0,
   (*MARK_DEBUG="TRUE"*)input     wire    [4:0]bufm_ID_count_0,

    (*MARK_DEBUG="TRUE"*)output    reg     [133:0]out_pac_data_1,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_data_wr_1,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_1,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_wr_1,
    (*MARK_DEBUG="TRUE"*)output    reg     [23:0]out_pac_tsn_md_1,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_tsn_md_wr_1,
    (*MARK_DEBUG="TRUE"*)input     wire    [4:0]bufm_ID_count_1,
	
    (*MARK_DEBUG="TRUE"*)output    reg     [133:0]out_pac_data_2,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_data_wr_2,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_2,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_wr_2,
    (*MARK_DEBUG="TRUE"*)output    reg     [23:0]out_pac_tsn_md_2,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_tsn_md_wr_2,
    (*MARK_DEBUG="TRUE"*)input     wire    [4:0]bufm_ID_count_2,
	
    (*MARK_DEBUG="TRUE"*)output    reg     [133:0]out_pac_data_3,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_data_wr_3,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_3,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_valid_wr_3,
    (*MARK_DEBUG="TRUE"*)output    reg     [23:0]out_pac_tsn_md_3,
    (*MARK_DEBUG="TRUE"*)output    reg     out_pac_tsn_md_wr_3,
    (*MARK_DEBUG="TRUE"*)input     wire    [4:0]bufm_ID_count_3,
    
    //////reg from or to lcm /////////////
    input     wire    reg_tap,
    output    reg     [63:0]esw_pktout_cnt_0,
	output    reg     [63:0]esw_pktout_cnt_1,
	output    reg     [63:0]esw_pktout_cnt_2,
	output    reg     [63:0]esw_pktout_cnt_3,
    
    (*MARK_DEBUG="TRUE"*)output    wire    [7:0]bufm_ID_cnt_0,
	(*MARK_DEBUG="TRUE"*)output    wire    [7:0]bufm_ID_cnt_1,
	(*MARK_DEBUG="TRUE"*)output    wire    [7:0]bufm_ID_cnt_2,
	(*MARK_DEBUG="TRUE"*)output    wire    [7:0]bufm_ID_cnt_3
    );
    assign bufm_ID_cnt_0 = {3'h0,bufm_ID_count_0};
	assign bufm_ID_cnt_1 = {3'h0,bufm_ID_count_1};
	assign bufm_ID_cnt_2 = {3'h0,bufm_ID_count_2};
	assign bufm_ID_cnt_3 = {3'h0,bufm_ID_count_3};
    /////////register///////////////////
    reg     [133:0]delay0;
    reg     [10:0]reg_action;
    /*************ping_cnt*****************/
    
    (*MARK_DEBUG="TRUE"*)reg     [2:0]pac_state;
    
    localparam  IDLE_S   = 3'd0,
				DRC_S    = 3'd1,
                TRANS_S  = 3'd2,
                TAP_S    = 3'd3,
                DIC_S    = 3'd4;

always @(posedge clk or negedge rst_n) begin
          if(rst_n == 1'b0)begin
               out_pac_data_0      <= 134'h0;
               out_pac_data_wr_0   <= 1'h0;
               out_pac_valid_0     <= 1'h0;
               out_pac_valid_wr_0  <= 1'h0;	
               out_pac_tsn_md_0    <= 24'h0;
               out_pac_tsn_md_wr_0 <= 1'h0;	  
				
			   out_pac_data_1      <= 134'h0;
               out_pac_data_wr_1   <= 1'h0;
               out_pac_valid_1     <= 1'h0;
               out_pac_valid_wr_1  <= 1'h0;	
               out_pac_tsn_md_1    <= 24'h0;
               out_pac_tsn_md_wr_1 <= 1'h0;	
			   
			   out_pac_data_2      <= 134'h0;
               out_pac_data_wr_2   <= 1'h0;
               out_pac_valid_2     <= 1'h0;
               out_pac_valid_wr_2  <= 1'h0;	
               out_pac_tsn_md_2    <= 24'h0;
               out_pac_tsn_md_wr_2 <= 1'h0;	
			   
			   out_pac_data_3      <= 134'h0;
               out_pac_data_wr_3   <= 1'h0;
               out_pac_valid_3     <= 1'h0;
               out_pac_valid_wr_3  <= 1'h0;	
               out_pac_tsn_md_3    <= 24'h0;
               out_pac_tsn_md_wr_3 <= 1'h0;	
               reg_action          <= 11'h0;
               
               pac_state	 	 <= IDLE_S;   
          end
          else begin
               case(pac_state)
                    IDLE_S:begin
                        out_pac_data_0      <= 134'h0;
						out_pac_data_wr_0   <= 1'h0;
						out_pac_valid_0     <= 1'h0;
						out_pac_valid_wr_0  <= 1'h0;	
						out_pac_tsn_md_0    <= 24'h0;
						out_pac_tsn_md_wr_0 <= 1'h0;	 
						
						out_pac_data_1      <= 134'h0;
						out_pac_data_wr_1   <= 1'h0;
						out_pac_valid_1     <= 1'h0;
						out_pac_valid_wr_1  <= 1'h0;	
						out_pac_tsn_md_1    <= 24'h0;
						out_pac_tsn_md_wr_1 <= 1'h0;	
						
						out_pac_data_2      <= 134'h0;
						out_pac_data_wr_2   <= 1'h0;
						out_pac_valid_2     <= 1'h0;
						out_pac_valid_wr_2  <= 1'h0;	
						out_pac_tsn_md_2    <= 24'h0;
						out_pac_tsn_md_wr_2 <= 1'h0;	
						
						out_pac_data_3      <= 134'h0;
						out_pac_data_wr_3   <= 1'h0;
						out_pac_valid_3     <= 1'h0;
						out_pac_valid_wr_3  <= 1'h0;	
						out_pac_tsn_md_3    <= 24'h0;
						out_pac_tsn_md_wr_3 <= 1'h0;	

                        if(in_pac_data_wr == 1'b1)begin	
							pac_state      <= DRC_S;
							reg_action  <=  in_pac_action;
                            delay0          <= {in_pac_data[133:118],in_pac_action[5:0],in_pac_data[111:0]};					
                        end
                        else begin
                            reg_action  <=  reg_action;
                            pac_state      <= IDLE_S;
                        end
                    end
					
                    DRC_S:begin
						out_pac_valid_0     <= 1'h0;
						out_pac_valid_wr_0  <= 1'h0;
						out_pac_valid_1     <= 1'h0;
						out_pac_valid_wr_1  <= 1'h0;
						out_pac_valid_2     <= 1'h0;
						out_pac_valid_wr_2  <= 1'h0;
						out_pac_valid_3     <= 1'h0;
						out_pac_valid_wr_3  <= 1'h0;
						
						if(in_pac_data_wr == 1'b1)begin
							delay0    <= in_pac_data;
							out_pac_tsn_md_0 	<= {reg_action[8:6],delay0[107:96],reg_action[0],8'h0};
							out_pac_tsn_md_1 	<= {reg_action[8:6],delay0[107:96],reg_action[0],8'h0};
							out_pac_tsn_md_2 	<= {reg_action[8:6],delay0[107:96],reg_action[0],8'h0};
							out_pac_tsn_md_3 	<= {reg_action[8:6],delay0[107:96],reg_action[0],8'h0};
							if(reg_action[10:9] == 2'b00)begin
								case(reg_action[5:0])
                                    6'b000001:begin
                                        if( bufm_ID_count_0 == 5'h3)begin
                                            if(reg_action[8:6] == 3'h0)begin
                                                out_pac_data_0      <= 134'h0;
                                                out_pac_data_wr_0   <= 1'h0;		  

                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_0      <= delay0;
												out_pac_data_wr_0   <= 1'h1;													
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end
                                        end
                                        else if((bufm_ID_count_0 == 5'h2)||(bufm_ID_count_0 == 5'h1))begin
                                            if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
                                                out_pac_data_0      <= 134'h0;
                                                out_pac_data_wr_0   <= 1'h0;
                                                                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_0      <= delay0;
												out_pac_data_wr_0   <= 1'h1;												
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end				 
                                        end
                                        else if(bufm_ID_count_0 == 5'h0)begin
                                            out_pac_data_0      <= 134'h0;
                                            out_pac_data_wr_0   <= 1'h0;
                                                
                                            pac_state      <= DIC_S;
                                        end
                                        else begin
											out_pac_data_0      <= delay0;
											out_pac_data_wr_0   <= 1'h1;
                                            if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                            end
                                            else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                            end
                                        end
                                    end
                                    6'b000010:begin
                                        if( bufm_ID_count_1 == 5'h3)begin
                                            if(reg_action[8:6] == 3'h0)begin
                                                out_pac_data_1      <= 134'h0;
                                                out_pac_data_wr_1   <= 1'h0;
                                                                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_1      <= delay0;
												out_pac_data_wr_1   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end
                                        end
                                        else if((bufm_ID_count_1 == 5'h2)||(bufm_ID_count_1 == 5'h1))begin
                                            if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
                                                out_pac_data_1      <= 134'h0;
                                                out_pac_data_wr_1   <= 1'h0;
                                                                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_1      <= delay0;
												out_pac_data_wr_1   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end				 
                                        end
                                        else if(bufm_ID_count_1 == 5'h0)begin
                                            out_pac_data_1      <= 134'h0;
                                            out_pac_data_wr_1   <= 1'h0;
                                                                                          
                                            pac_state      <= DIC_S;
                                        end
                                        else begin
											out_pac_data_1      <= delay0;
											out_pac_data_wr_1   <= 1'h1;
                                            if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                            end
                                            else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                            end
                                        end
                                    end
                                    6'b000100:begin
                                        if( bufm_ID_count_2 == 5'h3)begin
                                            if(reg_action[8:6] == 3'h0)begin
                                                out_pac_data_2      <= 134'h0;
                                                out_pac_data_wr_2   <= 1'h0;
                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_2      <= delay0;
												out_pac_data_wr_2   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end
                                        end
                                        else if((bufm_ID_count_2 == 5'h2)||(bufm_ID_count_2 == 5'h1))begin
                                            if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
                                                out_pac_data_2      <= 134'h0;
                                                out_pac_data_wr_2   <= 1'h0;
                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_2      <= delay0;
											    out_pac_data_wr_2   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end				 
                                        end
                                        else if(bufm_ID_count_2 == 5'h0)begin
                                            out_pac_data_2      <= 134'h0;
                                            out_pac_data_wr_2   <= 1'h0;
                                            
                                            pac_state      <= DIC_S;
                                        end
                                        else begin
											out_pac_data_2      <= delay0;
											out_pac_data_wr_2   <= 1'h1;
                                            if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                            end
                                            else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                            end
                                        end							
                                    end
                                    6'b001000:begin
                                        if( bufm_ID_count_3 == 5'h3)begin
                                            if(reg_action[8:6] == 3'h0)begin
                                                out_pac_data_3      <= 134'h0;
                                                out_pac_data_wr_3   <= 1'h0;
                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_3      <= delay0;
												out_pac_data_wr_3   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end
                                        end
                                        else if((bufm_ID_count_3 == 5'h2)||(bufm_ID_count_3 == 5'h1))begin
                                            if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
                                                out_pac_data_3      <= 134'h0;
                                                out_pac_data_wr_3   <= 1'h0;
                                                
                                                pac_state      <= DIC_S;
                                            end
                                            else begin
												out_pac_data_3      <= delay0;
												out_pac_data_wr_3   <= 1'h1;
                                                if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                                end
                                                else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                                end
                                            end				 
                                        end
                                        else if(bufm_ID_count_3 == 5'h0)begin
                                            out_pac_data_3      <= 134'h0;
                                            out_pac_data_wr_3   <= 1'h0;
                                            
                                            pac_state      <= DIC_S;
                                        end
                                        else begin
											out_pac_data_3      <= delay0;
											out_pac_data_wr_3   <= 1'h1;
                                            if(reg_tap == 1'b0)begin
                                                    pac_state      <= TRANS_S;
                                            end
                                            else begin
													out_pac_data_3      <= delay0;
													out_pac_data_wr_3   <= 1'h1;
													
                                                    pac_state      <= TAP_S;
                                            end
                                        end							
                                    end
								endcase
							end
							else begin //广播和组播的处理还未完善
								if(reg_tap == 1'b0)begin
									pac_state      <= TRANS_S;
								end
								else begin
									out_pac_data_3      <= delay0;
									out_pac_data_wr_3   <= 1'h1;
											
									pac_state      <= TAP_S;
								end
								
								if( bufm_ID_count_0 == 5'h3)begin
									if(reg_action[8:6] == 3'h0)begin										
										out_pac_data_0      <= 134'h0;
                                        out_pac_data_wr_0   <= 1'h0;
										reg_action[0]       <= 1'h0;
									end
									else begin
										out_pac_data_0      <= delay0;
										out_pac_data_wr_0   <= reg_action[0];
									end
								end
								else if((bufm_ID_count_0 == 5'h2)||(bufm_ID_count_0 == 5'h1))begin
									if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
										out_pac_data_0      <= 134'h0;
                                        out_pac_data_wr_0   <= 1'h0;
										reg_action[0]       <= 1'h0;
									end
									else begin
										out_pac_data_0      <= delay0;
										out_pac_data_wr_0   <= reg_action[0];
									end				 
								end
								else if(bufm_ID_count_0 == 5'h0)begin
									out_pac_data_0      <= 134'h0;
                                    out_pac_data_wr_0   <= 1'h0;
									reg_action[0]       <= 1'h0;
								end
								else begin
									out_pac_data_0      <= delay0;
									out_pac_data_wr_0   <= reg_action[0];
								end
								
								if( bufm_ID_count_1 == 5'h3)begin
									if(reg_action[8:6] == 3'h0)begin
										out_pac_data_1      <= 134'h0;
                                        out_pac_data_wr_1   <= 1'h0;
										reg_action[1]       <= 1'h0;
									end
									else begin
										out_pac_data_1      <= delay0;
										out_pac_data_wr_1   <= reg_action[1];
									end
								end
								else if((bufm_ID_count_1 == 5'h2)||(bufm_ID_count_1 == 5'h1))begin
									if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
										out_pac_data_1      <= 134'h0;
                                        out_pac_data_wr_1   <= 1'h0;
										reg_action[1]       <= 1'h0;
									end
									else begin
										out_pac_data_1      <= delay0;
										out_pac_data_wr_1   <= reg_action[1];
									end				 
								end
								else if(bufm_ID_count_1 == 5'h0)begin
									out_pac_data_1      <= 134'h0;
                                    out_pac_data_wr_1   <= 1'h0;
									reg_action[1]       <= 1'h0;
								end
								else begin
									out_pac_data_1      <= delay0;
									out_pac_data_wr_1   <= reg_action[1];
								end
								
								if( bufm_ID_count_2 == 5'h3)begin
									if(reg_action[8:6] == 3'h0)begin
										out_pac_data_2      <= 134'h0;
                                        out_pac_data_wr_2   <= 1'h0;
										reg_action[2]       <= 1'h0;
									end
									else begin
										out_pac_data_2      <= delay0;
										out_pac_data_wr_2   <= reg_action[2];
									end
								end
								else if((bufm_ID_count_2 == 5'h2)||(bufm_ID_count_2 == 5'h1))begin
									if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
										out_pac_data_2      <= 134'h0;
                                        out_pac_data_wr_2   <= 1'h0;
										reg_action[2]       <= 1'h0;
									end
									else begin
										out_pac_data_2      <= delay0;
										out_pac_data_wr_2   <= reg_action[2];
									end				 
								end
								else if(bufm_ID_count_2 == 5'h0)begin
									out_pac_data_2      <= 134'h0;
                                    out_pac_data_wr_2   <= 1'h0;
									reg_action[2]       <= 1'h0;
								end
								else begin
									out_pac_data_2      <= delay0;
									out_pac_data_wr_2   <= reg_action[2];
								end
								
								if( bufm_ID_count_3 == 5'h3)begin
									if(reg_action[8:6] == 3'h0)begin
										out_pac_data_3      <= 134'h0;
                                        out_pac_data_wr_3   <= 1'h0;
										reg_action[3]       <= 1'h0;
									end
									else begin
										out_pac_data_3      <= delay0;
										out_pac_data_wr_3   <= reg_action[3];
									end
								end
								else if((bufm_ID_count_3 == 5'h2)||(bufm_ID_count_3 == 5'h1))begin
									if((reg_action[8:6] == 3'h0)||(reg_action[8:6] == 3'h1))begin
										out_pac_data_3      <= 134'h0;
                                        out_pac_data_wr_3   <= 1'h0;
										reg_action[3]       <= 1'h0;
									end
									else begin
										out_pac_data_3      <= delay0;
										out_pac_data_wr_3   <= reg_action[3];
									end				 
								end
								else if(bufm_ID_count_3 == 5'h0)begin
									out_pac_data_3      <= 134'h0;
                                    out_pac_data_wr_3   <= 1'h0;
                                    reg_action[3]       <= 1'h0;
								end
								else begin
								    out_pac_data_3      <= delay0;
								    out_pac_data_wr_3   <= reg_action[3];
                                end
							end
						end
						else begin
							pac_state	 	 <= DRC_S;   
						end
					end
					
                    TRANS_S:begin
						out_pac_data_0	 	<= delay0;
						out_pac_data_1	 	<= delay0;
						out_pac_data_2	 	<= delay0;
						out_pac_data_3	 	<= delay0;
						out_pac_data_wr_0  <= reg_action[0];
						out_pac_data_wr_1  <= reg_action[1];
						out_pac_data_wr_2  <= reg_action[2];
						out_pac_data_wr_3  <= reg_action[3];
						
						if(delay0[133:132] == 2'b10)begin
							out_pac_valid_0     <= reg_action[0];
							out_pac_valid_wr_0  <= reg_action[0];
							
							out_pac_valid_1     <= reg_action[1];
							out_pac_valid_wr_1  <= reg_action[1];
							
							out_pac_valid_2     <= reg_action[2];
							out_pac_valid_wr_2  <= reg_action[2];
							
							out_pac_valid_3     <= reg_action[3];
							out_pac_valid_wr_3  <= reg_action[3];
							
							
							out_pac_tsn_md_wr_0 <= 1'h0;
							out_pac_tsn_md_wr_1 <= 1'h0;
							out_pac_tsn_md_wr_2 <= 1'h0;
							out_pac_tsn_md_wr_3 <= 1'h0;
							if(in_pac_data[133:132]==2'b01)begin
								reg_action  <=  in_pac_action;
								delay0      <= {in_pac_data[133:118],in_pac_action[5:0],in_pac_data[111:0]};
								pac_state      <= DRC_S;
							end
							else begin
								pac_state      <= IDLE_S;
							end
						end
						else begin
						    delay0    <=  in_pac_data;
							out_pac_valid_0       <= 1'h0;
							out_pac_valid_wr_0    <= 1'h0;
							out_pac_valid_1       <= 1'h0;
							out_pac_valid_wr_1    <= 1'h0;
							out_pac_valid_2       <= 1'h0;
							out_pac_valid_wr_2    <= 1'h0;
							out_pac_valid_3       <= 1'h0;
							out_pac_valid_wr_3    <= 1'h0;
							
							out_pac_tsn_md_wr_0   <= reg_action[0];
							out_pac_tsn_md_wr_1   <= reg_action[1];
							out_pac_tsn_md_wr_2   <= reg_action[2];
							out_pac_tsn_md_wr_3   <= reg_action[3];
							
							pac_state      <= TRANS_S;
						end
					end	

                    TAP_S:begin
						out_pac_data_0	 	<= delay0;
						out_pac_data_1	 	<= delay0;
						out_pac_data_2	 	<= delay0;
						out_pac_data_3	 	<= delay0;
						out_pac_data_wr_0  <= reg_action[0];
						out_pac_data_wr_1  <= reg_action[1];
						out_pac_data_wr_2  <= reg_action[2];
						out_pac_data_wr_3  <= 1'b1;
						
						if(delay0[133:132] == 2'b10)begin
							out_pac_valid_0     <= reg_action[0];
							out_pac_valid_wr_0  <= reg_action[0];
							
							out_pac_valid_1     <= reg_action[1];
							out_pac_valid_wr_1  <= reg_action[1];
							
							out_pac_valid_2     <= reg_action[2];
							out_pac_valid_wr_2  <= reg_action[2];
							
							out_pac_valid_3     <= 1'h1;
							out_pac_valid_wr_3  <= 1'h1;
							
							
							out_pac_tsn_md_wr_0 <= 1'h0;
							out_pac_tsn_md_wr_1 <= 1'h0;
							out_pac_tsn_md_wr_2 <= 1'h0;
							out_pac_tsn_md_wr_3 <= 1'h0;
							if(in_pac_data[133:132]==2'b01)begin
								reg_action  <=  in_pac_action;
								delay0      <= {in_pac_data[133:118],in_pac_action[5:0],in_pac_data[111:0]};
								
								pac_state      <= DRC_S;
							end
							else begin
								pac_state      <= IDLE_S;
							end
						end
						else begin
						    delay0    <=  in_pac_data;
							out_pac_valid_0       <= 1'h0;
							out_pac_valid_wr_0    <= 1'h0;
							out_pac_valid_1       <= 1'h0;
							out_pac_valid_wr_1    <= 1'h0;
							out_pac_valid_2       <= 1'h0;
							out_pac_valid_wr_2    <= 1'h0;
							out_pac_valid_3       <= 1'h0;
							out_pac_valid_wr_3    <= 1'h0;
							
							out_pac_tsn_md_wr_0   <= reg_action[0];
							out_pac_tsn_md_wr_1   <= reg_action[1];
							out_pac_tsn_md_wr_2   <= reg_action[2];
							out_pac_tsn_md_wr_3   <= 1'h1;
							
							pac_state      <= TAP_S;
						end
					end

                    DIC_S:begin
                        if(in_pac_data[133:132]==2'b10)begin
								pac_state      <= IDLE_S;
                        end
                        else begin
                            pac_state      <= DIC_S;
                        end
                    end
                    
                    default:begin
                        pac_state      <= IDLE_S;
                    end
               endcase
          end
    end
    ////////////////pkt out count //////////////////////
    always @(posedge clk or negedge rst_n) begin
          if(rst_n == 1'b0)begin
               esw_pktout_cnt_0 <= 64'h0;
               esw_pktout_cnt_1 <= 64'h0;
               esw_pktout_cnt_2 <= 64'h0;
               esw_pktout_cnt_3 <= 64'h0;
          end
          else begin
               if(out_pac_valid_wr_0 == 1'b1)begin
                    esw_pktout_cnt_0 <= esw_pktout_cnt_0 + 64'h1;
               end
               else begin
                    esw_pktout_cnt_0 <= esw_pktout_cnt_0;
               end
               
               if(out_pac_valid_wr_1 == 1'b1)begin
                    esw_pktout_cnt_1 <= esw_pktout_cnt_1 + 64'h1;
               end
               else begin
                    esw_pktout_cnt_1 <= esw_pktout_cnt_1;
               end
               
               if(out_pac_valid_wr_2 == 1'b1)begin
                    esw_pktout_cnt_2 <= esw_pktout_cnt_2 + 64'h1;
               end
               else begin
                    esw_pktout_cnt_1 <= esw_pktout_cnt_2;
               end
               
               if(out_pac_valid_wr_3 == 1'b1)begin
                    esw_pktout_cnt_3 <= esw_pktout_cnt_3 + 64'h1;
               end
               else begin
                    esw_pktout_cnt_3 <= esw_pktout_cnt_3;
               end
          end
    end
    endmodule
    /**************************
    pac  pac{
    .clk(),
    .rst_n(),
    
    //////////pkt and reg_action from pfw /////
    .in_pac_data(),
    .in_pac_data_wr(),
    .in_pac_valid(),
    .in_pac_valid_wr(),
    .in_pac_action(),
    .in_pac_action_wr(),
    
    /////////pkt and tsn_md to ibm0///////  
    .out_pac_data_0(),
    .out_pac_data_wr_0(),
    .out_pac_valid_0(),
    .out_pac_valid_wr_0(),
    .out_pac_tsn_md_0(),
    .out_pac_tsn_md_wr_0(),
    .bufm_ID_count_0(),
    
    /////////pkt and tsn_md to ibm1///////  
    .out_pac_data_1(),
    .out_pac_data_wr_1(),
    .out_pac_valid_1(),
    .out_pac_valid_wr_1(),
    .out_pac_tsn_md_1(),
    .out_pac_tsn_md_wr_1(),
    .bufm_ID_count_1(),
    
    /////////pkt and tsn_md to ibm2///////  
    .out_pac_data_2(),
    .out_pac_data_wr_2(),
    .out_pac_valid_2(),
    .out_pac_valid_wr_2(),
    .out_pac_tsn_md_2(),
    .out_pac_tsn_md_wr_2(),
    .bufm_ID_count_2(),
    
    /////////pkt and tsn_md to ibm3///////  
    .out_pac_data_3(),
    .out_pac_data_wr_3(),
    .out_pac_valid_3(),
    .out_pac_valid_wr_3(),
    .out_pac_tsn_md_3(),
    .out_pac_tsn_md_wr_3(),
    .bufm_ID_count_3(),
    
    //////reg from or to lcm /////////////
    .reg_tap(),
    .esw_pktout_cnt_0(),
    .esw_pktout_cnt_1(),
    .esw_pktout_cnt_2(),
    .esw_pktout_cnt_3(),
    .bufm_ID_cnt_0(),
    .bufm_ID_cnt_1(),
    .bufm_ID_cnt_2(),
    .bufm_ID_cnt_3()
    };
    *************************/