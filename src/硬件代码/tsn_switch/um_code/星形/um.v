module um(
input  wire  clk,
input  wire  rst_n,

input  wire  [133:0]pktin_data,
input  wire  pktin_data_wr,
input  wire  pktin_valid,
input  wire  pktin_valid_wr,
output wire  pktin_ready,

input  wire  cfg_cs_n, //low active
output wire  cfg_ack_n, //low active
input  wire  cfg_rw, //0 :write, 1 :read
input  wire  [31:0] cfg_addr,
input  wire  [31:0] cfg_wdata,
output wire  [31:0] cfg_rdata, 
output wire  cyc_init,
output wire  [47:0]temp_cnt,

input  wire  [7:0]pktout_usedw_0,
input  wire  [7:0]pktout_usedw_1,
input  wire  [7:0]pktout_usedw_2,
input  wire  [7:0]pktout_usedw_3,

output wire  [133:0]pktout_data_0,
output wire  pktout_data_wr_0,
output wire  pktout_valid_0,
output wire  pktout_valid_wr_0,

output wire  [133:0]pktout_data_1,
output wire  pktout_data_wr_1,
output wire  pktout_valid_1,
output wire  pktout_valid_wr_1,

output wire  [133:0]pktout_data_2,
output wire  pktout_data_wr_2,
output wire  pktout_valid_2,
output wire  pktout_valid_wr_2,

output wire  [133:0]pktout_data_3,
output wire  pktout_data_wr_3,
output wire  pktout_valid_3,
output wire  pktout_valid_wr_3
);
//PTP to lcm//
wire   [47:0]precision_time;
wire   [47:0]device_id;

wire   ptp2lcm_data_wr;
wire   [133:0]ptp2lcm_data;
wire   ptp2lcm_data_valid;
wire   ptp2lcm_data_valid_wr;
wire   ptp2lcm_ready;

//lcm to esw//
wire  [133:0]lcm2esw_data;
wire  lcm2esw_data_wr;
wire  lcm2esw_valid;
wire  lcm2esw_valid_wr;

wire  [47:0]MAC_0;
wire  [47:0]MAC_1;
wire  [47:0]MAC_2;
wire  [47:0]MAC_3;
wire  [47:0]MAC_4;
wire  [47:0]MAC_5;
wire  [47:0]MAC_6;
wire  [47:0]MAC_7;
wire  [47:0]MAC_8;
wire  [47:0]MAC_9;
wire  [47:0]MAC_10;
wire  [47:0]MAC_11;

wire  [15:0]port_0;
wire  [15:0]port_1;
wire  [15:0]port_2;
wire  [15:0]port_3;
wire  [15:0]port_4;
wire  [15:0]port_5;
wire  [15:0]port_6;
wire  [15:0]port_7;
wire  [15:0]port_8; 
wire  [15:0]port_9; 
wire  [15:0]port_10;
wire  [15:0]port_11;

wire  [63:0]esw_pktout_cnt_0;
wire  [63:0]esw_pktout_cnt_1;
wire  [63:0]esw_pktout_cnt_2;
wire  [63:0]esw_pktout_cnt_3;

wire  [7:0]bufm_ID_cnt_0;
wire  [7:0]bufm_ID_cnt_1;
wire  [7:0]bufm_ID_cnt_2;
wire  [7:0]bufm_ID_cnt_3;

wire  [7:0]eos_q0_used_cnt_0;
wire  [7:0]eos_q1_used_cnt_0;
wire  [7:0]eos_q2_used_cnt_0;
wire  [7:0]eos_q3_used_cnt_0;
       
wire  [7:0]eos_q0_used_cnt_1;
wire  [7:0]eos_q1_used_cnt_1;
wire  [7:0]eos_q2_used_cnt_1;
wire  [7:0]eos_q3_used_cnt_1;
       
wire  [7:0]eos_q0_used_cnt_2;
wire  [7:0]eos_q1_used_cnt_2;
wire  [7:0]eos_q2_used_cnt_2;
wire  [7:0]eos_q3_used_cnt_2;
       
wire  [7:0]eos_q0_used_cnt_3;
wire  [7:0]eos_q1_used_cnt_3;
wire  [7:0]eos_q2_used_cnt_3;
wire  [7:0]eos_q3_used_cnt_3;

wire  [63:0]eos_mdin_cnt_0;
wire  [63:0]eos_mdin_cnt_1;
wire  [63:0]eos_mdin_cnt_2;
wire  [63:0]eos_mdin_cnt_3;
wire  [63:0]eos_mdout_cnt_0;
wire  [63:0]eos_mdout_cnt_1;
wire  [63:0]eos_mdout_cnt_2;
wire  [63:0]eos_mdout_cnt_3;

