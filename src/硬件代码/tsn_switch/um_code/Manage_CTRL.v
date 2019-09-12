module Manage_CTRL#(
    parameter LMID = 8'd8
)(
    input wire clk,
	input wire rst_n,
	
	input cfg_cs_n, //low active
	output reg cfg_ack_n, //low active
	input cfg_rw, //0 :write, 1 :read
	input [31:0] cfg_addr,
	input [31:0] cfg_wdata,
	output reg [31:0] cfg_rdata,
	
	output reg sync_start,
	output reg [1:0]device_role,
	input error,
	input [47:0]offset,
	input	[47:0]	temp_cnt,
	input [47:0]rtt_time,
	output reg [47:0]device_mac,
//	output reg [30:0]test_time,
	input error1,
	output reg [31:0]sync_cnt,
	output reg [47:0]device_id
);
reg [31:0] address;

reg [2:0] ctrl_state;
localparam IDLE_S  = 3'd1,
           WRITE_S = 3'd2,
		   READ_S  = 3'd3,
		   ACK_S  = 3'd4;


wire cs_n;
sync_sig sync_inst(
    .clk(clk),
	.rst_n(rst_n),
	.in_sig(~cfg_cs_n),
	.out_sig(cs_n)
);           	   
always @ ( posedge clk or negedge rst_n ) begin
    if(rst_n == 1'b0) begin
	   cfg_ack_n <= 1'b1;
	   cfg_rdata <= 32'b0;
	   sync_start <= 1'b0;
	   device_role <= 2'b0;
	   device_mac <= 48'h000606000000;
	   device_id<= 48'h000606000000;
	   address <= 32'b0;
	   sync_cnt<=32'd12500;
	   ctrl_state <= IDLE_S;   
	end
	else begin
	   case(ctrl_state)
	   IDLE_S:begin
	      cfg_ack_n <= 1'b1;
	      cfg_rdata <= 32'b0;
           if((cs_n == 1'b1) && (cfg_ack_n == 1'b1)) begin
			if(cfg_rw == 1'b0) begin    //write
			     ctrl_state <= WRITE_S;
			  end
			  else begin                      //read
			     ctrl_state <= READ_S;
			  end
			  address<=cfg_addr;
		   end
		   else begin
		      ctrl_state <= IDLE_S;
		   end
		
		end
	   WRITE_S: begin
	      case(address[9:2])
		   32'd0:begin 
		   device_mac[7:0] <= cfg_wdata[7:0]; 
		   device_id[7:0]  <= cfg_wdata[7:0]; 
		   if(cfg_wdata[7:0]==8'b0)begin
		     device_role<=2'b11;	
		     sync_start<=1'b1;	   
		   end
		   else begin
		     device_role<=2'b10;
		     sync_start<=1'b1;	
		   end
		   end
	//	   32'd12:begin test_time<=cin_reg0[30:0];end
		   default:begin
		      sync_start <= sync_start;
			  device_role <= device_role;
			  device_mac <= device_mac;
		   end
		   endcase
		   ctrl_state <= ACK_S;
	   end
	   
	   READ_S :begin
		  case(address[9:2])
		  32'd0:begin  cfg_rdata[31:0]<={31'b0,sync_start};   end
		  32'd1:begin  cfg_rdata[31:0]<={30'b0,device_role}; end
		  32'd2:begin  cfg_rdata[31:0]<=device_mac[47:16]; end
		  32'd3:begin  cfg_rdata[31:0]<={16'b0,device_mac[15:0]}; end
		  32'd4:begin  cfg_rdata[31:0]<={31'b0,error}; end 
		  32'd5:begin  cfg_rdata[31:0]<={31'b0,error1}; end 
		  32'd6:begin  cfg_rdata[31:0]<=offset[47:17]; end 
		  32'd7:begin  cfg_rdata[31:0]<=offset[16:0]; end 
		  32'd8:begin  cfg_rdata[31:0]<=temp_cnt[47:17]; end 
		  32'd9:begin  cfg_rdata[31:0]<=temp_cnt[16:0]; end 
		  32'd10:begin  cfg_rdata[31:0]<=rtt_time[47:17]; end 
		  32'd11:begin  cfg_rdata[31:0]<=rtt_time[16:0]; end 
		  32'd12:begin cfg_rdata[31:0]<=sync_cnt;  end
		  default:begin cfg_rdata[31:0] <= 32'b0; end
		  endcase		  
	      ctrl_state <= ACK_S;
	   end
	   ACK_S: begin
                if(cs_n == 1'b1)begin										   
					    cfg_ack_n <= 1'b0;
                        ctrl_state <= ACK_S;					
                end				
                else begin
                    cfg_ack_n <= 1'b1;
                    ctrl_state <= IDLE_S;
                end
            end
       default : begin
          ctrl_state <= IDLE_S;
       end	   
	   endcase	
	end
end


endmodule