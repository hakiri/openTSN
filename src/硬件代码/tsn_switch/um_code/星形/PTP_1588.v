module PTP_1588(
input wire clk,
input wire rst_n,

input cfg_cs_n, //low active
output wire cfg_ack_n, //low active
input cfg_rw, //0 :write, 1 :read
input [31:0] cfg_addr,
input [31:0] cfg_wdata,
output wire [31:0] cfg_rdata,

output wire	 cyc_init,
output wire	[47:0]	temp_cnt,
//input wire cyc_init_ack,
/////////////////input/////////////
input	inptp_data_wr,
input	[133:0]	inptp_data,
input	inptp_valid_wr,
input	inptp_valid,
output	inptp_ready,
///////////////output/////////////
output wire outtx_data_wr,
output wire [133:0]outtx_data,
output wire outtx_valid_wr,
output wire outtx_valid,
input wire outtx_ready,
output wire [47:0]device_mac,
output wire [1:0]device_role,
output wire [47:0]device_id,
output wire [47:0]precision_time
);
	
wire sync_start;
//wire [1:0]device_role;
wire error;
wire [47:0] mac;	
wire error1;
wire	ptp_recv_type_valid;
wire	[3:0] ptp_recv_type;
wire 	send_dreq_pkt;
wire 	send_dresq_pkt;
wire	ptp_send_type_valid;
wire	[3:0] ptp_send_type;
wire 	m_or_s;
wire	status_ok;
wire	ts_3_valid;
wire	[47:0] ts_3;
wire	[47:0]	timer;
wire	send_sync_pkt;
wire	send_test_pkt;

wire	ts_2_record;
wire	ts_1_valid;
wire	[47:0]	ts_1;
wire	ts_4_valid;
wire	[47:0]	ts_4;
wire ts_4_time_wr;
wire [47:0]ts_4_time;
wire	 [53:0]	key;
wire	key_valid;
wire [47:0]offset;

wire [47:0]rtt_time;
wire [31:0]sync_cnt;
//wire   [30:0]test_time;
assign precision_time=timer;
Manage_CTRL Manage_CTRL_INST(
.clk(clk),
.rst_n(rst_n),
.cfg_cs_n(cfg_cs_n), //low active
.cfg_ack_n(cfg_ack_n), //low active
.cfg_rw(cfg_rw), //0 :write, 1 :read
.cfg_addr(cfg_addr),
.cfg_wdata(cfg_wdata),
.cfg_rdata(cfg_rdata),
.sync_start(sync_start),
.device_role(device_role),
.rtt_time(rtt_time),
.error(error),
.device_mac(device_mac),
//.test_time(test_time),
.offset(offset),
.temp_cnt(temp_cnt),
.error1(error1),
.sync_cnt(sync_cnt),
.device_id(device_id)
);


PTP_CTRL PTP_CTRL_INST(
.clk(clk),
.reset(rst_n),
//from rx_proc
.ptp_recv_type_valid(ptp_recv_type_valid),
.ptp_recv_type(ptp_recv_type),
	
// to tx_proc
.send_dreq_pkt(send_dreq_pkt),
.send_dresq_pkt(send_dresq_pkt),
//from tx_proc
.ptp_send_type(ptp_send_type),
.ptp_send_type_valid(ptp_send_type_valid),

	
//from ctrl
.sync_start(sync_start),
.device_role(device_role),
.error(error),
	
//to cyc_sync
.m_or_s(m_or_s),
.status_ok(status_ok)
);


CYC_SYNC CYC_SYNC_INST(
.clk(clk),
.reset(rst_n),
.m_or_s(m_or_s),
.status_ok(status_ok),
.ts_3_valid(ts_3_valid),
.ts_3(ts_3),
.timer(timer),
.send_sync_pkt(send_sync_pkt),
.send_test_pkt(send_test_pkt),
.ts_2_record(ts_2_record),
.ts_1_valid(ts_1_valid),
.ts_1(ts_1),
.ts_4_valid(ts_4_valid),
.ts_4(ts_4),
.error1(error1),
.cyc_init(cyc_init),
//.cyc_init_ack(cyc_init_ack),
.offset(offset),
//.test_time(test_time),
.temp_cnt(temp_cnt),
.sync_cnt(sync_cnt)
);







RX_PROC RX_PROC_INST(
.clk(clk),
.reset(rst_n),
//from port_cmp
.inptp_data_wr(inptp_data_wr),
.inptp_data(inptp_data),
.inptp_valid_wr(inptp_valid_wr),
.inptp_valid(inptp_valid),
.inptp_ready(inptp_ready),

// to tx_proc
.ts_4_time_wr(ts_4_time_wr),
.ts_4_time(ts_4_time),
.key(key),
.key_valid(key_valid),
	
// to ptp_ctrl
.ptp_recv_type_valid(ptp_recv_type_valid),
.ptp_recv_type(ptp_recv_type),
	
// to cyc_sync
.ts_2_record(ts_2_record),
.ts_1_valid(ts_1_valid),
.ts_1(ts_1),
.ts_4_valid(ts_4_valid),
.ts_4(ts_4),
.timer(timer)	
);


TX_PROC TX_PROC_INST(
.clk(clk),
.rst_n(rst_n),
////////////////cyc_sync////////////////
.ts_3_valid(ts_3_valid),
.ts_3(ts_3),//{ms_cnt,cyc_cnt}
.cyc_timer(timer),
.send_sync_pkt(send_sync_pkt),
.send_test_pkt(send_test_pkt),
//////////////RX_porc///////////////////
.ts_4_valid(ts_4_time_wr),
.ts_4(ts_4_time),
.key(key),
.key_valid(key_valid),
///////////////PTP_CTRL/////////////////
.send_dreq_pkt(send_dreq_pkt),
.send_dresq_pkt(send_dresq_pkt),
.ptp_send_type(ptp_send_type),
.ptp_send_type_valid(ptp_send_type_valid),
/////////////////MAC////////////////////
.outtx_data_wr(outtx_data_wr),
.outtx_data(outtx_data),
.outtx_valid_wr(outtx_valid_wr),
.outtx_valid(outtx_valid),
.outtx_ready(outtx_ready),
///////////////CTRL///////////////////////
.rtt_time(rtt_time),
.MAC_ADDR(device_mac)
);
endmodule