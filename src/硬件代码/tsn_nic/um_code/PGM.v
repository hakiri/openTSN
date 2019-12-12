/////////////////////////////////////////////////////////////////
// NUDT.  All rights reserved.
//*************************************************************
//                     Basic Information
//*************************************************************
//Vendor: NUDT
//FAST URL://www.fastswitch.org 
//Target Device: Xilinx
//Filename: PGM.v
//Version: 1.0
//date: 2019/08/10
//Author : Peng Jintao
//*************************************************************
//                     Module Description
//*************************************************************
// PGM(Packet Generation Module):top module.
//     
//*************************************************************
//                     Revision List
//*************************************************************
//	rn1: 
//      date:
//      modifier: 
//      description: 
//////////////////////////////////////////////////////////////
module PGM#(
    parameter    PLATFORM = "xilinx"
)(
//clk & rst
input			clk,
input			rst_n,

input           cnt_rst,

input           lau_update_finish,
//receive from LCM
input     [47:0] timestamp,
input            in_pgm_addr_shift,
input            in_pgm_slot_shift, 
input	         in_pgm_test_start,       
input            test_stop, 
input     [11:0] in_pgm_pkt_1_len,
input     [11:0] in_pgm_pkt_2_len,
input     [11:0] in_pgm_pkt_3_len,
input     [11:0] in_pgm_pkt_4_len,
input     [11:0] in_pgm_pkt_5_len,
input     [11:0] in_pgm_pkt_6_len,
input     [11:0] in_pgm_pkt_7_len,
input     [11:0] in_pgm_pkt_8_len,

input	  [15:0] in_pgm_tb_1_size, 
input     [15:0] in_pgm_tb_1_rate,
input	  [15:0] in_pgm_tb_2_size, 
input     [15:0] in_pgm_tb_2_rate,
input	  [15:0] in_pgm_tb_3_size, 
input     [15:0] in_pgm_tb_3_rate,
input	  [15:0] in_pgm_tb_4_size, 
input     [15:0] in_pgm_tb_4_rate,
input	  [15:0] in_pgm_tb_5_size, 
input     [15:0] in_pgm_tb_5_rate,
input	  [15:0] in_pgm_tb_6_size, 
input     [15:0] in_pgm_tb_6_rate,
input	  [15:0] in_pgm_tb_7_size, 
input     [15:0] in_pgm_tb_7_rate,
input	  [15:0] in_pgm_tb_8_size, 
input     [15:0] in_pgm_tb_8_rate,

input            gcl_ram_rd,
input     [3:0]  slot_shift_cnt, 
input     [8:0]  slot_ID,

//transmit to LCM
output     [31:0] out_pgm_pkt_1_cnt,
output     [31:0] out_pgm_pkt_2_cnt,
output     [31:0] out_pgm_pkt_3_cnt,
output     [31:0] out_pgm_pkt_4_cnt,
output     [31:0] out_pgm_pkt_5_cnt,
output     [31:0] out_pgm_pkt_6_cnt,
output     [31:0] out_pgm_pkt_7_cnt,
output     [31:0] out_pgm_pkt_8_cnt,

//receive from UDO
input     [7:0]  in_pgm_fifo_usedw, 

//receive from GCL_RAM
output           out_pgm_gcl_rd,   
output    [4:0]  out_pgm_gcl_addr,    
input	  [127:0]in_pgm_gc,         

//receive from PKT_HDR_RAM
output	         out_pgm_pkt_hdr_rd,
output	  [5:0]  out_pgm_pkt_hdr_addr,
input	  [127:0]in_pgm_pkt_hdr,

//transmit to FPGA OS
(*mark_debug="TRUE"*)output	   [133:0]out_pgm_data,
(*mark_debug="TRUE"*)output	          out_pgm_data_wr,
(*mark_debug="TRUE"*)output	          out_pgm_data_valid,
(*mark_debug="TRUE"*)output	          out_pgm_data_valid_wr
);

//***************************************************
//        Intermediate Variable Declaration
//***************************************************

//TSM to DRM,TGM,PHE
wire [7:0]	tsm_selected;  
//TGM to GCM
wire        tgr2gcm_req_1;
wire        tgr2gcm_req_2;
wire        tgr2gcm_req_3;
wire        tgr2gcm_req_4;
wire        tgr2gcm_req_5;
wire        tgr2gcm_req_6;
wire        tgr2gcm_req_7;
wire        tgr2gcm_req_8;
//GCM to TSM
wire [7:0]  gcm2tsm_valid;

//DRM to PHE
wire [127:0]drm2phe_pkt_hdr;
wire        drm2phe_pkt_hdr_wr;

