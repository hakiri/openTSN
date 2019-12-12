
/*
 *@Author: Wu Shangming
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
 module ssm_encap #(
     parameter PLATFORM = "Xilinx-OpenBox-S4"
 ) (
    input clk,
    input rst_n,

    input [133:0] pktin_encap_data,
    input pktin_encap_data_wr,

    output reg[133:0] pktout_encap_data,
    output reg pktout_encap_data_wr

 );
    reg [2:0] ssm_encap_state;

    reg [103:0] pkt_5tuple;

    reg [1:0] vlan_flag;

    reg [133:0] register1;
    reg [133:0] register2;
    reg [133:0] register3;
     //reg [133:0] register4;
     //reg [133:0] register5;

    localparam IDLE_S = 3'd0,
               ENCAP_MD1_S = 3'd1,
               ENCAP_ETH_HDR2_S = 3'd2,
               GET_PROTOCOL_IP_TRANSMD0_S = 3'd3,
               GET_IP_PORT_TRANSMD1_S = 3'd4,
               TRAN_S =3'd5;

    always@(posedge clk or negedge rst_n)begin
          if(!rst_n)begin
              pktout_encap_data <=134'b0;
              pktout_encap_data_wr <=1'b0;

              pkt_5tuple <=104'b0;
              vlan_flag <=2'b00;

              register1 <=134'b0;
              register2 <=134'b0;
              register3 <=134'b0;
              

              ssm_encap_state <=IDLE_S;
          end
          else begin
               case(ssm_encap_state)
                    IDLE_S:begin
                        if((pktin_encap_data[133:132]==2'b01)&&(pktin_encap_data_wr==1'b1))begin
                            pktout_encap_data <={6'b010000, 20'h00000, pktin_encap_data[107:96]+12'd48, 96'b0};    //encap FAST hdr2 MD0
                            pktout_encap_data_wr <=1'b1;
                            register1 <={6'b110000, pktin_encap_data[127:0]};

                            ssm_encap_state <=ENCAP_MD1_S;
                        end
                        else begin
                            pktout_encap_data <=134'b0;
                            pktout_encap_data_wr <=1'b0;

                            ssm_encap_state <=IDLE_S;
                        end
                    end

                    ENCAP_MD1_S:begin
                        if((pktin_encap_data[133:132]==2'b11)&&(pktin_encap_data_wr==1'b1))begin
                            pktout_encap_data <={6'b110000, 128'b0};                                   //encap FAST hdr2 MD1
                            pktout_encap_data_wr <=1'b1;

                            register2 <=register1;
                            register1 <=pktin_encap_data;
                            
                            ssm_encap_state <=ENCAP_ETH_HDR2_S;
                        end 
                        else begin
                            ssm_encap_state <=IDLE_S;
                        end
                    end
 
                   ENCAP_ETH_HDR2_S:begin
                         if((pktin_encap_data[133:132]==2'b11) && (pktin_encap_data_wr==1'b1)&&(pktin_encap_data[31:16]==16'h8100))begin
                           pktout_encap_data <={6'b110000, 48'hffff_ffff_ffff, 48'h0000_0000_0000, 16'hff03, 16'h0000};        //encap ETH hdr2
                           pktout_encap_data_wr <=1'b1;

                           vlan_flag <=2'b01;
  
                           register3 <=register2;
                           register2 <=register1;
                           register1 <=pktin_encap_data;

                           ssm_encap_state <=GET_PROTOCOL_IP_TRANSMD0_S;
                       end
                       else if((pktin_encap_data[133:132]==2'b11) && (pktin_encap_data_wr==1'b1)&&(pktin_encap_data[31:16]==16'h0800))begin
                           pktout_encap_data <={6'b110000, 48'hffff_ffff_ffff, 48'h0000_0000_0000, 16'hff03, 16'h0000};       //encap ETH hdr2
                           pktout_encap_data_wr <=1'b1;

                           vlan_flag <=2'b00;

                           register3 <=register2;
                           register2 <=register1;
                           register1 <=pktin_encap_data;

                           ssm_encap_state <=GET_PROTOCOL_IP_TRANSMD0_S;
                       end
                       else begin
                           pktout_encap_data <={6'b110000, 48'hffff_ffff_ffff, 48'h0000_0000_0000, 16'hff03, 16'h0000};       //encap ETH hdr2
                           pktout_encap_data_wr <=1'b1;
                           vlan_flag <=2'b11;
                           
                           register3 <=register2;
                           register2 <=register1;
                           register1 <=pktin_encap_data;
                           ssm_encap_state <= GET_PROTOCOL_IP_TRANSMD0_S;
                       end
                    end

                   GET_PROTOCOL_IP_TRANSMD0_S:begin
                         if((vlan_flag==2'b01)&&(pktin_encap_data[127:112]==16'h0800)&&((pktin_encap_data[39:32]==8'h06)||(pktin_encap_data[39:32]==8'h11)))begin  
                               pkt_5tuple[39:32]  <=pktin_encap_data[39:32];  //protocal
                               pkt_5tuple[103:88] <=pktin_encap_data[15:0];  //source ip
                               
                               pktout_encap_data <=register3;
                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data;

                               ssm_encap_state <= GET_IP_PORT_TRANSMD1_S;
                         end
                         else if((vlan_flag==2'b00)&&((pktin_encap_data[71:64]==8'h06)||(pktin_encap_data[71:64]==8'h11)))begin          //vlan_flag=00 TCP or UDP
                               pkt_5tuple[39:32]  <=pktin_encap_data[71:64];   //protocal
                               pkt_5tuple[103:72] <=pktin_encap_data[47:16];  //source ip
                               pkt_5tuple[71:56]  <=pktin_encap_data[15:0];    //dst ip
                               
                               pktout_encap_data <=register3;
                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data;

                               ssm_encap_state <= GET_IP_PORT_TRANSMD1_S;
                         end
                              else begin
                                     pktout_encap_data <=register3;
                                     register3 <=register2;
                                     register2 <=register1;
                                     register1 <=pktin_encap_data;
                                     ssm_encap_state <=GET_IP_PORT_TRANSMD1_S;
                              end
                    end
                    
                    GET_IP_PORT_TRANSMD1_S:begin
                          if(vlan_flag==2'b01)begin
                               pktout_encap_data[133:104] <={6'b110000, 24'h0000_00};
                          
                               pktout_encap_data[87:72] <=pktin_encap_data[127:112]; //source ip
                               pktout_encap_data[71:40] <=pktin_encap_data[111:80];  //dst ip
                               pktout_encap_data[31:16] <=pktin_encap_data[79:64];   //source port
                               pktout_encap_data[15:0]  <=pktin_encap_data[63:48];    //dst port

                               pktout_encap_data[39:32]  <=pkt_5tuple[39:32];  //protocal
                               pktout_encap_data[103:88] <=pkt_5tuple[103:88];   //source ip

                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data;

                               ssm_encap_state <=TRAN_S;
                          end
                          else if(vlan_flag==2'b00) begin
                               pktout_encap_data[133:104] <={6'b110000, 24'h0000_00};
                          
                               pktout_encap_data[55:40]  <=pktin_encap_data[127:112];    //dst ip
                               pktout_encap_data[31:16]  <=pktin_encap_data[111:96];     //source port
                               pktout_encap_data[15:0]  <=pktin_encap_data[95:80];      //dst port

                               pktout_encap_data[39:32]  <=pkt_5tuple[39:32];
                               pktout_encap_data[103:72]  <=pkt_5tuple[103:72];
                               pktout_encap_data[71:56]  <=pkt_5tuple[71:56];

                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data;
                               
                               ssm_encap_state <=TRAN_S;
                          end
                          else begin
                               pktout_encap_data <=register3;
                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data;

                               ssm_encap_state <=TRAN_S;
                          end
                     end
                    
                    TRAN_S:begin
                          if(register3[133:132]==2'b10)begin
                               pktout_encap_data <=register3;
                               pktout_encap_data_wr <=1'b1;

                               register3 <=134'b0;
                         // latch2 <=latch1;
                         // latch1 <=pktin_encap_data; 

                               ssm_encap_state <=IDLE_S;
                           end
                           else begin
                               register3 <=register2;
                               register2 <=register1;
                               register1 <=pktin_encap_data; 
                          
                               pktout_encap_data <=register3;
                               pktout_encap_data_wr <=1'b1;
                         
                               ssm_encap_state <=TRAN_S;
                         end 
                    end
               endcase
          end
    end           
 endmodule