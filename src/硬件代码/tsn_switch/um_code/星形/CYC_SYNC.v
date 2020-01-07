//===============================================================//
//	Module name: Master-clock module for precise time synchronization (USG);
//	Communication with fuwenwen(fuwenwen94@163.com)
//	Last edited time: 2018/12/30
//	Function outline: ptp_v0.1
//===============================================================//

//	Function outline: ptp_v0.1
//===============================================================//

`timescale 1ns/1ps

module	CYC_SYNC(
	clk,
	reset,
	m_or_s,
	status_ok,
	ts_3_valid,
	ts_3,
	timer,
	send_sync_pkt,
	send_test_pkt,
	ts_2_record,
	ts_1_valid,
	ts_1,
	ts_4_valid,
	ts_4,
	offset,
	error1,
	cyc_init,
//	cyc_init_ack,
	temp_cnt,
	sync_cnt
);

 reg [31:0]send_sync_cnt;  
       
always @(posedge clk or negedge reset) begin
   if(reset == 1'b0) begin
           send_sync_cnt<=32'b0;
   end
   else begin
          if(send_sync_pkt==1'b1)begin
              send_sync_cnt<=send_sync_cnt+32'b1;
          end      
          else begin
              send_sync_cnt<=send_sync_cnt;  
          end     
    end      
end

	input 	clk;
	input 	reset;
	input 	m_or_s;
	input 	status_ok;
	input 	ts_3_valid;
	input 	[47:0] ts_3;
	output	reg	[47:0]	timer;
	output	reg	send_sync_pkt;
	output	reg	send_test_pkt;
	

	input	ts_2_record;
	input	ts_1_valid;
	input	[47:0]	ts_1;
	input	ts_4_valid;
	input	[47:0]	ts_4;
	output	reg error1;
	output	reg cyc_init;
	//(*mark_debug="TRUE"*)input	 cyc_init_ack;
	output	reg	[47:0]	temp_cnt;
	input [31:0]sync_cnt;
	reg [31:0]sync_cmp_cnt;
	
	
	(*MARK_DEBUG="TRUE"*)reg [47:0]	ts_2_reg,ts_1_reg,ts_3_reg,ts_4_reg;
	(*MARK_DEBUG="TRUE"*)output reg	[47:0]	offset;
	reg	[30:0]	ms_cnt;
	reg	[16:0]	cyc_cnt;
	reg			cyc_valid, temp_cnt_valid;
	reg [1:0]	offset_tag;
	 reg [47:0]clc_offset;
	(*MARK_DEBUG="TRUE"*)reg clc_ok,offset_ok;
	
//	reg [47:0]md_timestamp;   


reg [1:0]cyc_done;
reg [1:0] syn_state;
localparam  IDLE_S    = 2'd0,
            SYNC_S    = 2'd1,
            RELEASE_S = 2'd2;
always @(posedge clk or negedge reset) begin
        if (!reset) begin
			cyc_init	<= 1'b0;
			cyc_done   <=2'b11;
	    end
		else begin
		    if(cyc_valid==1'b1 || temp_cnt_valid==1'b1)begin
				cyc_init	<= 1'b1;
				cyc_done   <=2'b0;
			end
			else if(&cyc_done)begin
				cyc_init	<= 1'b0;
			end
			else begin
			    cyc_init	<=cyc_init;
			    cyc_done   <=cyc_done+1'b1;
			end
		
		end
end
/*
sync_sig sync_ack(
    .clk(clk),
    .rst_n(reset),
    
    .in_sig(cyc_init_ack),
    .out_sig(cyc_done)
);*/


always @(posedge clk or negedge reset) begin
	if (!reset) begin
		send_sync_pkt <= 1'd0;
		send_test_pkt	<= 1'b0;
		send_sync_cnt<=16'b0;
	end
	else begin
		if((sync_cmp_cnt==32'd512) && (m_or_s == 1'b1)) begin
			send_sync_pkt	<= 	1'd1;
		end
		else send_sync_pkt	<= 	1'd0;
		
		if(ms_cnt[9:0] == 10'h3ff &&cyc_cnt==17'd0 ) begin
			send_test_pkt	<= 	1'd0;
		end
		else send_test_pkt	<= 	1'd0;
	end
end


always @(posedge clk or negedge reset) begin
	if (!reset) begin
		ms_cnt	<=	31'b0;
		cyc_cnt	<=	17'b0;
		
		timer <= 48'd0;
		cyc_valid<=1'b0;
		sync_cmp_cnt<=32'b0;
	end
	else begin
	     if(sync_cmp_cnt==32'h1e847)begin
		      cyc_valid <= 1'b1;	
			  sync_cmp_cnt<=32'b0;
	     end
	     else begin
	       cyc_valid	<= 1'b0;
		   sync_cmp_cnt<=sync_cmp_cnt+32'b1;
		 end  
		   
		timer<={ms_cnt,cyc_cnt};
		if(temp_cnt_valid ==1'b0)begin	
		    if(cyc_cnt == 17'd124999) begin
		    	ms_cnt	<=	ms_cnt + 1;
		    	cyc_cnt	<=	17'b0;
		    		    	
		    end
		    else begin
		    	ms_cnt	<=	ms_cnt;
		    	cyc_cnt	<=	cyc_cnt+1'b1;
		    	
		    end
		end
		else begin 
		    ms_cnt	<=	temp_cnt[47:17];
		    cyc_cnt	<=	temp_cnt[16:0]+1'b1;		
		end
	end
end
reg	[47:0]	clc_timer;
always @(posedge clk or negedge reset) begin
	if (!reset) begin
		error1	<= 1'b0;
		temp_cnt	<= 48'b0;
		clc_offset<=48'b0;
		offset<=48'b0;
		temp_cnt_valid <= 1'b0;
		offset_tag	<= 2'b0;
	    clc_ok<=1'b0;
	    offset_ok<=1'b0;
	    ts_1_reg<=48'b0;
	    ts_2_reg<=48'b0;
	    ts_3_reg<=48'b0;
	    ts_4_reg<=48'b0;
		clc_timer<=48'b0;

	end
	else begin
	
		
		
		if(ts_2_record) ts_2_reg <= timer;
		else ts_2_reg <= ts_2_reg;
		
		if(ts_1_valid) ts_1_reg <= ts_1;
		else ts_1_reg <= ts_1_reg;
		
		if(ts_3_valid) ts_3_reg <= ts_3;
		else ts_3_reg <= ts_3_reg;
		
		if(ts_4_valid) ts_4_reg <= ts_4;
		else ts_4_reg <= ts_4_reg;
		
		if(status_ok) begin
		    clc_ok<=1'b1;
			if((ts_2_reg + ts_3_reg +49'b0)>=(ts_1_reg + ts_4_reg+49'b0)) begin				
				if((ts_2_reg[16:0] + ts_3_reg[16:0]+18'b0) >=( ts_1_reg[16:0] +ts_4_reg[16:0]+18'b0))begin
				clc_offset[16:0]	<= (ts_2_reg[16:0] + ts_3_reg[16:0] - ts_1_reg[16:0] - ts_4_reg[16:0]);
				clc_offset[47:17]	<= (ts_2_reg[47:17] + ts_3_reg[47:17] - ts_1_reg[47:17] - ts_4_reg[47:17]);
				end
				else begin
				clc_offset[47:17]	<= (ts_2_reg[47:17] + ts_3_reg[47:17] - ts_1_reg[47:17] - ts_4_reg[47:17]-17'b1);
				clc_offset[16:0]	<= (ts_2_reg[16:0] + ts_3_reg[16:0]+18'd125000 - ts_1_reg[16:0] - ts_4_reg[16:0]);
				end
				offset_tag	<= 2'b1;			
			end
			else begin
			    if(ts_4_reg[16:0] + ts_1_reg[16:0]+18'b0 >= ts_2_reg[16:0] +ts_3_reg[16:0]+18'b0)begin
				clc_offset[16:0]	<= (ts_4_reg[16:0] + ts_1_reg[16:0] - ts_2_reg[16:0] - ts_3_reg[16:0]);
				clc_offset[47:17]	<= (ts_4_reg[47:17] + ts_1_reg[47:17] - ts_2_reg[47:17] - ts_3_reg[47:17]);
				end
				else begin
				clc_offset[47:17]	<= (ts_4_reg[47:17] + ts_1_reg[47:17] - ts_2_reg[47:17] - ts_3_reg[47:17]-17'b1);
				clc_offset[16:0]	<= (ts_4_reg[16:0] + ts_1_reg[16:0]+18'd125000 - ts_2_reg[16:0] - ts_3_reg[16:0]);
				end			
				offset_tag	<= 2'd2;								
			end
		end
		else begin				     
			  clc_offset	<=	clc_offset;
			   clc_ok<=1'b0;
			  temp_cnt <= temp_cnt;
		end
		
		if( clc_ok==1'b1)begin
	        offset_ok<=1'b1;
			clc_timer<=timer;
	        if(clc_offset[17]==1'b1)begin
	          offset[47:17]<=(clc_offset[47:17]-31'b1)>>1;
	          offset[16:0]<=(clc_offset[16:0]>>1)+17'hF424;
	        end
	        else begin
	           offset[47:17]<=clc_offset[47:17]>>1;
	           offset[16:0]<=clc_offset[16:0]>>1;
	        end	
		    if((clc_offset[47:17]>>1) >= 1'b1)begin error1 <= 1'b1;end
	        else begin error1 <= 1'b0;end	    
	     end
	      else begin
	        offset<=offset;
	        offset_ok<=1'b0;
	     end
		
		if((offset_ok==1'b1) && (offset_tag == 2'b1)) begin
		   if((clc_timer[16:0]>=offset[16:0]))begin
				temp_cnt<={clc_timer[47:17]-offset[47:17],(clc_timer[16:0]-offset[16:0]+17'd3)};
				end
				else begin
				temp_cnt<={clc_timer[47:17]-offset[47:17]-1'b1,(clc_timer[16:0]+17'd125000-offset[16:0]+17'd3)};
				end
			temp_cnt_valid <= 1'b1;
			offset_tag <= 2'b0;
			
		end
		else if((offset_ok==1'b1)&&(offset_tag == 2'd2)) begin
		    if(clc_timer[16:0]+offset[16:0]+18'b0>=18'd125000)begin
				temp_cnt	<=	{clc_timer[47:17]+offset[47:17]+1,clc_timer[16:0]+offset[16:0]-17'd125000+17'd3};
			end
				else begin
				temp_cnt	<=	{clc_timer[47:17]+offset[47:17],clc_timer[16:0]+offset[16:0]+17'd3};
			end
			temp_cnt_valid <= 1'b1;
			offset_tag <= 2'b0;
		end
		 else begin
	        if(cyc_valid)begin
				temp_cnt[47:17]<=ms_cnt;
		    	temp_cnt[16:0]<=cyc_cnt;
				temp_cnt_valid <= 1'b0;
		      end
		      else begin		     	
			    temp_cnt_valid	<= 1'b0;
			    temp_cnt[47:0]  <= temp_cnt[47:0];
			    
			  end
		 end
		
		
	end
end	
endmodule