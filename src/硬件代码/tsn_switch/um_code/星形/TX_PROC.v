module TX_PROC(
input wire clk,
input wire rst_n,
////////////////cyc_sync////////////////
output reg ts_3_valid,
output reg [47:0]ts_3,//{ms_cnt,cyc_cnt}
input wire [47:0]cyc_timer,
input wire send_sync_pkt,
input wire send_test_pkt,
//////////////RX_porc///////////////////
input wire ts_4_valid,
input wire [47:0]ts_4,
input wire [53:0]key,
input wire key_valid,
///////////////PTP_CTRL/////////////////
input send_dreq_pkt,
input send_dresq_pkt,
output reg[3:0]ptp_send_type,
output reg ptp_send_type_valid,
/////////////////MAC////////////////////
output reg outtx_data_wr,
output reg [133:0]outtx_data,
output reg outtx_valid_wr,
output reg outtx_valid,
input wire outtx_ready,
output reg [47:0]rtt_time,
///////////////CTRL///////////////////////
input [47:0]MAC_ADDR
);

reg send_sync_pkt_rd;
reg send_dreq_pkt_rd;
reg send_dresq_pkt_rd;
reg send_test_pkt_rd;
reg key_rd;
reg ts_4_rd;
reg [47:0]ts_4_reg;
reg [47:0]timestamp;
wire [53:0]key_rdata;
wire [47:0]ts_4_rdata;
reg [3:0]tx_type;
wire ts_4_empty;
wire sync_empty;
wire req_empty;
wire resq_empty;

reg [47:0]t1_time;
reg [47:0]t4_time;
reg clc_rtt;
wire test_empty;
reg [5:0]outport;
reg [2:0]tx_state;
    localparam idle_s=3'd0,
    trans_meta1=3'd1,
    trans_fdata=3'd2,
    trans_sdata=3'd3,
    trans_tdata=3'd4,
    trans_edata=3'd5;
