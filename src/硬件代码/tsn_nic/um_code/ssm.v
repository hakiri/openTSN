/*
 * @Author: Wu Shangming
 * @Email: guangming836@163.com
 * @Date: 2019-08-10 23:03:42
 * @LastEditors: Shangming.W
 * @LastEditTime: 2019-8-13 23:31:20
 * @Description: 
   1.The code of fsm.
   2.Statistic related data and information.
   3.More information in Doc.
 */

 //`timescale 1 ns/1 ps
 module ssm#(
     parameter PLATFORM = "Xilinx-Openbox-S4"
 )(
     input clk,
     input rst_n,

     input [133:0] pktin_data,
     input pktin_data_wr,

     input [15:0] sample_freq,
     input [47:0] lcm2ssm_time,

     input cnt_rst,

     output wire [133:0] pktout_data,
     output wire pktout_data_wr,
     output wire pktout_data_valid,
     output wire pktout_data_valid_wr,
     output wire [31:0] pkt_num
 );
     
     wire [133:0] encap2sample_data;
     wire encap2sample_data_wr;

//***************************************************
//                  Module Instance
//***************************************************
     ssm_encap ssm_encap_inst(
      .clk(clk),
      .rst_n(rst_n),
      .pktin_encap_data(pktin_data),
      .pktin_encap_data_wr(pktin_data_wr),
       
       //from encap
      .pktout_encap_data(encap2sample_data),
      .pktout_encap_data_wr(encap2sample_data_wr)
     );

     ssm_sample ssm_sample_inst(
      .clk(clk),
      .rst_n(rst_n),
      .cnt_rst(cnt_rst),

      // To sample
      .pktin_sample_data(encap2sample_data),
      .pktin_sample_data_wr(encap2sample_data_wr),

      .sample_freq(sample_freq),
      .lcm2ssm_sample_time(lcm2ssm_time),

      .pktout_sample_data(pktout_data),
      .pktout_sample_data_wr(pktout_data_wr),
      .pktout_sample_data_valid(pktout_data_valid),
      .pktout_sample_data_valid_wr(pktout_data_valid_wr),
      .pkt_num(pkt_num)
     );
 endmodule