//esw to ibm//
wire  [133:0]esw2ibm_data_0;
wire  esw2ibm_data_wr_0;
wire  esw2ibm_valid_0;
wire  esw2ibm_valid_wr_0;
wire  [23:0]tsn_md_0;
wire  tsn_md_wr_0;
wire [4:0]ibm2esw_bufm_ID_0;

wire  [133:0]esw2ibm_data_1;
wire  esw2ibm_data_wr_1;
wire  esw2ibm_valid_1;
wire  esw2ibm_valid_wr_1;
wire  [23:0]tsn_md_1;
wire  tsn_md_wr_1;
wire [4:0]ibm2esw_bufm_ID_1;

wire  [133:0]esw2ibm_data_2;
wire  esw2ibm_data_wr_2;
wire  esw2ibm_valid_2;
wire  esw2ibm_valid_wr_2;
wire  [23:0]tsn_md_2;
wire  tsn_md_wr_2;
wire [4:0]ibm2esw_bufm_ID_2;

wire  [133:0]esw2ibm_data_3;
wire  esw2ibm_data_wr_3;
wire  esw2ibm_valid_3;
wire  esw2ibm_valid_wr_3;
wire  [23:0]tsn_md_3;
wire  tsn_md_wr_3;
wire [4:0]ibm2esw_bufm_ID_3;

//ibm to data_cache//
wire [133:0]out_ibm_data_0;
wire out_ibm_data_wr_0;
wire out_ibm_valid_0;
wire out_ibm_valid_wr_0;
wire [7:0]in_ibm_ID_0;
wire [4:0]in_ibm_ID_count_0;

wire [133:0]out_ibm_data_1;
wire out_ibm_data_wr_1;
wire out_ibm_valid_1;
wire out_ibm_valid_wr_1;
wire [7:0]in_ibm_ID_1;
wire [4:0]in_ibm_ID_count_1;

wire [133:0]out_ibm_data_2;
wire out_ibm_data_wr_2;
wire out_ibm_valid_2;
wire out_ibm_valid_wr_2;
wire [7:0]in_ibm_ID_2;
wire [4:0]in_ibm_ID_count_2;

wire [133:0]out_ibm_data_3;
wire out_ibm_data_wr_3;
wire out_ibm_valid_3;
wire out_ibm_valid_wr_3;
wire [7:0]in_ibm_ID_3;
wire [4:0]in_ibm_ID_count_3;

//ibm to eos//
wire [23:0]out_ibm_md_0;
wire out_ibm_md_wr_0;
 
wire [23:0]out_ibm_md_1;
wire out_ibm_md_wr_1;

wire [23:0]out_ibm_md_2;
wire out_ibm_md_wr_2;

wire [23:0]out_ibm_md_3;
wire out_ibm_md_wr_3;
 
//eos to ebm//
wire [7:0]out_eos_tsn_md_0;
wire out_eos_tsn_md_wr_0;

wire [7:0]out_eos_tsn_md_1;
wire out_eos_tsn_md_wr_1;

wire [7:0]out_eos_tsn_md_2;
wire out_eos_tsn_md_wr_2;

wire [7:0]out_eos_tsn_md_3;
wire out_eos_tsn_md_wr_3;

wire in_ebm_bandwidth_discard_0;
wire in_ebm_bandwidth_discard_1;
wire in_ebm_bandwidth_discard_2;
wire in_ebm_bandwidth_discard_3;

//data_cache to ebm// 
wire out_data_cache_data_wr_0;
wire [133:0]out_data_cache_data_0;
wire out_data_cache_valid_wr_0;
wire out_data_cache_valid_0;
wire [7:0]in_data_cache_ID_0;
wire in_data_cache_ID_wr_0;

wire out_data_cache_data_wr_1;
wire [133:0]out_data_cache_data_1;
wire out_data_cache_valid_wr_1;
wire out_data_cache_valid_1;
wire [7:0]in_data_cache_ID_1;
wire in_data_cache_ID_wr_1;

wire out_data_cache_data_wr_2;
wire [133:0]out_data_cache_data_2;
wire out_data_cache_valid_wr_2;
wire out_data_cache_valid_2;
wire [7:0]in_data_cache_ID_2;
wire in_data_cache_ID_wr_2;

wire out_data_cache_data_wr_3;
wire [133:0]out_data_cache_data_3;
wire out_data_cache_valid_wr_3;
wire out_data_cache_valid_3;
wire [7:0]in_data_cache_ID_3;
wire in_data_cache_ID_wr_3;

