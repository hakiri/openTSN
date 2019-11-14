/*
 * @Author: Wu Shangming
 * @Email: guangming836@163.com
 * @Date: 2019-07-23 23:03:42
 * @LastEditors: Shangming.W
 * @LastEditTime: 2019-07-24 23:31:20
 * @Description: 
   1.The code of fsm.
   2.Statistic related data and information.
   3.More information in Doc.
 */

 //`timescale 1 ns/1 ps
 module fsm #(
     parameter PLATFORM = "Xilinx-OpenBox-S4"
 )(
     input clk,
     input rst_n,

     input cnt_rst,
     
     (*mark_debug="TRUE"*) input [133:0] pktin_data,
     (*mark_debug="TRUE"*) input pktin_data_wr,

     (*mark_debug="TRUE"*) input [103:0] lcm2fsm_5tuple,
     (*mark_debug="TRUE"*) input [103:0] lcm2fsm_5tuplemask,

     //statistic
     (*mark_debug="TRUE"*) output reg [39:0] fsm_byte_num,
     (*mark_debug="TRUE"*) output reg [31:0] fsm_pkt_num
 );

    (*mark_debug="TRUE"*) reg [2:0] fsm_state;
     
     reg [103:0] pkt_5tuple;
     
     reg vlan_flag;
     //reg [103:0] rule_5tuple;
     //reg [103:0] rule_5tuplemask;

    // reg [103:0] tuple_match;
     
    (*mark_debug="TRUE"*) reg [11:0] temp_pkt_byte;


     //assign rule_5tuple = lcm2fsm_5tuple;
     //assign rule_5tuplemask = lcm2fsm_5tuplemask;

     localparam IDLE_S = 3'd0,
                GET_FASTMD1_S = 3'd1,
                WAIT_ETH_PKTHEAD_S = 3'd2,
                GET_PROTOCOL_IP_S = 3'd3,
                GET_IP_PORT_S = 3'd4,
                MATCH_5TUPLE_UPDATE_CNT_S = 3'd5;
               // UPDATE_CNT_S = 3'd6;

     always@(posedge clk or negedge rst_n)begin
          if(!rst_n)begin
              fsm_byte_num <=40'b0;
              fsm_pkt_num <=32'b0;
              
              pkt_5tuple <=104'b0;
              vlan_flag <=1'b0;

              fsm_state <=IDLE_S;
          end 
          else begin
              case(fsm_state)
                   IDLE_S:begin
                      if(cnt_rst==1'b0)begin
                         if((pktin_data[133:132]==2'b01) && (pktin_data_wr==1'b1))begin
                             temp_pkt_byte <= pktin_data[107:96];
                             fsm_state <=GET_FASTMD1_S;
                              pkt_5tuple <=104'b0;
                          end
                          else begin
                             fsm_state <=IDLE_S;
                           end
                       end
                       else begin
                           fsm_pkt_num <=32'b0;
                           fsm_byte_num <=40'b0;                     
                       end
                     end  
                          
                   GET_FASTMD1_S:begin
                          fsm_state <=WAIT_ETH_PKTHEAD_S;
                     end  
                            
                   WAIT_ETH_PKTHEAD_S:begin
                          if(pktin_data[31:16]==16'h8100)begin
                              vlan_flag <=1'b1;
                              fsm_state <=GET_PROTOCOL_IP_S;
                          end
                          else if(pktin_data[31:16]==16'h0800)begin
                              vlan_flag <=1'b0;
                              fsm_state <=GET_PROTOCOL_IP_S;
                          end
                               else begin
                                    fsm_state <=IDLE_S;
                               end
                     end

                   GET_PROTOCOL_IP_S:begin
                          if((vlan_flag==1'b1)&&(pktin_data[127:112]==16'h0800)&&((pktin_data[39:32]==8'h06)||(pktin_data[39:32]==8'h11)))begin
                               pkt_5tuple[39:32] <=pktin_data[39:32];  //protocal
                               pkt_5tuple[103:88] <=pktin_data[15:0];  //source ip
                               
                               fsm_state <=GET_IP_PORT_S;
                          end
                          else if((vlan_flag==1'b0)&&((pktin_data[71:64]==8'h06)||(pktin_data[71:64]==8'h11)))begin   //vlan_flag=0 TCP or UDP
                               pkt_5tuple[39:32] <=pktin_data[71:64];   //protocal
                               pkt_5tuple[103:72] <=pktin_data[47:16];  //source ip
                               pkt_5tuple[71:56] <=pktin_data[15:0];    //dst ip
        
                               fsm_state <=GET_IP_PORT_S;
                          end
                               else begin
                                     fsm_state <=IDLE_S;
                               end
                     end  
                          
                   GET_IP_PORT_S:begin
                          if(vlan_flag==1'b1)begin
                               pkt_5tuple[87:72] <=pktin_data[127:112]; //source ip
                               pkt_5tuple[71:40] <=pktin_data[111:80];  //dst ip
                               pkt_5tuple[31:16] <=pktin_data[79:64];   //source port
                               pkt_5tuple[15:0] <=pktin_data[63:48];    //dst port
                               
                               fsm_state <=MATCH_5TUPLE_UPDATE_CNT_S;
                          end
                          else begin
                               pkt_5tuple[55:40] <=pktin_data[127:112];
                               pkt_5tuple[31:16] <=pktin_data[111:96];
                               pkt_5tuple[15:0] <=pktin_data[95:80];

                               fsm_state <=MATCH_5TUPLE_UPDATE_CNT_S;
                          end
                     end  
                           
                   MATCH_5TUPLE_UPDATE_CNT_S:begin
                         // tuple_match <=((pkt_5tuple^rule_5tuple)&rule_5tuplemask);
                          if(|((pkt_5tuple^lcm2fsm_5tuple)&lcm2fsm_5tuplemask)==1'b0)begin
                               fsm_byte_num <= (fsm_byte_num+temp_pkt_byte-12'd32);
                               fsm_pkt_num <= fsm_pkt_num+32'd1;                               

                               fsm_state <= IDLE_S;  
                          end
                          else begin
                               fsm_state <= IDLE_S;
                          end    
                     end  
                          
                  
               endcase    
           end
       end           
 endmodule