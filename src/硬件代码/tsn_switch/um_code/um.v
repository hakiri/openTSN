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

output wire  [133:0]pktout_data_0,
output wire  pktout_data_wr_0,
output wire  pktout_valid_0,
output wire  pktout_valid_wr_0,

output wire  [133:0]pktout_data_1,
output wire  pktout_data_wr_1,
output wire  pktout_valid_1,
output wire  pktout_valid_wr_1,

output wire [133:0]port2_pktout_data,
output wire port2_pktout_data_wr,
output wire port2_pktout_valid,
output wire port2_pktout_valid_wr,

output wire [133:0]port3_pktout_data,
output wire port3_pktout_data_wr,
output wire port3_pktout_valid,
output wire port3_pktout_valid_wr
);
//PTP to lcm//
wire   [47:0]precision_time;
wire   [47:0]device_id;

wire   ptp2lcm_data_wr;
wire   [133:0]ptp2lcm_data;
wire   ptp2lcm_data_valid;
wire   ptp2lcm_data_valid_wr;
wire   ptp2lcm_ready;

//lcm to ping//
wire  [133:0]lcm2ping_data;
wire  lcm2ping_data_wr;
wire  lcm2ping_valid;
wire  lcm2ping_valid_wr;

//ping to esw//
wire [133:0]ping2esw_data;
wire ping2esw_data_wr;
wire ping2esw_valid;
wire ping2esw_valid_wr;

//esw to ibm//
wire  [133:0]esw2ibm_data;
wire  esw2ibm_data_wr;
wire  esw2ibm_valid;
wire  esw2ibm_valid_wr;
wire  [23:0]tsn_md;
wire  tsn_md_wr;
wire [4:0]ibm2esw_bufm_ID;

//ibm to data_cache//
wire [133:0]out_ibm_data;
wire out_ibm_data_wr;
wire out_ibm_valid;
wire out_ibm_valid_wr;
wire [7:0]in_ibm_ID;
wire [4:0]in_ibm_ID_count;

//ibm to eos//
wire [23:0]out_ibm_md;
wire out_ibm_md_wr;
 
//eos to ebm//
wire [7:0]out_eos_tsn_md;
wire out_eos_tsn_md_wr;
wire in_ebm_bandwidth_discard;

//data_cache to ebm// 
wire out_data_cache_data_wr;
wire [133:0]out_data_cache_data;
wire out_data_cache_valid_wr;
wire out_data_cache_valid;
wire [7:0]in_data_cache_ID;
wire in_data_cache_ID_wr;

//reg/////////////////
//lcm to esw//////////
wire direction;
wire [47:0]local_mac_addr;
wire [47:0]direct_mac_addr;
wire [63:0]esw_pktin_cnt;
wire [63:0]esw_pktout_cnt;
wire [7:0]bufm_ID_cnt;

//lcm to eos//////////
wire [7:0]eos_q0_used_cnt;
wire [7:0]eos_q1_used_cnt;
wire [7:0]eos_q2_used_cnt;
wire [7:0]eos_q3_used_cnt;
wire time_slot_flag;
wire [63:0]eos_mdin_cnt;
wire [63:0]eos_mdout_cnt;
wire [31:0]token_bucket_para;

// ebm to goe ///////////
wire in_goe_data_wr;
wire [133:0] in_goe_data;
wire in_goe_valid_wr;
wire in_goe_valid;

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
.out_lcm_data_wr(lcm2ping_data_wr),
.out_lcm_data(lcm2ping_data),
.out_lcm_data_valid(lcm2ping_valid),
.out_lcm_data_valid_wr(lcm2ping_valid_wr),

//readable & changeable registers and counters
.out_direction(direction),
.out_token_bucket_para(token_bucket_para),
.out_direct_mac_addr(direct_mac_addr),

	//input from esw
.esw_pktin_cnt(esw_pktin_cnt),
.esw_pktout_cnt(esw_pktout_cnt),
.bufm_id_cnt(bufm_ID_cnt),

//input from eos
.eos_q0_used_cnt(eos_q0_used_cnt),
.eos_q1_used_cnt(eos_q1_used_cnt),
.eos_q2_used_cnt(eos_q2_used_cnt),
.eos_q3_used_cnt(eos_q3_used_cnt),

.time_slot_flag(time_slot_flag),
.eos_mdin_cnt(eos_mdin_cnt),
.eos_mdout_cnt(eos_mdout_cnt),

//input from goe	
.goe_pktin_cnt(),
.goe_port0out_cnt(),
.goe_port1out_cnt(),
.goe_discard_cnt()
);

ping_cnt ping_cnt(
	.clk(clk),
	.rst_n(rst_n),
	.in_ping_data(lcm2ping_data),
	.in_ping_data_wr(lcm2ping_data_wr),
	.in_ping_data_valid(lcm2ping_valid),
	.in_ping_data_valid_wr(lcm2ping_valid_wr),

	.out_ping_data(ping2esw_data),
	.out_ping_data_wr(ping2esw_data_wr),
	.out_ping_data_valid(ping2esw_valid),
	.out_ping_data_valid_wr(ping2esw_valid_wr)
);

