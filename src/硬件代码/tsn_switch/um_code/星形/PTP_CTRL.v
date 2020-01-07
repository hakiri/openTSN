//===============================================================//
//	Module name: Master-clock module for precise time synchronization (USG);
//	Communication with fuwenwen(fuwenwen94@163.com)
//	Last edited time: 2018/12/30
//	Function outline: ptp_v0.1
//===============================================================//

//	Function outline: ptp_v0.1
//===============================================================//

`timescale 1ns/1ps

module	PTP_CTRL(
	clk,
	reset,
	//from rx_proc
	ptp_recv_type_valid,
	ptp_recv_type,
	
	// to tx_proc
	send_dreq_pkt,
	send_dresq_pkt,
	//from tx_proc
	ptp_send_type,
	ptp_send_type_valid,

	
	//from ctrl
	sync_start,
	device_role,
	error,
	
	//to cyc_sync
	m_or_s,
	status_ok
);

reg [31:0]send_req_cnt;  
reg [31:0]send_resq_cnt;  
       
always @(posedge clk or negedge reset) begin
   if(reset == 1'b0) begin
           send_req_cnt<=32'b0;
           send_resq_cnt<=32'b0;
   end
   else begin
          if(send_dreq_pkt==1'b1)begin
              send_req_cnt<=send_req_cnt+32'b1;
          end
          else  if(send_dresq_pkt==1'd1)begin
             send_resq_cnt<=send_resq_cnt+32'b1;
          end   
          else begin
              send_req_cnt<=send_req_cnt;  
              send_resq_cnt<=send_resq_cnt; 
          end     
    end      
end



	input 	clk;
	input 	reset;
	input 	ptp_recv_type_valid;
	input 	[3:0] ptp_recv_type;
	output 	reg send_dreq_pkt;
	output 	reg send_dresq_pkt;
	input 	ptp_send_type_valid;
	input 	[3:0] ptp_send_type;
	
	input 	sync_start;
	input 	[1:0]device_role;
	output  reg error;
	output 	m_or_s;
	output	reg	status_ok;
	
	reg [2:0]	ptp_ctrl_state;
	parameter	CLOSED_S = 3'd0,
				RUN_MASTER_STATE = 3'd1,
				//WAIT_RECV_DREQ_S = 3'd2,
			//	WAIT_SEND_DRESQ_S = 3'd3,
				WAIT_RECV_SYNC_S  =3'd4,
				WAIT_SEND_DREQ_S = 3'd5,
				WAIT_RECV_DRESQ_S = 3'd6;
	assign 	m_or_s =device_role[0];
	always @(posedge clk or negedge reset) 
		if(!reset) begin
			
			send_dreq_pkt	<= 1'b0;
			send_dresq_pkt	<= 1'b0;
			error			<= 1'b0;
			status_ok		<= 1'b0;
			ptp_ctrl_state	<=	CLOSED_S;
		end
		else begin
			case(ptp_ctrl_state)
			CLOSED_S: begin
				send_dreq_pkt	<= 1'b0;
				send_dresq_pkt	<= 1'b0;
				status_ok		<= 1'b0;
				error			<= 1'b0;	
				if(sync_start == 1'b1) begin
					if(device_role[0] == 1'b1) begin						
						ptp_ctrl_state	<=	RUN_MASTER_STATE;//master
					end
					else begin						
						ptp_ctrl_state	<=	WAIT_RECV_SYNC_S;//slave
					end
				end
				else begin
					ptp_ctrl_state	<=	CLOSED_S;
				end
			end
									
			RUN_MASTER_STATE:begin
			    if((ptp_recv_type == 4'd3) && (ptp_recv_type_valid == 1'b1)) begin
			        send_dresq_pkt	<= 1'b1;
					ptp_ctrl_state	<=	RUN_MASTER_STATE;
				end
				else if((ptp_recv_type != 4'd3) && (ptp_recv_type_valid == 1'b1))begin
				    ptp_ctrl_state	<=	CLOSED_S;
				    send_dresq_pkt<=1'b0;
				end
				else begin
				    send_dresq_pkt	<= 1'b0;
					ptp_ctrl_state	<=	RUN_MASTER_STATE;
				
				end
			
			end
			
			/*WAIT_SEND_SYNC_S:begin
				if((ptp_send_type == 4'd1) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<= WAIT_RECV_DREQ_S;
				end
				else if((ptp_send_type != 4'd1) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<=	WAIT_SEND_SYNC_S;
			end
					
			WAIT_RECV_DREQ_S:begin
				if((ptp_recv_type == 4'd3) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	WAIT_SEND_DRESQ_S;
					send_dresq_pkt	<= 1'b1;
				end
				else if((ptp_recv_type != 4'd3) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<= WAIT_RECV_DREQ_S;
			end		
			
			WAIT_SEND_DRESQ_S:begin
				send_dresq_pkt	<= 1'b0;
				if((ptp_send_type == 4'd4) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<= CLOSED_S;
				end
				else if((ptp_send_type != 4'd4) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<=	WAIT_SEND_DRESQ_S;
			end*/
			
			WAIT_RECV_SYNC_S:begin
				if((ptp_recv_type == 4'd1) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	WAIT_SEND_DREQ_S;
					send_dreq_pkt	<= 1'b1;
				end
				else if((ptp_recv_type != 4'd1) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<= WAIT_RECV_SYNC_S;
			end
	
			WAIT_SEND_DREQ_S:begin
				send_dreq_pkt	<= 1'b0;
				if((ptp_send_type == 4'd3) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<= WAIT_RECV_DRESQ_S;
				end
				else if((ptp_send_type != 4'd3) && (ptp_send_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<=	WAIT_SEND_DREQ_S;
			end
						
			WAIT_RECV_DRESQ_S:begin
				if((ptp_recv_type == 4'd4) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					status_ok	<= 1'b1;
				end
				else if((ptp_recv_type != 4'd4) && (ptp_recv_type_valid == 1'b1)) begin
					ptp_ctrl_state	<=	CLOSED_S;
					error	<= 1'b1;
				end
				else ptp_ctrl_state	<= WAIT_RECV_DRESQ_S;
			end
			
			endcase
		end
endmodule