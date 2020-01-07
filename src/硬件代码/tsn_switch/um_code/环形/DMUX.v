module DMUX(
    input wire clk,
	input wire rst_n,
	
	input wire pktin_data_wr,
	input wire [133:0] pktin_data,
	input wire pktin_data_valid,
	input wire pktin_data_valid_wr,
	output pktin_ready,
	
	output reg parser2rx_data_wr,
	output reg [133:0] parser2rx_data,
	output reg parser2rx_data_valid,
	output reg parser2rx_data_valid_wr,
	input wire rx2parser_data_alf,
	
	output reg parser2mux_data_wr,
	output reg [133:0] parser2mux_data,
	output reg parser2mux_data_valid,
	output reg parser2mux_data_valid_wr,
	input wire mux2parser_data_alf,
	
	input [47:0]device_mac,
    input [1:0]device_role
);

reg [7:0] pkt_step_cnt;
reg [133:0] md_reg0;
reg [133:0] md_reg1;
reg [2:0] parser_state;
wire parser_alf;

assign parser_alf = rx2parser_data_alf | mux2parser_data_alf;
assign pktin_ready = ~ parser_alf;
reg [31:0]ping_count;


localparam  IDLE_S = 3'd0,
            SWITCH_S =3'd1,
			TRANMUX_S = 3'd2,
			TRANRX_S = 3'd3,
			TRANRXMUX_S = 3'd4,
			DISCARD_S = 3'd5;
            