//reg/////////////////
//lcm to esw//////////
wire direction;
wire reg_tap;
wire [47:0]local_mac_addr;
wire [47:0]direct_mac_addr;
wire [63:0]esw_pktin_cnt;
wire [63:0]esw_pktout_cnt;
wire [7:0]bufm_ID_cnt;

//lcm to eos//////////
wire time_slot_flag;
wire [15:0]token_bucket_para;
wire [15:0]token_bucket_depth;

// ebm to goe ///////////
wire in_goe_data_wr_0;
wire [133:0] in_goe_data_0;
wire in_goe_valid_wr_0;
wire in_goe_valid_0;

wire in_goe_data_wr_1;
wire [133:0] in_goe_data_1;
wire in_goe_valid_wr_1;
wire in_goe_valid_1;

wire in_goe_data_wr_2;
wire [133:0] in_goe_data_2;
wire in_goe_valid_wr_2;
wire in_goe_valid_2;

wire in_goe_data_wr_3;
wire [133:0] in_goe_data_3;
wire in_goe_valid_wr_3;
wire in_goe_valid_3;

PTP PTP(
.clk(clk),
.rst_n(rst_n),   

.pktin_data_wr(pktin_data_wr),
.pktin_data(pktin_data),
.pktin_data_valid(pktin_valid),
.pktin_data_valid_wr(pktin_valid_wr),
.pktin_ready(pktin_ready),

.ptp2lcm_data_wr(ptp2lcm_data_wr),
.ptp2lcm_data(ptp2lcm_data),
.ptp2lcm_data_valid(ptp2lcm_data_valid),
.ptp2lcm_data_valid_wr(ptp2lcm_data_valid_wr),
.ptp2lcm_ready(ptp2lcm_ready),  
    
.cfg_cs_n(cfg_cs_n), 
.cfg_ack_n(cfg_ack_n), 
.cfg_rw(cfg_rw), 
.cfg_addr(cfg_addr),
.cfg_wdata(cfg_wdata),
.cfg_rdata(cfg_rdata), 
.cyc_init(cyc_init),
.temp_cnt(temp_cnt),

.precision_time(precision_time),
.device_id(device_id)
);

lcm lcm(
.clk(clk),
.rst_n(rst_n),
//um signals
.in_lcm_data(ptp2lcm_data),
.in_lcm_data_wr(ptp2lcm_data_wr),
.in_lcm_data_valid(ptp2lcm_data_valid),
.in_lcm_data_valid_wr(ptp2lcm_data_valid_wr),
.pktin_ready(ptp2lcm_ready),
.precision_time(precision_time),
.in_local_mac_id(device_id),

//esw signals 
.out_lcm_data_wr(lcm2esw_data_wr),
.out_lcm_data(lcm2esw_data),
.out_lcm_data_valid(lcm2esw_valid),
.out_lcm_data_valid_wr(lcm2esw_valid_wr),

//readable & changeable registers and counters
.out_direction(direction),
.out_token_bucket_para(token_bucket_para),
.out_token_bucket_depth(token_bucket_depth),
.out_direct_mac_addr(direct_mac_addr),
.out_reg_tap(reg_tap),
//output to esw 
.out_MAC_0(MAC_0),
.out_MAC_1(MAC_1),
.out_MAC_2(MAC_2),
.out_MAC_3(MAC_3),
.out_MAC_4(MAC_4),
.out_MAC_5(MAC_5),
.out_MAC_6(MAC_6),
.out_MAC_7(MAC_7),
.out_MAC_8(MAC_8),
.out_MAC_9(MAC_9),
.out_MAC_10(MAC_10),
.out_MAC_11(MAC_11),

.out_port_0(port_0),
.out_port_1(port_1),
.out_port_2(port_2),
.out_port_3(port_3),
.out_port_4(port_4),
.out_port_5(port_5),
.out_port_6(port_6),
.out_port_7(port_7),
.out_port_8(port_8),
.out_port_9(port_9),
.out_port_10(port_10),
.out_port_11(port_11),

//input from esw
.esw_pktin_cnt(esw_pktin_cnt),
.esw_pktout_cnt_0(esw_pktout_cnt_0),
.esw_pktout_cnt_1(esw_pktout_cnt_1),
.esw_pktout_cnt_2(esw_pktout_cnt_2),
.esw_pktout_cnt_3(esw_pktout_cnt_3),
.bufm_id_cnt_0(bufm_ID_cnt_0),
.bufm_id_cnt_1(bufm_ID_cnt_1),
.bufm_id_cnt_2(bufm_ID_cnt_2),
.bufm_id_cnt_3(bufm_ID_cnt_3),

//input from eos
.eos_q0_used_cnt_0(eos_q0_used_cnt_0),
.eos_q1_used_cnt_0(eos_q1_used_cnt_0),
.eos_q2_used_cnt_0(eos_q2_used_cnt_0),
.eos_q3_used_cnt_0(eos_q3_used_cnt_0),

.eos_q0_used_cnt_1(eos_q0_used_cnt_1),
.eos_q1_used_cnt_1(eos_q1_used_cnt_1),
.eos_q2_used_cnt_1(eos_q2_used_cnt_1),
.eos_q3_used_cnt_1(eos_q3_used_cnt_1),

.eos_q0_used_cnt_2(eos_q0_used_cnt_2),
.eos_q1_used_cnt_2(eos_q1_used_cnt_2),
.eos_q2_used_cnt_2(eos_q2_used_cnt_2),
.eos_q3_used_cnt_2(eos_q3_used_cnt_2),

.eos_q0_used_cnt_3(eos_q0_used_cnt_3),
.eos_q1_used_cnt_3(eos_q1_used_cnt_3),
.eos_q2_used_cnt_3(eos_q2_used_cnt_3),
.eos_q3_used_cnt_3(eos_q3_used_cnt_3),

.time_slot_flag(time_slot_flag),

.eos_mdin_cnt_0(eos_mdin_cnt_0),
.eos_mdin_cnt_1(eos_mdin_cnt_1),
.eos_mdin_cnt_2(eos_mdin_cnt_2),
.eos_mdin_cnt_3(eos_mdin_cnt_3),
.eos_mdout_cnt_0(eos_mdout_cnt_0),
.eos_mdout_cnt_1(eos_mdout_cnt_1),
.eos_mdout_cnt_2(eos_mdout_cnt_2),
.eos_mdout_cnt_3(eos_mdout_cnt_3)
);