esw esw(
.clk(clk),
.rst_n(rst_n),

.in_esw_data(ping2esw_data),
.in_esw_data_wr(ping2esw_data_wr),
.in_esw_valid(ping2esw_valid),
.in_esw_valid_wr(ping2esw_valid_wr),

.out_esw_data(esw2ibm_data),
.out_esw_data_wr(esw2ibm_data_wr),
.out_esw_valid(esw2ibm_valid),
.out_esw_valid_wr(esw2ibm_valid_wr),
.out_esw_tsn_md(tsn_md),
.out_esw_tsn_md_wr(tsn_md_wr),
.bufm_ID_count(ibm2esw_bufm_ID),

.out_esw2port_data2(port2_pktout_data),
.out_esw2port_data_wr2(port2_pktout_data_wr),
.out_esw2port_valid2(port2_pktout_valid),
.out_esw2port_valid_wr2(port2_pktout_valid_wr),

.out_esw2port_data3(port3_pktout_data),
.out_esw2port_data_wr3(port3_pktout_data_wr),
.out_esw2port_valid3(port3_pktout_valid),
.out_esw2port_valid_wr3(port3_pktout_valid_wr),

.direction(direction),
.local_mac_addr(local_mac_addr),
.direct_mac_addr(direct_mac_addr),
.esw_pktin_cnt(esw_pktin_cnt),
.esw_pktout_cnt(esw_pktout_cnt),
.bufm_ID_cnt(bufm_ID_cnt)
);

ibm ibm_inst(
.clk(clk),
.rst_n(rst_n),

.in_ibm_data(esw2ibm_data),
.in_ibm_data_wr(esw2ibm_data_wr),
.in_ibm_valid(esw2ibm_valid),
.in_ibm_valid_wr(esw2ibm_valid_wr),
.out_ibm_bufm_ID(ibm2esw_bufm_ID),

.in_ibm_tsn_md(tsn_md),
.in_ibm_tsn_md_wr(tsn_md_wr),

.out_ibm_data(out_ibm_data),
.out_ibm_data_wr(out_ibm_data_wr),
.out_ibm_valid(out_ibm_valid),
.out_ibm_valid_wr(out_ibm_valid_wr),

.in_ibm_ID(in_ibm_ID),
.in_ibm_ID_count(in_ibm_ID_count),

.out_ibm_md(out_ibm_md),
.out_ibm_md_wr(out_ibm_md_wr)
);

data_cache data_cache_inst(
.clk(clk),
.rst_n(rst_n),

.in_data_cache_data_wr(out_ibm_data_wr),
.in_data_cache_data(out_ibm_data),
.in_data_cache_valid_wr(out_ibm_valid_wr),
.in_data_cache_valid(out_ibm_valid),
.out_data_cache_ID(in_ibm_ID),
.out_data_cache_ID_count(in_ibm_ID_count),
   
.out_data_cache_data_wr(out_data_cache_data_wr),
.out_data_cache_data(out_data_cache_data),
.out_data_cache_valid_wr(out_data_cache_valid_wr),
.out_data_cache_valid(out_data_cache_valid),
.in_data_cache_ID(in_data_cache_ID),
.in_data_cache_ID_wr(in_data_cache_ID_wr)
);

eos eos(
.clk(clk),
.rst_n(rst_n),

.in_eos_time_slot_flag(time_slot_flag),
.in_eos_rate_limit(token_bucket_para),

.out_eos_q0_used_cnt(eos_q0_used_cnt),
.out_eos_q1_used_cnt(eos_q1_used_cnt),
.out_eos_q2_used_cnt(eos_q2_used_cnt),
.out_eos_q3_used_cnt(eos_q3_used_cnt),
.out_eos_mdin_cnt(eos_mdin_cnt), 
.out_eos_mdout_cnt(eos_mdout_cnt),

.in_eos_md(out_ibm_md),
.in_eos_md_wr(out_ibm_md_wr),

.pktout_usedw_0(pktout_usedw_0),
.pktout_usedw_1(pktout_usedw_1), 

.out_eos_bandwidth_discard(in_ebm_bandwidth_discard),
.out_eos_md(out_eos_tsn_md),
.out_eos_md_wr(out_eos_tsn_md_wr),
.in_eos_pkt_valid(in_goe_valid)
);

ebm ebm_inst(
.clk(clk),
.rst_n(rst_n),

.in_ebm_data(out_data_cache_data),
.in_ebm_data_wr(out_data_cache_data_wr),
.in_ebm_valid(out_data_cache_valid),
.in_ebm_valid_wr(out_data_cache_valid_wr),
.out_ebm_ID(in_data_cache_ID),
.out_ebm_ID_wr(in_data_cache_ID_wr),

.out_ebm_data(in_goe_data),
.out_ebm_data_wr(in_goe_data_wr),
.out_ebm_valid(in_goe_valid),
.out_ebm_valid_wr(in_goe_valid_wr),

.in_ebm_bandwidth_discard(in_ebm_bandwidth_discard),
.in_ebm_md(out_eos_tsn_md),
.in_ebm_md_wr(out_eos_tsn_md_wr)  
);


 goe (

.clk(clk),
.rst_n(rst_n),
	
//uda pkt waiting for transmit
.in_goe_data_wr(in_goe_data_wr),
.in_goe_data(in_goe_data),
.in_goe_valid_wr(in_goe_valid_wr),
.in_goe_valid(in_goe_valid),
	
//pkt waiting for transmit
.pktout_data_wr_0(pktout_data_wr_0),
.pktout_data_0(pktout_data_0),
.pktout_data_valid_wr_0(pktout_valid_wr_0),
.pktout_data_valid_0(pktout_valid_0),

    //pkt waiting for transmit
.pktout_data_wr_1(pktout_data_wr_1),
.pktout_data_1(pktout_data_1),
.pktout_data_valid_wr_1(pktout_valid_wr_1),
.pktout_data_valid_1(pktout_valid_1)
);

endmodule