always @(posedge clk or negedge rst_n) begin
   if(rst_n == 1'b0) begin
      parser2rx_data_wr <= 1'b0;
	  parser2rx_data <= 134'b0;
	  parser2rx_data_valid <= 1'b0;
	  parser2rx_data_valid_wr <= 1'b0;
	  parser2mux_data_wr <= 1'b0;
	  parser2mux_data <= 134'b0;
	  parser2mux_data_valid <= 1'b0;
	  parser2mux_data_valid_wr <= 1'b0;
	  pkt_step_cnt <= 8'b0;
	  md_reg0 <= 134'b0;
	  md_reg1 <= 134'b0;
	  ping_count<=32'b0;
	  parser_state <= IDLE_S;
   end
   else begin
      case(parser_state)
	  IDLE_S:begin
	     parser2rx_data_wr <= 1'b0;
         parser2rx_data <= 134'b0;
         parser2rx_data_valid <= 1'b0;
         parser2rx_data_valid_wr <= 1'b0;
         parser2mux_data_wr <= 1'b0;
         parser2mux_data <= 134'b0;
         parser2mux_data_valid <= 1'b0;
         parser2mux_data_valid_wr <= 1'b0;       
	     if((pktin_data[133:132] == 2'b01) && (pktin_data_wr == 1'b1) && (parser_alf == 1'b0)) begin  //Metadata
		    pkt_step_cnt <= pkt_step_cnt + 8'd1;
			md_reg0 <= pktin_data;
			parser_state <= SWITCH_S;
		 end
		 else if((md_reg1[133:132] == 2'b01) && (parser_alf == 1'b0)) begin  //have next pkt in,just trans 
		    pkt_step_cnt <=  8'd2;
            md_reg0 <= md_reg1;
            md_reg1 <= pktin_data;
            parser_state <= SWITCH_S;
		 end
		 else begin     //idle wait
		    pkt_step_cnt <= pkt_step_cnt;
			md_reg0 <= md_reg0;
			parser_state <= IDLE_S;
		 end
	  end
	  
	  SWITCH_S:begin
	     parser2rx_data_wr <= 1'b0;
         parser2rx_data <= 134'b0;
         parser2rx_data_valid <= 1'b0;
         parser2rx_data_valid_wr <= 1'b0;
         parser2mux_data_wr <= 1'b0;
         parser2mux_data <= 134'b0;
         parser2mux_data_valid <= 1'b0;
         parser2mux_data_valid_wr <= 1'b0;
	     if((pktin_data_wr == 1'b1) && (parser_alf == 1'b0)) begin
		    if(pkt_step_cnt == 8'd1) begin   
               pkt_step_cnt <= pkt_step_cnt + 8'd1;
			   md_reg1 <= pktin_data;
			   parser_state <= SWITCH_S;
            end	
            else if(pkt_step_cnt == 8'd2) begin
                 if(pktin_data[31:16] == 16'h88f7) begin  //is ptp
                     if(device_role[1]==1'b1) begin    //role swicth
			              if(pktin_data[127:80]==device_mac && (pktin_data[15:0]==16'h0301||pktin_data[15:0]==16'h0401))begin	//unicast
			                   md_reg0 <= md_reg1;
                               md_reg1 <= pktin_data;
                               parser2rx_data_wr <= 1'b1;
                               parser2rx_data <= md_reg0;                 
                               parser_state <= TRANRX_S;    			              			              			             			              
			              end	
			               else if( pktin_data[127:80]==48'hffffffffffff)begin	  	//slave and boardcast		    
			                     if(device_role[0]==1'b0)begin              
				                  md_reg0 <= md_reg1;
                                  md_reg1 <= pktin_data;
				                  parser2rx_data_wr <= 1'b1;
				                  parser2rx_data <= md_reg0;
				                  parser2mux_data_wr <= 1'b1;
                                  parser2mux_data <= md_reg0;				  
				                  parser_state <= TRANRXMUX_S;
			                     end
				                  else begin
				                   parser_state <= DISCARD_S;  
				                   end			                  
				               end
				           else begin//no match mac
				               md_reg0 <= md_reg1;
                               md_reg1 <= pktin_data;
                               parser2mux_data_wr <= 1'b1;
                               parser2mux_data <= md_reg0;
                               parser_state <= TRANMUX_S;				           
				           end	
				   end
				   else begin//role m s
				       if(pktin_data[127:80]==device_mac || pktin_data[127:80]==48'hffffffffffff )begin	//unicast
                            md_reg0 <= md_reg1;
                            md_reg1 <= pktin_data;
                            parser2rx_data_wr <= 1'b1;
                            parser2rx_data <= md_reg0;                 
                            parser_state <= TRANRX_S;                                                                                                           
                       end    
                       else begin//no match mac
                            parser_state <= DISCARD_S;                           
                        end    
                   end          
			   end
			   else begin////is't ptp
			     ping_count<=ping_count+32'b1;
			      md_reg0 <= md_reg1;
                  md_reg1 <= pktin_data;
                  parser2mux_data_wr <= 1'b1;
                  parser2mux_data <= md_reg0;
				  parser_state <= TRANMUX_S;
			   end
            end
			else begin
			   pkt_step_cnt <= 8'b0;
			   md_reg0 <= 134'b0;
			   md_reg1 <= 134'b0;
			   parser_state <= IDLE_S;
			end           			
		 end
		 else begin
		    pkt_step_cnt <= 8'b0;
			md_reg0 <= 134'b0;
			md_reg1 <= 134'b0;
			parser_state <= IDLE_S;
		 end
	  end
	  
	  TRANRX_S:begin
		md_reg0 <= md_reg1;
        md_reg1 <= pktin_data;
        parser2rx_data_wr <= 1'b1;
        parser2rx_data <= md_reg0;
        if(md_reg0[133:132] == 2'b10) begin
           parser2rx_data_valid <= 1'b1;
           parser2rx_data_valid_wr <= 1'b1;        
           if(md_reg1[133:132] == 2'b01) begin
              pkt_step_cnt <= 8'd2;
              parser_state <= SWITCH_S;
           end
           else begin
              pkt_step_cnt <= 8'b0;
              parser_state <= IDLE_S;
           end        
        end
        else begin
           parser_state <= TRANRX_S;        
        end
	  end
	  
	  TRANMUX_S:begin
        md_reg0 <= md_reg1;
        md_reg1 <= pktin_data;
        parser2mux_data_wr <= 1'b1;
        parser2mux_data <= md_reg0;
        if(md_reg0[133:132] == 2'b10) begin
           parser2mux_data_valid <= 1'b1;
           parser2mux_data_valid_wr <= 1'b1;
           pkt_step_cnt <= 8'b0;
           if(md_reg1[133:132] == 2'b01) begin
              pkt_step_cnt <= 8'd2;
              parser_state <= SWITCH_S;
           end
           else begin
              pkt_step_cnt <= 8'b0;
              parser_state <= IDLE_S;
           end
        end
        else begin
           parser_state <= TRANMUX_S;        
        end
      end	 
      
       TRANRXMUX_S:begin
             md_reg0 <= md_reg1;
             md_reg1 <= pktin_data;
             parser2rx_data_wr <= 1'b1;
             parser2rx_data <= md_reg0;
             parser2mux_data_wr <= 1'b1;
             parser2mux_data <= md_reg0;
             if(md_reg0[133:132] == 2'b10) begin
                parser2mux_data_valid <= 1'b1;
                parser2mux_data_valid_wr <= 1'b1;
                parser2rx_data_valid <= 1'b1;
                parser2rx_data_valid_wr <= 1'b1; 
                pkt_step_cnt <= 8'b0;
                if(md_reg1[133:132] == 2'b01) begin
                   pkt_step_cnt <= 8'd2;
                   parser_state <= SWITCH_S;
                end
                else begin
                   pkt_step_cnt <= 8'b0;
                   parser_state <= IDLE_S;
                end
             end
             else begin
                parser_state <= TRANRXMUX_S;        
             end
           end     
      DISCARD_S:begin
          if(pktin_data[133:132]==2'b10)begin
		      pkt_step_cnt <= 8'b0;
              parser_state <= IDLE_S;         
          end
          else begin
             parser_state <= DISCARD_S;     
          end     
      end
	    
	  default:begin
	    parser2rx_data_wr <= 1'b0;
		parser2rx_data <= 134'b0;
		parser2rx_data_valid <= 1'b0;
		parser2rx_data_valid_wr <= 1'b0;
		parser2mux_data_wr <= 1'b0;
		parser2mux_data <= 134'b0;
		parser2mux_data_valid <= 1'b0;
		parser2mux_data_valid_wr <= 1'b0;
		pkt_step_cnt <= 8'b0;
		md_reg0 <= 134'b0;
		md_reg1 <= 134'b0;
		parser_state <= IDLE_S;
	  end
	  endcase   
   end
end


endmodule