esw  esw_inst(
.clk(clk),
.rst_n(rst_n),

.in_esw_data(lcm2esw_data),
.in_esw_data_wr(lcm2esw_data_wr),
.in_esw_valid(lcm2esw_valid),
.in_esw_valid_wr(lcm2esw_valid_wr),

.out_esw_data_0(esw2ibm_data_0),
.out_esw_data_wr_0(esw2ibm_data_wr_0),
.out_esw_valid_0(esw2ibm_valid_0),
.out_esw_valid_wr_0(esw2ibm_valid_wr_0),
.out_esw_tsn_md_0(tsn_md_0),
.out_esw_tsn_md_wr_0(tsn_md_wr_0),
.bufm_ID_count_0(ibm2esw_bufm_ID_0),

.out_esw_data_1(esw2ibm_data_1),
.out_esw_data_wr_1(esw2ibm_data_wr_1),
.out_esw_valid_1(esw2ibm_valid_1),
.out_esw_valid_wr_1(esw2ibm_valid_wr_1),
.out_esw_tsn_md_1(tsn_md_1),
.out_esw_tsn_md_wr_1(tsn_md_wr_1),
.bufm_ID_count_1(ibm2esw_bufm_ID_1),

.out_esw_data_2(esw2ibm_data_2),
.out_esw_data_wr_2(esw2ibm_data_wr_2),
.out_esw_valid_2(esw2ibm_valid_2),
.out_esw_valid_wr_2(esw2ibm_valid_wr_2),
.out_esw_tsn_md_2(tsn_md_2),
.out_esw_tsn_md_wr_2(tsn_md_wr_2),
.bufm_ID_count_2(ibm2esw_bufm_ID_2),

.out_esw_data_3(esw2ibm_data_3),
.out_esw_data_wr_3(esw2ibm_data_wr_3),
.out_esw_valid_3(esw2ibm_valid_3),
.out_esw_valid_wr_3(esw2ibm_valid_wr_3),
.out_esw_tsn_md_3(tsn_md_3),
.out_esw_tsn_md_wr_3(tsn_md_wr_3),
.bufm_ID_count_3(ibm2esw_bufm_ID_3),

.in_reg_tap(reg_tap),
.in_MAC_0(MAC_0),
.in_MAC_1(MAC_1),
.in_MAC_2(MAC_2),
.in_MAC_3(MAC_3),
.in_MAC_4(MAC_4),
.in_MAC_5(MAC_5),
.in_MAC_6(MAC_6),
.in_MAC_7(MAC_7),
.in_MAC_8(MAC_8),
.in_MAC_9(MAC_9),
.in_MAC_10(MAC_10),
.in_MAC_11(MAC_11),

.in_port_0(port_0),
.in_port_1(port_1),
.in_port_2(port_2),
.in_port_3(port_3),
.in_port_4(port_4),
.in_port_5(port_5),
.in_port_6(port_6),
.in_port_7(port_7),
.in_port_8(port_8),
.in_port_9(port_9),
.in_port_10(port_10),
.in_port_11(port_11),

.esw_pktin_cnt(esw_pktin_cnt),
.esw_pktout_cnt_0(esw_pktout_cnt_0),
.esw_pktout_cnt_1(esw_pktout_cnt_1),
.esw_pktout_cnt_2(esw_pktout_cnt_2),
.esw_pktout_cnt_3(esw_pktout_cnt_3),

.bufm_ID_cnt_0(bufm_ID_cnt_0),
.bufm_ID_cnt_1(bufm_ID_cnt_1),
.bufm_ID_cnt_2(bufm_ID_cnt_2),
.bufm_ID_cnt_3(bufm_ID_cnt_3)
);

