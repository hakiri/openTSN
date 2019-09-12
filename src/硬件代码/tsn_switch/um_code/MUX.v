module MUX(
    input wire clk,
	input wire rst_n,
	
	input wire tx2mux_data_wr,
	input wire [133:0] tx2mux_data,
	input wire tx2mux_data_valid,
	input wire tx2mux_data_valid_wr,
	output mux2tx_data_alf,
	
	input wire parser2mux_data_wr,
	input wire [133:0] parser2mux_data,
	input wire parser2mux_data_valid,
	input wire parser2mux_data_valid_wr,
	output mux2parser_data_alf,
	
	output reg mux2gpp_data_wr,
	output reg [133:0] mux2gpp_data,
	output reg mux2gpp_data_valid,
	output reg mux2gpp_data_valid_wr,
	input gpp2mux_data_alf
	

	
);
reg [31:0]mux_out;
always @ (posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
       mux_out<=32'b0;
    end
    else begin
        if(mux2gpp_data_valid)begin
           mux_out<=mux_out+32'b1;
        end
        else begin
            mux_out<=mux_out;
        end
    end
end


reg [31:0]mux_ping;
reg [10:0]pkt_step;
always @ (posedge clk or negedge rst_n) begin
    if(rst_n == 1'b0) begin
       mux_ping<=32'b0;
       pkt_step<=11'b0;
    end
    else begin
        if(parser2mux_data_wr==1'b1 && parser2mux_data_valid==1'b0)begin
            pkt_step<=pkt_step+11'b1;
         end
         else if(parser2mux_data_valid==1'b1)begin
            pkt_step<=11'b0;
         end   
         else begin
             pkt_step<=pkt_step;
         end
            
        if(parser2mux_data_wr==1'b1 && pkt_step==11'd2 && parser2mux_data[31:16]==16'h0800)begin
           mux_ping<=mux_ping+32'b1;
        end
        else begin
            mux_ping<=mux_ping;
        end
    end
end


reg tx_dfifo_rd;
wire [133:0] tx_dfifo_rdata;
wire [8:0] tx_dfifo_usedw;

reg tx_vfifo_rd;
wire tx_vfifo_rdata;
wire tx_vfifo_empty;

reg parser_dfifo_rd;
wire [133:0] parser_dfifo_rdata;
wire [8:0] parser_dfifo_usedw;

reg parser_vfifo_rd;
wire parser_vfifo_rdata;
wire parser_vfifo_empty;

reg [1:0] mux_state;
reg [31:0]ping_count;
assign mux2tx_data_alf = tx_dfifo_usedw[8];
assign mux2parser_data_alf = parser_dfifo_usedw[8];

localparam IDLE_S  = 2'd1,
           TRANTX_S = 2'd2,
		   TRANPR_S = 2'd3;


always @ (posedge clk or negedge rst_n) begin
   if(rst_n == 1'b0) begin
      mux2gpp_data_wr <= 1'b0;
	  mux2gpp_data <= 134'b0;
	  mux2gpp_data_valid <= 1'b0;
	  mux2gpp_data_valid_wr <= 1'b0;
	  tx_dfifo_rd <= 1'b0;
	  tx_vfifo_rd <= 1'b0;
	  parser_dfifo_rd <= 1'b0;
	  parser_vfifo_rd <= 1'b0;
	  mux_state <= IDLE_S;
   end
   else begin
      case(mux_state)
	    IDLE_S:begin
	       mux2gpp_data_wr <= 1'b0;
           mux2gpp_data <= 134'b0;
           mux2gpp_data_valid <= 1'b0;
           mux2gpp_data_valid_wr <= 1'b0;
		   if(gpp2mux_data_alf == 1'b0) begin
		      if((tx_vfifo_empty == 1'b0)&&(parser_vfifo_empty == 1'b0)) begin  //both have pkt，trans tx
			     tx_dfifo_rd <= 1'b1;
				 tx_vfifo_rd <= 1'b1;
				 parser_dfifo_rd <= 1'b0;
				 parser_vfifo_rd <= 1'b0;
				 mux_state <= TRANTX_S;				 
			  end
			  else if((tx_vfifo_empty == 1'b0)&&(parser_vfifo_empty == 1'b1)) begin //tx have pkt,trans tx
			     tx_dfifo_rd <= 1'b1;
				 tx_vfifo_rd <= 1'b1;
				 parser_dfifo_rd <= 1'b0;
				 parser_vfifo_rd <= 1'b0;
				 mux_state <= TRANTX_S;
			  end
			  else if((tx_vfifo_empty == 1'b1)&&(parser_vfifo_empty == 1'b0)) begin //parser have pkt,trans parser
			     tx_dfifo_rd <= 1'b0;
				 tx_vfifo_rd <= 1'b0;
				 parser_dfifo_rd <= 1'b1;
				 parser_vfifo_rd <= 1'b1;
				 mux_state <= TRANPR_S;
			  end 
			  else begin
				 tx_dfifo_rd <= tx_dfifo_rd;
				 tx_vfifo_rd <= tx_vfifo_rd;
				 parser_dfifo_rd <= parser_dfifo_rd;
				 parser_vfifo_rd <= parser_vfifo_rd;
				 mux_state <= IDLE_S;
			  end		   
		   end
		   else begin
			  tx_dfifo_rd <= tx_dfifo_rd;
			  tx_vfifo_rd <= tx_vfifo_rd;
			  parser_dfifo_rd <= parser_dfifo_rd;
			  parser_vfifo_rd <= parser_vfifo_rd;
			  mux_state <= IDLE_S;
		   end
		end
		
		TRANTX_S:begin
		   tx_vfifo_rd <= 1'b0;
		   mux2gpp_data_wr <= 1'b1;
		   mux2gpp_data <= tx_dfifo_rdata;
		   if(tx_dfifo_rdata[133:132] == 2'b10) begin
		      tx_dfifo_rd <= 1'b0;
			  mux2gpp_data_valid <= tx_vfifo_rdata;
			  mux2gpp_data_valid_wr <= 1'b1;
		      mux_state <= IDLE_S;
		   end
		   else begin
		      tx_dfifo_rd <= 1'b1;
			  mux2gpp_data_valid <= 1'b0;
			  mux2gpp_data_valid_wr <= 1'b0;
		      mux_state <= TRANTX_S;
		   end
		end
		
		TRANPR_S:begin
		   parser_vfifo_rd <= 1'b0;
		   mux2gpp_data_wr <= 1'b1;
		   mux2gpp_data <= parser_dfifo_rdata;
		   if(parser_dfifo_rdata[133:132] == 2'b10) begin
		      parser_dfifo_rd <= 1'b0;
			  mux2gpp_data_valid <= parser_vfifo_rdata;
			  mux2gpp_data_valid_wr <= 1'b1;
		      mux_state <= IDLE_S;
		   end
		   else begin
		      parser_dfifo_rd <= 1'b1;
			  mux2gpp_data_valid <= 1'b0;
			  mux2gpp_data_valid_wr <= 1'b0;
		      mux_state <= TRANPR_S;
		   end		
		end
			
		default:begin
		    mux2gpp_data_wr <= 1'b0;
			mux2gpp_data <= 134'b0;
			mux2gpp_data_valid <= 1'b0;
			mux2gpp_data_valid_wr <= 1'b0;
			tx_dfifo_rd <= 1'b0;
			tx_vfifo_rd <= 1'b0;
			parser_dfifo_rd <= 1'b0;
			parser_vfifo_rd <= 1'b0;
			mux_state <= IDLE_S;
		end	  
	  endcase  
   end
end


mux_fifo_w134_d256 tx_dfifo(
    .srst(!rst_n),
    .clk(clk),
    .din(tx2mux_data),
    .wr_en(tx2mux_data_wr),
    .dout(tx_dfifo_rdata),
    .rd_en(tx_dfifo_rd),
    .data_count(tx_dfifo_usedw),
    .empty(),
    .full()
);

mux_fifo_w1_d64 tx_vfifo(
    .srst(!rst_n),
    .clk(clk),
    .din(tx2mux_data_valid),
    .wr_en(tx2mux_data_valid_wr),
    .dout(tx_vfifo_rdata),
    .rd_en(tx_vfifo_rd),
  //  .data_count(),
    .empty(tx_vfifo_empty),
    .full()
);

mux_fifo_w134_d256 parser_dfifo(
    .srst(!rst_n),
    .clk(clk),
    .din(parser2mux_data),
    .wr_en(parser2mux_data_wr),
    .dout(parser_dfifo_rdata),
    .rd_en(parser_dfifo_rd),
    .data_count(parser_dfifo_usedw),
    .empty(),
    .full()
);

mux_fifo_w1_d64 parser_vfifo(
    .srst(!rst_n),
    .clk(clk),
    .din(parser2mux_data_valid),
    .wr_en(parser2mux_data_valid_wr),
    .dout(parser_vfifo_rdata),
    .rd_en(parser_vfifo_rd),
  //  .data_count(),
    .empty(parser_vfifo_empty),
    .full()
);

endmodule