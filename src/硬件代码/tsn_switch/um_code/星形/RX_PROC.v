//===============================================================//
//	Module name: Master-clock module for precise time synchronization (USG);
//	Communication with fuwenwen(fuwenwen94@163.com)
//	Last edited time: 2018/12/30
//	Function outline: ptp_v0.1
//===============================================================//

//	Function outline: ptp_v0.1
//===============================================================//

`timescale 1ns/1ps

module	RX_PROC(
	clk,
	reset,
//from port_cmp
	inptp_data_wr,
	inptp_data,
	inptp_valid_wr,
	inptp_valid,
	inptp_ready,

// to tx_proc
	ts_4_time_wr,
	ts_4_time,
	key,
	key_valid,
	
// to ptp_ctrl
	ptp_recv_type_valid,
	ptp_recv_type,
	
// to cyc_sync
	ts_2_record,
	ts_1_valid,
	ts_1,
	ts_4_valid,
	ts_4,
	timer	
);

reg [31:0]rx_t1_type;  
reg [31:0]rx_t3_type;  
reg [31:0]rx_t4_type;            
always @(posedge clk or negedge reset) begin
   if(reset == 1'b0) begin
           rx_t1_type<=32'b0;
           rx_t3_type<=32'b0;
           rx_t4_type<=32'b0;
   end
   else begin
       if(ptp_recv_type_valid==1'b1)begin
          if(ptp_recv_type==4'b1)begin
              rx_t1_type<=rx_t1_type+32'b1;
          end
          else  if(ptp_recv_type==4'd3)begin
             rx_t3_type<=rx_t3_type+32'b1;
          end
          else if (ptp_recv_type==4'd4)begin
               rx_t4_type<=rx_t4_type+32'b1;
          end
          else begin
            rx_t1_type<=rx_t1_type;  
           rx_t3_type<=rx_t3_type;  
           rx_t4_type<=rx_t4_type;  
          end
       end
       else begin
          rx_t1_type<=rx_t1_type;  
          rx_t3_type<=rx_t3_type;  
          rx_t4_type<=rx_t4_type;  
       end
   end
end



parameter w_pkt = 134,
		  w_key = 54,
		  w_ts	= 48;
input	clk;
input	reset;
input	inptp_data_wr;
input	[w_pkt-1:0]	inptp_data;
input	inptp_valid_wr;
input	inptp_valid;
output	wire inptp_ready;
output reg ts_4_time_wr;
output reg [47:0]ts_4_time;

output	reg [w_key-1:0]	key;
output	reg key_valid;

output	reg	ptp_recv_type_valid;
output 	reg	[4:0]	ptp_recv_type;

output	reg	ts_2_record;
output	reg	ts_1_valid;
output 	reg	[w_ts-1:0]	ts_1;
output 	reg	ts_4_valid;
output	reg	[w_ts-1:0]	ts_4;
input	[w_ts-1:0]	timer;
wire [9:0]data_count;

reg [w_ts-1:0]rx_timer;
//input_fifo parameter
reg		rdreq_pktin;
wire	[w_pkt-1:0]	q_pktin;
wire	empty_pktin;
reg inptp_valid_rd;
wire inptp_valid_rdata;

reg [47:0]md_timestamp;
//reg		[3:0]	ptp_recv_type_temp;
reg		[47:0]	modidy_domain, timer_reg;
reg     [31:0]rx_t1_count,rx_t3_count,rx_t4_count;
reg		[3:0]	rx_state;
parameter		IDLE_S = 4'd0,
				READ_METE_0_S = 4'd1,
				READ_METE_1_S = 4'd2,
				READ_ETH_S = 4'd3,
				READ_PTP1_S = 4'd4,
				READ_PTP2_S = 4'd5,
				READ_PTP3_S = 4'd6,
				DISCARD_S =4'd7;

assign inptp_ready = ~data_count[8];
always @(posedge clk or negedge reset)
	if(!reset) begin
		ts_4_time_wr<=1'b0;
		ts_4_valid	<= 1'b0;
		key_valid	<= 1'b0;
		key			<= 54'b0;
		inptp_valid_rd<=1'b0;
		ptp_recv_type_valid	<= 1'b0;
		ptp_recv_type	<= 4'b0;
		ts_2_record	<= 1'b0;
		ts_1_valid	<= 1'b0;
		rx_timer<=48'b0;
		rx_t1_count<=32'b0;
		rx_t3_count<=32'b0;
		rx_t4_count<=32'b0;
		rx_state	<= IDLE_S;
	end
	else begin
		case(rx_state)
			
			IDLE_S:begin
			  ts_4_valid	<= 1'b0;
			  ts_4_time_wr<=1'b0;
			ptp_recv_type	<= 4'd0;
			 ptp_recv_type_valid	<= 1'b0;
			 inptp_valid_rd<=1'b0;
			 ts_2_record		<= 1'b0;
			 key_valid		<= 1'b0;
			 ts_1_valid		<= 1'b0;
			 rdreq_pktin	<= 1'b0;
			 md_timestamp<=48'b0;
			 rx_timer<=48'b0;
			 inptp_valid_rd<=1'b0;
				if((empty_pktin==1'b0)) begin
				     if (inptp_valid_rdata==1'b1)begin
					    rdreq_pktin		<=  1'b1;
					    inptp_valid_rd<=1'b1;
					    rx_state		<=	READ_METE_0_S;
				      end
				      else begin
				        rdreq_pktin		<=  1'b1;
				        inptp_valid_rd<=1'b1;
				        rx_state		<=	DISCARD_S;
				      end
				 end
				else begin
					rx_state		<=	IDLE_S;
				end
			end
			READ_METE_0_S: begin
			    inptp_valid_rd<=1'b0;
				
				md_timestamp<=q_pktin[47:0];//metadata timer
				key[5:0] <=	q_pktin[125:120];//in_port
				rx_timer<=timer;
				rx_state   <=	READ_METE_1_S;
			end
			READ_METE_1_S:begin
				rx_state   <=	READ_ETH_S;
			end
			READ_ETH_S:begin
				key[53:6]	<= q_pktin[79:32];//SMAC
				if((rx_timer[16:0]>=md_timestamp[16:0]))begin
				md_timestamp<={rx_timer[47:17]-md_timestamp[47:17],(rx_timer[16:0]-md_timestamp[16:0])};
				end
				else begin
				md_timestamp<={rx_timer[47:17]-md_timestamp[47:17]-1'b1,(rx_timer[16:0]+(17'd125000-md_timestamp[16:0]))};
				end
				if(q_pktin[11:8] == 4'd1) begin
					ptp_recv_type	<=	4'd1;
					ptp_recv_type_valid	<= 1'b1;
					ts_2_record	<= 1'b1;
					key_valid	<= 1'b1;
					rx_t1_count<=rx_t1_count+1'b1;
				end
				else if(q_pktin[11:8] == 4'd3) begin
					ptp_recv_type	<=	4'd3;
					ptp_recv_type_valid	<= 1'b1;
					key_valid	<= 1'b1;
					rx_t3_count<=rx_t3_count+1'b1;
				end
				else if(q_pktin[11:8] == 4'd4) begin
				ptp_recv_type    <=    4'd4;
				rx_t4_count<=rx_t4_count+1'b1;
                end                                      
				else begin
				   rx_t4_count<=rx_t4_count;
				    rx_t1_count<=rx_t1_count;
				     rx_t3_count<=rx_t3_count;
					ptp_recv_type_valid	<= 1'b0;
				end
				rx_state   <=	READ_PTP1_S;
			end
			READ_PTP1_S:begin
				ptp_recv_type_valid	<= 1'b0;
				key_valid	<= 1'b0;
				ts_2_record	<= 1'b0;
				if(md_timestamp[16:0]+q_pktin[32:16]+18'b0>=18'd125000)begin
				modidy_domain	<=	{md_timestamp[47:17]+q_pktin[79:33]+1,md_timestamp[16:0]+q_pktin[32:16]-17'd125000};
				end
				else begin
				modidy_domain	<=	{md_timestamp[47:17]+q_pktin[79:33],md_timestamp[16:0]+q_pktin[32:16]};
				end				
				rx_state   <=	READ_PTP2_S;
			end
			READ_PTP2_S:begin
				ts_2_record	<= 1'b0;
				key_valid	<= 1'b0;
				rx_state   <=	READ_PTP3_S;
			end
			READ_PTP3_S:begin
				if(ptp_recv_type == 4'd1) begin
				    if(modidy_domain[16:0]+q_pktin[64:48]+18'b0>=18'd125000)begin
				        ts_1	<=	{modidy_domain[47:17]+q_pktin[95:65]+1,modidy_domain[16:0]+q_pktin[64:48]-17'd125000};
				      end
				        else begin
				        ts_1	<=	{modidy_domain[47:17]+q_pktin[95:65],modidy_domain[16:0]+q_pktin[64:48]};
				    end		
					ts_1_valid	<= 1'b1;
				end
				else if(ptp_recv_type == 4'd3) begin				
					if((rx_timer[16:0]>=modidy_domain[16:0]))begin
				     ts_4_time<={rx_timer[47:17]-modidy_domain[47:17],(rx_timer[16:0]-modidy_domain[16:0])};
				    end
				     else begin
				     ts_4_time<={rx_timer[47:17]-modidy_domain[47:17]-1'b1,(rx_timer[16:0]+(17'd125000-modidy_domain[16:0]))};
				     end
				     
					ts_4_time_wr<= 1'b1;
				end
				else if(ptp_recv_type == 4'd4) begin
					ts_4	<=	q_pktin[95:48];
					ts_4_valid	<= 1'b1;
					  ptp_recv_type_valid    <= 1'b1;
                                                                     
				end
				else begin
					ts_1	<=	ts_1;
					ts_4	<=	ts_4;
				end
				rdreq_pktin	<= 1'b0;
				rx_state   <=	IDLE_S;
			end
		DISCARD_S:begin
		    inptp_valid_rd<=1'b0;
		    if(q_pktin[133:132]==2'b10)begin
		        rx_state   <=	IDLE_S;
		        rdreq_pktin<=1'b0;
		    end
		    else begin
		        rdreq_pktin<=1'b1;
		        rx_state   <=	DISCARD_S;
		    end		
		end
		endcase
	end
	



 mux_fifo_w134_d256  PKT_DFIFO(
	.srst(!reset),
	.clk(clk),
	.din(inptp_data),
	.rd_en(rdreq_pktin),
	.wr_en(inptp_data_wr),
	.dout(q_pktin),
	.data_count(data_count),
	.empty(),
	.full()

	);

mux_fifo_w1_d64  PKT_VFIFO_INST(
	.srst(!reset),
	.clk(clk),
	.din(inptp_valid),
	.rd_en(inptp_valid_rd),
	.wr_en(inptp_valid_wr),
	.dout(inptp_valid_rdata),
//	.data_count(),
	.empty(empty_pktin),
	.full()
	);	

endmodule