ibm ibm0_inst(
.clk(clk),
.rst_n(rst_n),

.in_ibm_data(esw2ibm_data_0),
.in_ibm_data_wr(esw2ibm_data_wr_0),
.in_ibm_valid(esw2ibm_valid_0),
.in_ibm_valid_wr(esw2ibm_valid_wr_0),
.out_ibm_bufm_ID(ibm2esw_bufm_ID_0),

.in_ibm_tsn_md(tsn_md_0),
.in_ibm_tsn_md_wr(tsn_md_wr_0),

.out_ibm_data(out_ibm_data_0),
.out_ibm_data_wr(out_ibm_data_wr_0),
.out_ibm_valid(out_ibm_valid_0),
.out_ibm_valid_wr(out_ibm_valid_wr_0),

.in_ibm_ID(in_ibm_ID_0),
.in_ibm_ID_count(in_ibm_ID_count_0),

.out_ibm_md(out_ibm_md_0),
.out_ibm_md_wr(out_ibm_md_wr_0)
);
ibm ibm1_inst(
.clk(clk),
.rst_n(rst_n),

.in_ibm_data(esw2ibm_data_1),
.in_ibm_data_wr(esw2ibm_data_wr_1),
.in_ibm_valid(esw2ibm_valid_1),
.in_ibm_valid_wr(esw2ibm_valid_wr_1),
.out_ibm_bufm_ID(ibm2esw_bufm_ID_1),

.in_ibm_tsn_md(tsn_md_1),
.in_ibm_tsn_md_wr(tsn_md_wr_1),

.out_ibm_data(out_ibm_data_1),
.out_ibm_data_wr(out_ibm_data_wr_1),
.out_ibm_valid(out_ibm_valid_1),
.out_ibm_valid_wr(out_ibm_valid_wr_1),

.in_ibm_ID(in_ibm_ID_1),
.in_ibm_ID_count(in_ibm_ID_count_1),

.out_ibm_md(out_ibm_md_1),
.out_ibm_md_wr(out_ibm_md_wr_1)
);
ibm ibm2_inst(
.clk(clk),
.rst_n(rst_n),

.in_ibm_data(esw2ibm_data_2),
.in_ibm_data_wr(esw2ibm_data_wr_2),
.in_ibm_valid(esw2ibm_valid_2),
.in_ibm_valid_wr(esw2ibm_valid_wr_2),
.out_ibm_bufm_ID(ibm2esw_bufm_ID_2),

.in_ibm_tsn_md(tsn_md_2),
.in_ibm_tsn_md_wr(tsn_md_wr_2),

.out_ibm_data(out_ibm_data_2),
.out_ibm_data_wr(out_ibm_data_wr_2),
.out_ibm_valid(out_ibm_valid_2),
.out_ibm_valid_wr(out_ibm_valid_wr_2),

.in_ibm_ID(in_ibm_ID_2),
.in_ibm_ID_count(in_ibm_ID_count_2),

.out_ibm_md(out_ibm_md_2),
.out_ibm_md_wr(out_ibm_md_wr_2)
);
ibm ibm3_inst(
.clk(clk),
.rst_n(rst_n),

.in_ibm_data(esw2ibm_data_3),
.in_ibm_data_wr(esw2ibm_data_wr_3),
.in_ibm_valid(esw2ibm_valid_3),
.in_ibm_valid_wr(esw2ibm_valid_wr_3),
.out_ibm_bufm_ID(ibm2esw_bufm_ID_3),

.in_ibm_tsn_md(tsn_md_3),
.in_ibm_tsn_md_wr(tsn_md_wr_3),

.out_ibm_data(out_ibm_data_3),
.out_ibm_data_wr(out_ibm_data_wr_3),
.out_ibm_valid(out_ibm_valid_3),
.out_ibm_valid_wr(out_ibm_valid_wr_3),

.in_ibm_ID(in_ibm_ID_3),
.in_ibm_ID_count(in_ibm_ID_count_3),

.out_ibm_md(out_ibm_md_3),
.out_ibm_md_wr(out_ibm_md_wr_3)
);

