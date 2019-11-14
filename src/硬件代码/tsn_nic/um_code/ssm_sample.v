/*
 * @Author: Shangming.W
 * @Email: guangming836@163.com
 * @Date: 2019-08-12 23:03:42
 * @LastEditors: Shangming.W
 * @LastEditTime: 2019-08-12 23:31:20
 * @Description: 
   1.The code of ssm_sample.
   2.Statistic related data and information.
   3.More information in Doc.
 */

 //`timescale 1 ns/1 ps
 module ssm_sample #(
     parameter PLATFORM = "Xilinx-OpenBOX-S4"
 )(
     input clk,
     input rst_n,
     input cnt_rst,

     input [133:0] pktin_sample_data,
     input pktin_sample_data_wr,

     input [15:0] sample_freq,
     input [47:0] lcm2ssm_sample_time,

     output reg [133:0] pktout_sample_data,
     output reg pktout_sample_data_wr,
     output reg pktout_sample_data_valid,
     output reg pktout_sample_data_valid_wr,

     output reg [31:0] pkt_num

 );

     reg [2:0] ssm_sample_state;
     reg [15:0] reg_pkt_num;

     localparam IDLE_S = 3'd0,
                TRAN_MD1_S = 3'd1,
                TRAN_ETH_HDR2_S = 3'd2,
                ADD_TIME_S =3'd3,
                TRAN_S = 3'd4;   

           
     always@(posedge clk or negedge rst_n)begin 
          if(!rst_n)begin
               pktout_sample_data <=134'b0;
               pktout_sample_data_wr <=1'b0;
               pktout_sample_data_valid <=1'b0;
               pktout_sample_data_valid_wr <=1'b0;

               pkt_num <=32'b0;
               reg_pkt_num <=16'b0;

               ssm_sample_state <=IDLE_S;
          end 
          else begin
               case(ssm_sample_state)
                    IDLE_S:begin
                    if(cnt_rst==1'b0)begin
                         if((pktin_sample_data[133:132]==2'b01) && (pktin_sample_data_wr==1'b1) && (reg_pkt_num==sample_freq-16'd1))begin
                              reg_pkt_num <=16'b0;
                              pkt_num <=pkt_num+16'd1;

                              pktout_sample_data <=pktin_sample_data;
                              pktout_sample_data_wr <=1'b1;
                              pktout_sample_data_valid <=1'b0;
                              pktout_sample_data_valid_wr <=1'b0;


                              ssm_sample_state <=TRAN_MD1_S;
                         end
                         else if((pktin_sample_data[133:132]==2'b01) && (pktin_sample_data_wr==1'b1) && (reg_pkt_num!=sample_freq-16'd1))begin
                              pkt_num <=pkt_num+16'd1;
                              reg_pkt_num <=reg_pkt_num+16'd1;
                         end
                         else begin            //reg_pkt_num !=sample_freq-16'd1
                              pktout_sample_data <=134'b0;
                              pktout_sample_data_wr <=1'b0;
                              pktout_sample_data_valid <=1'b0;
                              pktout_sample_data_valid_wr <=1'b0;
                              ssm_sample_state <=IDLE_S;
                         end
                     end
                     else begin
                             pkt_num <=32'b0;
                             reg_pkt_num <=16'b0;             
                     end
                 end

                    TRAN_MD1_S:begin
                         pktout_sample_data <=pktin_sample_data;
                         pktout_sample_data_wr <=1'b1;
                         pktout_sample_data_valid <=1'b0;
                         pktout_sample_data_valid_wr <=1'b0;
                         
                         ssm_sample_state <=TRAN_ETH_HDR2_S;               
                    end

                    TRAN_ETH_HDR2_S:begin
                         pktout_sample_data <=pktin_sample_data;
                         pktout_sample_data_wr <=1'b1;
                         pktout_sample_data_valid <=1'b0;
                         pktout_sample_data_valid_wr <=1'b0;

                         ssm_sample_state <=ADD_TIME_S;   
                    end

                    ADD_TIME_S:begin
                         pktout_sample_data <={pktin_sample_data[133:48], lcm2ssm_sample_time};
                         pktout_sample_data_wr <=1'b1;
                         pktout_sample_data_valid <=1'b0;
                         pktout_sample_data_valid_wr <=1'b0;

                         ssm_sample_state <=TRAN_S;  
                    end
          
                    TRAN_S:begin
                         if(pktin_sample_data[133:132]==2'b10)begin
                              pktout_sample_data <=pktin_sample_data;
                              pktout_sample_data_wr <=1'b1;
                              pktout_sample_data_valid <=1'b1;
                              pktout_sample_data_valid_wr <=1'b1;

                              ssm_sample_state <=IDLE_S;
                         end
                         else begin
                              pktout_sample_data <=pktin_sample_data;
                              pktout_sample_data_wr <=1'b1;
                              pktout_sample_data_valid <=1'b0;
                              pktout_sample_data_valid_wr <=1'b0;

                              ssm_sample_state <=TRAN_S;
                         end
                    end
               endcase
          end   
      end
 endmodule