reg     [31:0]tx_t1_count,tx_t3_count,tx_t4_count;	           
always @(posedge clk or negedge rst_n)begin
	if(rst_n==1'b0)begin
		ts_3_valid<=1'b0;
		ts_3<=48'b0;
		outtx_data_wr<=1'b0;
		outtx_data<=134'b0;
		outtx_valid_wr<=1'b0;
		outtx_valid<=1'b0;
		key_rd<=1'b0;
		ts_4_rd<=1'b0;
		tx_type<=4'b0;
		send_sync_pkt_rd<=1'b0;
		send_dreq_pkt_rd<=1'b0;
		send_dresq_pkt_rd<=1'b0;
		send_test_pkt_rd<=1'b0;
		ptp_send_type_valid<=1'b0;
		ptp_send_type<=4'b0;
		timestamp<=48'b0;
		
		t1_time<=48'b0;
		t4_time<=48'b0;
		clc_rtt<=1'b0;
		tx_t1_count<=32'b0;
		tx_t3_count<=32'b0;
		tx_t4_count<=32'b0;
		tx_state<=idle_s;
	end
	else begin
		case(tx_state)
		idle_s:begin
		    ts_3_valid<=1'b0;
		    ts_3<=48'b0;
		    outtx_data_wr<=1'b0;
		    outtx_data<=134'b0;
		    outtx_valid_wr<=1'b0;
			send_sync_pkt_rd<=1'b0;
		    send_dreq_pkt_rd<=1'b0;
		    send_dresq_pkt_rd<=1'b0;
		    send_test_pkt_rd<=1'b0;
		    ptp_send_type_valid<=1'b0;
            ptp_send_type<=4'b0;
		    outtx_valid<=1'b0;
			ts_4_rd<=1'b0;
			key_rd<=1'b0;
			tx_type<=4'b0;
			timestamp<=48'b0;
//			t1_time<=48'b0;
//	    	t4_time<=48'b0;
	    	clc_rtt<=1'b0;
            if(outtx_ready==1'b1)begin//ready 
			    timestamp<=cyc_timer;
		    	if(sync_empty==1'b0)begin//send sync enable//sync
		    		send_sync_pkt_rd<=1'b1;
	                tx_type<=4'b0001;
					outtx_data_wr<=1'b1;
					outtx_data[133:128] <= {2'b01,4'd0};//head,vbyte
					outtx_data[127] <= 1'd1;//pktsrc 
					outtx_data[126] <= 1'd0;//pktdst
                    outtx_data[125:120] <= 6'b111111;//IngressPort
					outtx_data[119:118] <=2'b00;//outtype
					outtx_data[117:112] <=6'b111111;//outport
					outtx_data[111:109] <=3'b0;//priority
					outtx_data[108] <= 1'b0;//discard
                    outtx_data[107:96] <= 12'd60+ 12'd32;//pkt_length
                    outtx_data[95:88] <= 8'd4;//SrcModuleID
                    outtx_data[87:80] <= 8'd5;//DstModuleID
					outtx_data[79:72] <= 8'b0;//pst
                    outtx_data[71:64] <= 12'b0;//pkt id(Seq_num)      							
                    outtx_data[63:50] <= 15'b0;//flowid
                    outtx_data[49:48] <= 18'b0;//reserve
                    outtx_data[47:0] <= cyc_timer;//timestamp 
                    
					tx_state<=trans_meta1;
				end
				else if(req_empty==1'b0)begin//req
					send_dreq_pkt_rd<=1'b1;
					outtx_data_wr<=1'b1;
		            outtx_data_wr<=1'b1;
					outtx_data[133:128] <= {2'b01,4'd0};//head,vbyte
					outtx_data[127] <= 1'd1;//pktsrc 
					outtx_data[126] <= 1'd0;//pktdst
                    outtx_data[125:120] <=key_rdata[5:0];//IngressPort
					outtx_data[119:118] <=2'b00;//outtype
					outtx_data[117:112] <=key_rdata[5:0];//outport
					outtx_data[111:109] <=3'b0;//priority
					outtx_data[108] <= 1'b0;//discard
                    outtx_data[107:96] <= 12'd60+ 12'd32;//pkt_length
                    outtx_data[95:88] <= 8'd4;//SrcModuleID
                    outtx_data[87:80] <= 8'd5;//DstModuleID
					outtx_data[79:72] <= 8'b0;//pst
                    outtx_data[71:64] <= 12'b0;//pkt id(Seq_num)      							
                    outtx_data[63:50] <= 15'b0;//flowid
                    outtx_data[49:48] <= 18'b0;//reserve
                    outtx_data[47:0] <= cyc_timer;//timestamp 
					tx_state<=trans_meta1;
					ts_3_valid<=1'b1;
					ts_3<=cyc_timer;
					tx_type<=4'b0010;
				end
				else if((resq_empty==1'b0)&&(ts_4_empty==1'b0))begin//resq
				    send_dresq_pkt_rd<=1'b1;
					outtx_data_wr<=1'b1;
		            outtx_data_wr<=1'b1;
					outtx_data[133:128] <= {2'b01,4'd0};//head,vbyte
					outtx_data[127] <= 1'd1;//pktsrc 
					outtx_data[126] <= 1'd0;//pktdst
                    outtx_data[125:120] <= key_rdata[5:0];//IngressPort
					outtx_data[119:118] <=2'b00;//outtype
					outtx_data[117:112] <=key_rdata[5:0];//outport
					outtx_data[111:109] <=3'b0;//priority
					outtx_data[108] <= 1'b0;//discard
                    outtx_data[107:96] <= 12'd60+ 12'd32;//pkt_length
                    outtx_data[95:88] <= 8'd4;//SrcModuleID
                    outtx_data[87:80] <= 8'd5;//DstModuleID
					outtx_data[79:72] <= 8'b0;//pst
                    outtx_data[71:64] <= 12'b0;//pkt id(Seq_num)      							
                    outtx_data[63:50] <= 15'b0;//flowid
                    outtx_data[49:48] <= 2'b0;//reserve
                    outtx_data[47:0] <= cyc_timer;//timestamp 
					tx_state<=trans_meta1;
					tx_type<=4'b0100;
			    end
				else if(test_empty==1'b0)begin//test
				    send_test_pkt_rd<=1'b1;
					outtx_data_wr<=1'b1;
		            outtx_data_wr<=1'b1;
					outtx_data[133:128] <= {2'b01,4'd0};//head,vbyte
					outtx_data[127] <= 1'd1;//pktsrc 
					outtx_data[126] <= 1'd0;//pktdst
                    outtx_data[125:120] <= 6'b0;//IngressPort
					outtx_data[119:118] <=2'b00;//outtype
					outtx_data[117:112] <=6'b0;//outport
					outtx_data[111:109] <=3'b0;//priority
					outtx_data[108] <= 1'b0;//discard
                    outtx_data[107:96] <= 12'd60+ 12'd32;//pkt_length
                    outtx_data[95:88] <= 8'd4;//SrcModuleID
                    outtx_data[87:80] <= 8'd5;//DstModuleID
					outtx_data[79:72] <= 8'b0;//pst
                    outtx_data[71:64] <= 12'b0;//pkt id(Seq_num)      							
                    outtx_data[63:50] <= 15'b0;//flowid
                    outtx_data[49:48] <= 2'b0;//reserve
                    outtx_data[47:0] <= cyc_timer;//timestamp 
					tx_state<=trans_meta1;
					tx_type<=4'b1000;
				end
				else begin
				tx_state<=idle_s;				
				end
			end
			else begin
			    tx_state<=idle_s;				
			end
	    end
		trans_meta1:begin
		    send_sync_pkt_rd<=1'b0;
		    send_dreq_pkt_rd<=1'b0;
		    send_dresq_pkt_rd<=1'b0;
		    send_test_pkt_rd<=1'b0;
			ts_3_valid<=1'b0;
			outtx_data_wr<=1'b1;
			outtx_data[133:128] <= {2'b11,4'd0};//mid,vbyte
			outtx_data[127:0] <= 128'd0;
		   case(tx_type)//resq
		   4'b0010:begin
		      key_rd<=1'b1;
		   end
		   4'b0100:begin//resq
			key_rd<=1'b1;
			ts_4_rd<=1'b1;
			ts_4_reg<=ts_4_rdata;
			
		   end
		   default:begin
		    key_rd<=1'b0;
			ts_4_rd<=1'b0;
		   end
		   endcase
		   tx_state<=trans_fdata;				
		end
		trans_fdata:begin
		ts_4_reg<=ts_4_reg;
		    key_rd<=1'b0;
			ts_4_rd<=1'b0;
		     case(tx_type)
		        4'b0001:begin//sync
		           outtx_data_wr<=1'b1;
		           outtx_data<={6'b110000,48'hffffffffffff,MAC_ADDR,16'h88F7,16'h0101};///////////////
		           tx_t1_count<=tx_t1_count+1'b1;
		        end
		        4'b0010:begin//req
			       outtx_data_wr<=1'b1;
		           outtx_data<={6'b110000,key_rdata[53:6],MAC_ADDR,16'h88F7,16'h0301};///////////////
		           tx_t3_count<=tx_t3_count+1'b1;
		        end
				4'b0100:begin//resq
				    outtx_data_wr<=1'b1;
		            outtx_data<={6'b110000,key_rdata[53:6],MAC_ADDR,16'h88F7,16'h0401};///////////////
		            tx_t4_count<=tx_t4_count+1'b1;
				end
				4'b1000:begin//test
				    outtx_data_wr<=1'b1;
		            outtx_data<={6'b110000,32'h12345678,MAC_ADDR[15:0],MAC_ADDR,16'h88F7,16'h0501};///////////////	
                end					
		        default:begin
		            outtx_data_wr<=1'b1;
		            outtx_data<={6'b110000,48'h1234567890ab,MAC_ADDR,16'h88F7,16'h0501};///////////////	
		        end			
		   endcase
		   tx_state<=trans_sdata;
		end
        trans_sdata:begin
		   	outtx_data_wr<=1'b1;
		    outtx_data<={6'b110000,16'h30,8'b00,8'b00,16'b0000,64'b0,16'b0};///////////////	 
            tx_state<=trans_tdata;			
		end
		 trans_tdata:begin
		   	outtx_data_wr<=1'b1;
		    outtx_data<={6'b110000,128'b0};///////////////	 
            tx_state<=trans_edata;			
		end
		trans_edata:begin
		   	outtx_data_wr<=1'b1;
		
			
			 case(tx_type)
                          4'b0001:begin//sync
                             outtx_data<={6'b100100,32'b0,timestamp,48'b0};///////////////
                             ptp_send_type_valid<=1'b1;
                             ptp_send_type<=4'd1;
                             t1_time<=timestamp;
                           end
                           4'b0010:begin//req
                             outtx_data<={6'b100100,32'b0,timestamp,48'b0};///////////////
                             ptp_send_type_valid<=1'b1;
                              ptp_send_type<=4'd3;
                           end
                           4'b0100:begin//resq
                              outtx_data<={6'b100100,32'b0,ts_4_reg,48'b0};///////////////
                              ptp_send_type_valid<=1'b1;
                              ptp_send_type<=4'd4;
                               t4_time<=ts_4_reg;
                               clc_rtt<=1'b1;
                           end
                           4'b1000:begin//test
                              outtx_data<={6'b100100,32'b0,timestamp,48'b0};///////////////  
                            //  ptp_send_type_valid<=1'b1;
                           //   ptp_send_type<=4'd5;
                           end                    
                           default:begin
                              outtx_data<={6'b100100,32'b0,timestamp,48'b0};/////////////// 
                            //  ptp_send_type_valid<=1'b1;
                            //  ptp_send_type<=4'd5;
                           end            
                      endcase						
			outtx_valid_wr<=1'b1;
			outtx_valid<=1'b1;
            tx_state<=idle_s;			
			
		end
	endcase
	end

end


always @(posedge clk or negedge rst_n)begin
	if(rst_n==1'b0)begin
	   rtt_time<=48'b0;
	end
	else begin
	    if(clc_rtt==1'b1)begin
	        if((t4_time[16:0]+18'h9C>=t1_time[16:0]+18'b0))begin
			      rtt_time<={t4_time[47:17]-t1_time[47:17],(t4_time[16:0]-t1_time[16:0]+17'h9C)};
			end
			else begin
			      rtt_time<={t4_time[47:17]-t1_time[47:17]-1'b1,(t4_time[16:0]+(17'd125000-t1_time[16:0]+17'h9C))};
		    end
	    end
	    else begin
	        rtt_time<=rtt_time;
	    end
	end
end

always @ * begin
  case(key_rdata[5:0])
  6'd0:begin outport=6'b000001;end
  6'd1:begin outport=6'b000010;end
  6'd2:begin outport=6'b000100;end
  6'd3:begin outport=6'b001000;end
  default:begin  outport=6'b000000;end
  endcase
end


fifo_w48_d64  tx_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(ts_4),
	.rd_en(ts_4_rd),
	.wr_en(ts_4_valid),
	.dout(ts_4_rdata),
//	.data_count(),
	.empty(ts_4_empty),
	.full()
	);

fifo_w54_d64  key_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(key),
	.rd_en(key_rd),
	.wr_en(key_valid),
	.dout(key_rdata),
//	.data_count(),
	.empty(),
	.full()
	);
mux_fifo_w1_d64  sync_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(send_sync_pkt),
	.rd_en(send_sync_pkt_rd),
	.wr_en(send_sync_pkt),
	.dout(),
//	.data_count(),
	.empty(sync_empty),
	.full()
	);
	
	

mux_fifo_w1_d64  test_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(send_test_pkt),
	.rd_en(send_test_pkt_rd),
	.wr_en(send_test_pkt),
	.dout(),
//	.data_count(),
	.empty(test_empty),
	.full()
	);
	
mux_fifo_w1_d64  req_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(send_dreq_pkt),
	.rd_en(send_dreq_pkt_rd),
	.wr_en(send_dreq_pkt),
	.dout(),
	//.data_count(),
	.empty(req_empty),
	.full()
	);
mux_fifo_w1_d64  resq_fifo(
	.srst(!rst_n),
	.clk(clk),
	.din(send_dresq_pkt),
	.rd_en(send_dresq_pkt_rd),
	.wr_en(send_dresq_pkt),
	.dout(),
//.data_count(),
	.empty(resq_empty),
	.full()
	);
endmodule