data_cache data_cache0_inst(
.clk(clk),
.rst_n(rst_n),

.in_data_cache_data_wr(out_ibm_data_wr_0),
.in_data_cache_data(out_ibm_data_0),
.in_data_cache_valid_wr(out_ibm_valid_wr_0),
.in_data_cache_valid(out_ibm_valid_0),
.out_data_cache_ID(in_ibm_ID_0),
.out_data_cache_ID_count(in_ibm_ID_count_0),
   
.out_data_cache_data_wr(out_data_cache_data_wr_0),
.out_data_cache_data(out_data_cache_data_0),
.out_data_cache_valid_wr(out_data_cache_valid_wr_0),
.out_data_cache_valid(out_data_cache_valid_0),
.in_data_cache_ID(in_data_cache_ID_0),
.in_data_cache_ID_wr(in_data_cache_ID_wr_0)
);
data_cache data_cache1_inst(
.clk(clk),
.rst_n(rst_n),

.in_data_cache_data_wr(out_ibm_data_wr_1),
.in_data_cache_data(out_ibm_data_1),
.in_data_cache_valid_wr(out_ibm_valid_wr_1),
.in_data_cache_valid(out_ibm_valid_1),
.out_data_cache_ID(in_ibm_ID_1),
.out_data_cache_ID_count(in_ibm_ID_count_1),
   
.out_data_cache_data_wr(out_data_cache_data_wr_1),
.out_data_cache_data(out_data_cache_data_1),
.out_data_cache_valid_wr(out_data_cache_valid_wr_1),
.out_data_cache_valid(out_data_cache_valid_1),
.in_data_cache_ID(in_data_cache_ID_1),
.in_data_cache_ID_wr(in_data_cache_ID_wr_1)
);
data_cache data_cache2_inst(
.clk(clk),
.rst_n(rst_n),

.in_data_cache_data_wr(out_ibm_data_wr_2),
.in_data_cache_data(out_ibm_data_2),
.in_data_cache_valid_wr(out_ibm_valid_wr_2),
.in_data_cache_valid(out_ibm_valid_2),
.out_data_cache_ID(in_ibm_ID_2),
.out_data_cache_ID_count(in_ibm_ID_count_2),
   
.out_data_cache_data_wr(out_data_cache_data_wr_2),
.out_data_cache_data(out_data_cache_data_2),
.out_data_cache_valid_wr(out_data_cache_valid_wr_2),
.out_data_cache_valid(out_data_cache_valid_2),
.in_data_cache_ID(in_data_cache_ID_2),
.in_data_cache_ID_wr(in_data_cache_ID_wr_2)
);
data_cache data_cache3_inst(
.clk(clk),
.rst_n(rst_n),

.in_data_cache_data_wr(out_ibm_data_wr_3),
.in_data_cache_data(out_ibm_data_3),
.in_data_cache_valid_wr(out_ibm_valid_wr_3),
.in_data_cache_valid(out_ibm_valid_3),
.out_data_cache_ID(in_ibm_ID_3),
.out_data_cache_ID_count(in_ibm_ID_count_3),
   
.out_data_cache_data_wr(out_data_cache_data_wr_3),
.out_data_cache_data(out_data_cache_data_3),
.out_data_cache_valid_wr(out_data_cache_valid_wr_3),
.out_data_cache_valid(out_data_cache_valid_3),
.in_data_cache_ID(in_data_cache_ID_3),
.in_data_cache_ID_wr(in_data_cache_ID_wr_3)
);