//***************************************************
//                  Module Instance
//***************************************************
TGR TGR_1_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_1_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_1_size),
.in_tgr_tb_rate(in_pgm_tb_1_rate),   
.in_tgr_selected(tsm_selected[0]),
.out_tgr_req(tgr2gcm_req_1)
);
TGR TGR_2_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_2_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_2_size),
.in_tgr_tb_rate(in_pgm_tb_2_rate),   
.in_tgr_selected(tsm_selected[1]),
.out_tgr_req(tgr2gcm_req_2)
);
TGR TGR_3_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_3_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_3_size),
.in_tgr_tb_rate(in_pgm_tb_3_rate),   
.in_tgr_selected(tsm_selected[2]),
.out_tgr_req(tgr2gcm_req_3)
);
TGR TGR_4_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_4_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_4_size),
.in_tgr_tb_rate(in_pgm_tb_4_rate),   
.in_tgr_selected(tsm_selected[3]),
.out_tgr_req(tgr2gcm_req_4)
);
TGR TGR_5_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_5_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_5_size),
.in_tgr_tb_rate(in_pgm_tb_5_rate),   
.in_tgr_selected(tsm_selected[4]),
.out_tgr_req(tgr2gcm_req_5)
);
TGR TGR_6_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_6_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_6_size),
.in_tgr_tb_rate(in_pgm_tb_6_rate),   
.in_tgr_selected(tsm_selected[5]),
.out_tgr_req(tgr2gcm_req_6)
);
TGR TGR_7_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_7_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_7_size),
.in_tgr_tb_rate(in_pgm_tb_7_rate),   
.in_tgr_selected(tsm_selected[6]),
.out_tgr_req(tgr2gcm_req_7)
);
TGR TGR_8_inst(
.clk(clk),
.rst_n(rst_n), 
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),
.in_tgr_pkt_len(in_pgm_pkt_8_len),
.in_tgr_slot_shift(in_pgm_slot_shift),
.in_tgr_tb_size(in_pgm_tb_8_size),
.in_tgr_tb_rate(in_pgm_tb_8_rate),   
.in_tgr_selected(tsm_selected[7]),
.out_tgr_req(tgr2gcm_req_8)
);

GCM GCM_inst(
.clk(clk),
.rst_n(rst_n),
.lau_update_finish(lau_update_finish),
.test_stop(test_stop),  
.in_gcm_test_start(in_pgm_test_start),  
//.in_gcm_test_stop(in_pgm_test_stop), 
.slot_shift_cnt(slot_shift_cnt),
.gcl_ram_rd(gcl_ram_rd),
.in_gcm_req_1(tgr2gcm_req_1),
.in_gcm_req_2(tgr2gcm_req_2),  
.in_gcm_req_3(tgr2gcm_req_3),
.in_gcm_req_4(tgr2gcm_req_4),
.in_gcm_req_5(tgr2gcm_req_5),
.in_gcm_req_6(tgr2gcm_req_6),
.in_gcm_req_7(tgr2gcm_req_7),
.in_gcm_req_8(tgr2gcm_req_8),
.out_gcm_gcl_rd(out_pgm_gcl_rd),
.out_gcm_gcl_addr(out_pgm_gcl_addr),
.in_gcm_gc(in_pgm_gc),
.out_gcm_valid(gcm2tsm_valid)
);

TSM TSM_inst(
.clk(clk),
.rst_n(rst_n),
.in_tsm_test_start(in_pgm_test_start),
.in_tsm_valid(gcm2tsm_valid),
.in_tsm_outport_free(out_pgm_data_valid),       
.in_tsm_fifo_usedw(in_pgm_fifo_usedw),
.out_tsm_selected(tsm_selected)
);

DRM DRM_inst(
.clk(clk),
.rst_n(rst_n),
.in_drm_addr_shift(in_pgm_addr_shift),
.in_drm_selected(tsm_selected),  
.out_drm_pkt_hdr_rd(out_pgm_pkt_hdr_rd),
.out_drm_pkt_hdr_addr(out_pgm_pkt_hdr_addr),
.in_drm_pkt_hdr(in_pgm_pkt_hdr),     
.out_drm_pkt_hdr(drm2phe_pkt_hdr),
.out_drm_pkt_hdr_wr(drm2phe_pkt_hdr_wr)
);

PHE PHE_inst(
.clk(clk),
.rst_n(rst_n),
.cnt_rst(cnt_rst),
.in_phe_test_start(in_pgm_test_start),
.timestamp(timestamp),
.slot_shift_cnt(slot_shift_cnt),
.slot_ID(slot_ID),
.in_phe_pkt_1_len(in_pgm_pkt_1_len),
.in_phe_pkt_2_len(in_pgm_pkt_2_len),
.in_phe_pkt_3_len(in_pgm_pkt_3_len),
.in_phe_pkt_4_len(in_pgm_pkt_4_len),
.in_phe_pkt_5_len(in_pgm_pkt_5_len),
.in_phe_pkt_6_len(in_pgm_pkt_6_len),
.in_phe_pkt_7_len(in_pgm_pkt_7_len),
.in_phe_pkt_8_len(in_pgm_pkt_8_len),
.out_phe_pkt_1_cnt(out_pgm_pkt_1_cnt),
.out_phe_pkt_2_cnt(out_pgm_pkt_2_cnt),
.out_phe_pkt_3_cnt(out_pgm_pkt_3_cnt),
.out_phe_pkt_4_cnt(out_pgm_pkt_4_cnt),
.out_phe_pkt_5_cnt(out_pgm_pkt_5_cnt),
.out_phe_pkt_6_cnt(out_pgm_pkt_6_cnt),
.out_phe_pkt_7_cnt(out_pgm_pkt_7_cnt),
.out_phe_pkt_8_cnt(out_pgm_pkt_8_cnt),
.in_phe_selected(tsm_selected),
.in_phe_pkt_hdr(drm2phe_pkt_hdr),
.in_phe_pkt_hdr_wr(drm2phe_pkt_hdr_wr),
.out_phe_data(out_pgm_data),
.out_phe_data_wr(out_pgm_data_wr),
.out_phe_data_valid(out_pgm_data_valid),
.out_phe_data_valid_wr(out_pgm_data_valid_wr)
);

endmodule