eos eos0_inst(
.clk(clk),
.rst_n(rst_n),

.in_eos_time_slot_flag(time_slot_flag),
.in_eos_rate_limit(token_bucket_para),
.in_eos_depth_limit(token_bucket_depth),

.out_eos_q0_used_cnt(eos_q0_used_cnt_0),
.out_eos_q1_used_cnt(eos_q1_used_cnt_0),
.out_eos_q2_used_cnt(eos_q2_used_cnt_0),
.out_eos_q3_used_cnt(eos_q3_used_cnt_0),
.out_eos_mdin_cnt(eos_mdin_cnt_0), 
.out_eos_mdout_cnt(eos_mdout_cnt_0),

.in_eos_md(out_ibm_md_0),
.in_eos_md_wr(out_ibm_md_wr_0),

.pktout_usedw(pktout_usedw_0),

.out_eos_bandwidth_discard(in_ebm_bandwidth_discard_0),
.out_eos_md(out_eos_tsn_md_0),
.out_eos_md_wr(out_eos_tsn_md_wr_0),
.in_eos_pkt_valid(in_goe_valid_0)
);
eos eos1_inst(
.clk(clk),
.rst_n(rst_n),

.in_eos_time_slot_flag(time_slot_flag),
.in_eos_rate_limit(token_bucket_para),
.in_eos_depth_limit(token_bucket_depth),

.out_eos_q0_used_cnt(eos_q0_used_cnt_1),
.out_eos_q1_used_cnt(eos_q1_used_cnt_1),
.out_eos_q2_used_cnt(eos_q2_used_cnt_1),
.out_eos_q3_used_cnt(eos_q3_used_cnt_1),
.out_eos_mdin_cnt(eos_mdin_cnt_1), 
.out_eos_mdout_cnt(eos_mdout_cnt_1),

.in_eos_md(out_ibm_md_1),
.in_eos_md_wr(out_ibm_md_wr_1),

.pktout_usedw(pktout_usedw_1), 

.out_eos_bandwidth_discard(in_ebm_bandwidth_discard_1),
.out_eos_md(out_eos_tsn_md_1),
.out_eos_md_wr(out_eos_tsn_md_wr_1),
.in_eos_pkt_valid(in_goe_valid_1)
);
eos eos2_inst(
.clk(clk),
.rst_n(rst_n),

.in_eos_time_slot_flag(time_slot_flag),
.in_eos_rate_limit(token_bucket_para),
.in_eos_depth_limit(token_bucket_depth),

.out_eos_q0_used_cnt(eos_q0_used_cnt_2),
.out_eos_q1_used_cnt(eos_q1_used_cnt_2),
.out_eos_q2_used_cnt(eos_q2_used_cnt_2),
.out_eos_q3_used_cnt(eos_q3_used_cnt_2),
.out_eos_mdin_cnt(eos_mdin_cnt_2), 
.out_eos_mdout_cnt(eos_mdout_cnt_2),

.in_eos_md(out_ibm_md_2),
.in_eos_md_wr(out_ibm_md_wr_2),

.pktout_usedw(pktout_usedw_2),

.out_eos_bandwidth_discard(in_ebm_bandwidth_discard_2),
.out_eos_md(out_eos_tsn_md_2),
.out_eos_md_wr(out_eos_tsn_md_wr_2),
.in_eos_pkt_valid(in_goe_valid_2)
);
eos eos3_inst(
.clk(clk),
.rst_n(rst_n),

.in_eos_time_slot_flag(time_slot_flag),
.in_eos_rate_limit(token_bucket_para),
.in_eos_depth_limit(token_bucket_depth),

.out_eos_q0_used_cnt(eos_q0_used_cnt_3),
.out_eos_q1_used_cnt(eos_q1_used_cnt_3),
.out_eos_q2_used_cnt(eos_q2_used_cnt_3),
.out_eos_q3_used_cnt(eos_q3_used_cnt_3),
.out_eos_mdin_cnt(eos_mdin_cnt_3), 
.out_eos_mdout_cnt(eos_mdout_cnt_3),

.in_eos_md(out_ibm_md_3),
.in_eos_md_wr(out_ibm_md_wr_3),

.pktout_usedw(pktout_usedw_3),

.out_eos_bandwidth_discard(in_ebm_bandwidth_discard_3),
.out_eos_md(out_eos_tsn_md_3),
.out_eos_md_wr(out_eos_tsn_md_wr_3),
.in_eos_pkt_valid(in_goe_valid_3)
);

ebm ebm0_inst(
.clk(clk),
.rst_n(rst_n),

.in_ebm_data(out_data_cache_data_0),
.in_ebm_data_wr(out_data_cache_data_wr_0),
.in_ebm_valid(out_data_cache_valid_0),
.in_ebm_valid_wr(out_data_cache_valid_wr_0),
.out_ebm_ID(in_data_cache_ID_0),
.out_ebm_ID_wr(in_data_cache_ID_wr_0),

.out_ebm_data(in_goe_data_0),
.out_ebm_data_wr(in_goe_data_wr_0),
.out_ebm_valid(in_goe_valid_0),
.out_ebm_valid_wr(in_goe_valid_wr_0),

.in_ebm_bandwidth_discard(in_ebm_bandwidth_discard_0),
.in_ebm_md(out_eos_tsn_md_0),
.in_ebm_md_wr(out_eos_tsn_md_wr_0)  
);
ebm ebm1_inst(
.clk(clk),
.rst_n(rst_n),

.in_ebm_data(out_data_cache_data_1),
.in_ebm_data_wr(out_data_cache_data_wr_1),
.in_ebm_valid(out_data_cache_valid_1),
.in_ebm_valid_wr(out_data_cache_valid_wr_1),
.out_ebm_ID(in_data_cache_ID_1),
.out_ebm_ID_wr(in_data_cache_ID_wr_1),

.out_ebm_data(in_goe_data_1),
.out_ebm_data_wr(in_goe_data_wr_1),
.out_ebm_valid(in_goe_valid_1),
.out_ebm_valid_wr(in_goe_valid_wr_1),

.in_ebm_bandwidth_discard(in_ebm_bandwidth_discard_1),
.in_ebm_md(out_eos_tsn_md_1),
.in_ebm_md_wr(out_eos_tsn_md_wr_1)  
);

ebm ebm2_inst(
.clk(clk),
.rst_n(rst_n),

.in_ebm_data(out_data_cache_data_2),
.in_ebm_data_wr(out_data_cache_data_wr_2),
.in_ebm_valid(out_data_cache_valid_2),
.in_ebm_valid_wr(out_data_cache_valid_wr_2),
.out_ebm_ID(in_data_cache_ID_2),
.out_ebm_ID_wr(in_data_cache_ID_wr_2),

.out_ebm_data(in_goe_data_2),
.out_ebm_data_wr(in_goe_data_wr_2),
.out_ebm_valid(in_goe_valid_2),
.out_ebm_valid_wr(in_goe_valid_wr_2),

.in_ebm_bandwidth_discard(in_ebm_bandwidth_discard_2),
.in_ebm_md(out_eos_tsn_md_2),
.in_ebm_md_wr(out_eos_tsn_md_wr_2)  
);

ebm ebm3_inst(
.clk(clk),
.rst_n(rst_n),

.in_ebm_data(out_data_cache_data_3),
.in_ebm_data_wr(out_data_cache_data_wr_3),
.in_ebm_valid(out_data_cache_valid_3),
.in_ebm_valid_wr(out_data_cache_valid_wr_3),
.out_ebm_ID(in_data_cache_ID_3),
.out_ebm_ID_wr(in_data_cache_ID_wr_3),

.out_ebm_data(in_goe_data_3),
.out_ebm_data_wr(in_goe_data_wr_3),
.out_ebm_valid(in_goe_valid_3),
.out_ebm_valid_wr(in_goe_valid_wr_3),

.in_ebm_bandwidth_discard(in_ebm_bandwidth_discard_3),
.in_ebm_md(out_eos_tsn_md_3),
.in_ebm_md_wr(out_eos_tsn_md_wr_3)  
);


 goe goe_inst(

.clk(clk),
.rst_n(rst_n),
	
//uda pkt waiting for transmit
.in_goe_data_wr_0(in_goe_data_wr_0),
.in_goe_data_0(in_goe_data_0),
.in_goe_valid_wr_0(in_goe_valid_wr_0),
.in_goe_valid_0(in_goe_valid_0),

.in_goe_data_wr_1(in_goe_data_wr_1),
.in_goe_data_1(in_goe_data_1),
.in_goe_valid_wr_1(in_goe_valid_wr_1),
.in_goe_valid_1(in_goe_valid_1),

.in_goe_data_wr_2(in_goe_data_wr_2),
.in_goe_data_2(in_goe_data_2),
.in_goe_valid_wr_2(in_goe_valid_wr_2),
.in_goe_valid_2(in_goe_valid_2),

.in_goe_data_wr_3(in_goe_data_wr_3),
.in_goe_data_3(in_goe_data_3),
.in_goe_valid_wr_3(in_goe_valid_wr_3),
.in_goe_valid_3(in_goe_valid_3),
	
//pkt waiting for transmit
.pktout_data_wr_0(pktout_data_wr_0),
.pktout_data_0(pktout_data_0),
.pktout_data_valid_wr_0(pktout_valid_wr_0),
.pktout_data_valid_0(pktout_valid_0),

//pkt waiting for transmit
.pktout_data_wr_1(pktout_data_wr_1),
.pktout_data_1(pktout_data_1),
.pktout_data_valid_wr_1(pktout_valid_wr_1),
.pktout_data_valid_1(pktout_valid_1),

.pktout_data_wr_2(pktout_data_wr_2),
.pktout_data_2(pktout_data_2),
.pktout_data_valid_wr_2(pktout_valid_wr_2),
.pktout_data_valid_2(pktout_valid_2),

.pktout_data_wr_3(pktout_data_wr_3),
.pktout_data_3(pktout_data_3),
.pktout_data_valid_wr_3(pktout_valid_wr_3),
.pktout_data_valid_3(pktout_valid_3)
);

endmodule


