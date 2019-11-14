module um_test#(
parameter    PLATFORM = "Xilinx",
                 LMID = 8'd1)
(
   input clk,
   input rst_n,   
//cpu2um
input  cpu2um_data_wr,
input  [133:0] cpu2um_data,
input  cpu2um_data_valid,
input  cpu2um_data_valid_wr,
output um2cpu_ready,//pktin_ready = um2port_alf

output um2cpu_data_wr,
output [133:0] um2cpu_data,
output um2cpu_data_valid,
output um2cpu_data_valid_wr,
input cpu2um_ready,//pktout_ready = port2um_alf    

// port02um
input   port02um_data_wr,
input  [133:0] port02um_data,
input  port02um_data_valid,
input  port02um_data_valid_wr,
output um2port0_ready,//pktin_ready = um2port_alf

output um2port0_data_wr,
output [133:0] um2port0_data,
output um2port0_data_valid,
output um2port0_data_valid_wr,
input [7:0]port02um_usedw,//pktout_ready = port2um_alf    

// port12um
input   port12um_data_wr,
input  [133:0] port12um_data,
input  port12um_data_valid,
input  port12um_data_valid_wr,
output um2port1_ready,//pktin_ready = um2port_alf

output um2port1_data_wr,
output [133:0] um2port1_data,
output um2port1_data_valid,
output um2port1_data_valid_wr,
input [7:0]port12um_usedw,//pktout_ready = port2um_alf    
   
// port22um
(*mark_debug="TRUE"*)input   port22um_data_wr,
(*mark_debug="TRUE"*)input  [133:0] port22um_data,
input  port22um_data_valid,
input  port22um_data_valid_wr,
(*mark_debug="TRUE"*)output um2port2_ready,//pktin_ready = um2port_alf

output um2port2_data_wr,
output [133:0] um2port2_data,
output um2port2_data_valid,
output um2port2_data_valid_wr,
input [7:0]port22um_usedw,//pktout_ready = port2um_alf  

// port32um
input   port32um_data_wr,
input  [133:0] port32um_data,
input  port32um_data_valid,
input  port32um_data_valid_wr,
output um2port3_ready,//pktin_ready = um2port_alf

output um2port3_data_wr,
output [133:0] um2port3_data,
output um2port3_data_valid,
output um2port3_data_valid_wr,
input [7:0]port32um_usedw//pktout_ready = port2um_alf  
);

assign um2port0_ready = 1'b1;
assign um2port1_ready = 1'b1;
assign um2port2_ready = 1'b1;
assign um2port3_ready = 1'b1;

assign um2cpu_data = cpu2um_data;
assign um2cpu_data_wr = cpu2um_data_wr;
assign um2cpu_data_valid = cpu2um_data_valid;
assign um2cpu_data_valid_wr = cpu2um_data_valid_wr;
assign um2cpu_ready = cpu2um_ready;

assign um2port2_data = 134'b0;
assign um2port2_data_wr = 1'b0;
assign um2port2_data_valid = 1'b0;
assign um2port2_data_valid_wr = 1'b0;


wire   [47:0] timestamp;
//lcm-pgm
wire          lcm2pgm_addr_shift;
wire          lcm2pgm_slot_shift; 
wire          test_start; 
wire          test_stop;
wire          lau_update_finish;
wire          cnt_rst;

wire          gcl_ram_rd;
wire   [3:0]  slot_shift_cnt;
wire   [8:0]  slot_ID;

wire   [11:0] lcm2pgm_pkt_1_len;
wire   [11:0] lcm2pgm_pkt_2_len;
wire   [11:0] lcm2pgm_pkt_3_len;
wire   [11:0] lcm2pgm_pkt_4_len;
wire   [11:0] lcm2pgm_pkt_5_len;
wire   [11:0] lcm2pgm_pkt_6_len;
wire   [11:0] lcm2pgm_pkt_7_len;
wire   [11:0] lcm2pgm_pkt_8_len;
 
wire   [15:0] lcm2pgm_tb_1_size; 
wire   [15:0] lcm2pgm_tb_1_rate;
wire   [15:0] lcm2pgm_tb_2_size; 
wire   [15:0] lcm2pgm_tb_2_rate;
wire   [15:0] lcm2pgm_tb_3_size; 
wire   [15:0] lcm2pgm_tb_3_rate;
wire   [15:0] lcm2pgm_tb_4_size; 
wire   [15:0] lcm2pgm_tb_4_rate;
wire   [15:0] lcm2pgm_tb_5_size; 
wire   [15:0] lcm2pgm_tb_5_rate;
wire   [15:0] lcm2pgm_tb_6_size; 
wire   [15:0] lcm2pgm_tb_6_rate;
wire   [15:0] lcm2pgm_tb_7_size; 
wire   [15:0] lcm2pgm_tb_7_rate;
wire   [15:0] lcm2pgm_tb_8_size; 
wire   [15:0] lcm2pgm_tb_8_rate;
 
wire   [31:0] pgm2lcm_pkt_1_cnt;
wire   [31:0] pgm2lcm_pkt_2_cnt;
wire   [31:0] pgm2lcm_pkt_3_cnt;
wire   [31:0] pgm2lcm_pkt_4_cnt;
wire   [31:0] pgm2lcm_pkt_5_cnt;
wire   [31:0] pgm2lcm_pkt_6_cnt;
wire   [31:0] pgm2lcm_pkt_7_cnt;
wire   [31:0] pgm2lcm_pkt_8_cnt;
//LCM-FSM  
wire   [31:0] fsm2lcm_pkt_1_cnt;
wire   [31:0] fsm2lcm_pkt_2_cnt;
wire   [31:0] fsm2lcm_pkt_3_cnt;
wire   [31:0] fsm2lcm_pkt_4_cnt;
wire   [31:0] fsm2lcm_pkt_5_cnt;
wire   [31:0] fsm2lcm_pkt_6_cnt;
wire   [31:0] fsm2lcm_pkt_7_cnt;
wire   [31:0] fsm2lcm_pkt_8_cnt;

wire   [103:0]lcm2fsm_rule_5tuple_1;
wire   [103:0]lcm2fsm_mask_1;
wire   [103:0]lcm2fsm_rule_5tuple_2;
wire   [103:0]lcm2fsm_mask_2;
wire   [103:0]lcm2fsm_rule_5tuple_3;
wire   [103:0]lcm2fsm_mask_3;
wire   [103:0]lcm2fsm_rule_5tuple_4;
wire   [103:0]lcm2fsm_mask_4;
wire   [103:0]lcm2fsm_rule_5tuple_5;
wire   [103:0]lcm2fsm_mask_5;
wire   [103:0]lcm2fsm_rule_5tuple_6;
wire   [103:0]lcm2fsm_mask_6;
wire   [103:0]lcm2fsm_rule_5tuple_7;
wire   [103:0]lcm2fsm_mask_7;
wire   [103:0]lcm2fsm_rule_5tuple_8;
wire   [103:0]lcm2fsm_mask_8;
//LCM-SSM 
wire   [15:0] lcm2ssm_samp_freq;
wire   [31:0] ssm2lcm_pkt_cnt;
//LCM to GCL_RAM
wire          lcm2ram_gc_wr;
wire   [4:0]  lcm2ram_gc_addr;
wire   [127:0]lcm2ram_gc;
//GCL_RAM to PGM
wire          ram2pgm_gc_rd;
wire   [4:0]  ram2pgm_gc_addr;
wire   [127:0]ram2pgm_gc;
//LCM to PKT_HDR_RAM
wire          lcm2ram_pkt_hdr_wr;
wire   [5:0]  lcm2ram_pkt_hdr_addr;
wire   [127:0]lcm2ram_pkt_hdr;
//PKT_HDR_RAM to PGM 
wire          ram2pgm_pkt_hdr_rd;
wire   [5:0]  ram2pgm_pkt_hdr_addr;
wire   [127:0]ram2pgm_pkt_hdr;
            
lcm  lcm_inst(
.clk(clk),
.rst_n(rst_n),

.in_lcm_data(port02um_data),
.in_lcm_data_wr(port02um_data_wr),

.slot_shift_cnt(slot_shift_cnt),  
.slot_ID(slot_ID),
.gcl_ram_rd(gcl_ram_rd),

.out_lcm_pkt_1_len(lcm2pgm_pkt_1_len),
.out_lcm_pkt_2_len(lcm2pgm_pkt_2_len),
.out_lcm_pkt_3_len(lcm2pgm_pkt_3_len),
.out_lcm_pkt_4_len(lcm2pgm_pkt_4_len),
.out_lcm_pkt_5_len(lcm2pgm_pkt_5_len),
.out_lcm_pkt_6_len(lcm2pgm_pkt_6_len),
.out_lcm_pkt_7_len(lcm2pgm_pkt_7_len),
.out_lcm_pkt_8_len(lcm2pgm_pkt_8_len),

.out_lcm_tb_1_size(lcm2pgm_tb_1_size), 
.out_lcm_tb_1_rate(lcm2pgm_tb_1_rate),
.out_lcm_tb_2_size(lcm2pgm_tb_2_size), 
.out_lcm_tb_2_rate(lcm2pgm_tb_2_rate),
.out_lcm_tb_3_size(lcm2pgm_tb_3_size), 
.out_lcm_tb_3_rate(lcm2pgm_tb_3_rate),
.out_lcm_tb_4_size(lcm2pgm_tb_4_size), 
.out_lcm_tb_4_rate(lcm2pgm_tb_4_rate),
.out_lcm_tb_5_size(lcm2pgm_tb_5_size), 
.out_lcm_tb_5_rate(lcm2pgm_tb_5_rate),
.out_lcm_tb_6_size(lcm2pgm_tb_6_size), 
.out_lcm_tb_6_rate(lcm2pgm_tb_6_rate),
.out_lcm_tb_7_size(lcm2pgm_tb_7_size), 
.out_lcm_tb_7_rate(lcm2pgm_tb_7_rate),
.out_lcm_tb_8_size(lcm2pgm_tb_8_size), 
.out_lcm_tb_8_rate(lcm2pgm_tb_8_rate),

.out_lcm_rule_5tuple_1(lcm2fsm_rule_5tuple_1),
.out_lcm_mask_1(lcm2fsm_mask_1),
.out_lcm_rule_5tuple_2(lcm2fsm_rule_5tuple_2),
.out_lcm_mask_2(lcm2fsm_mask_2),
.out_lcm_rule_5tuple_3(lcm2fsm_rule_5tuple_3),
.out_lcm_mask_3(lcm2fsm_mask_3),
.out_lcm_rule_5tuple_4(lcm2fsm_rule_5tuple_4),
.out_lcm_mask_4(lcm2fsm_mask_4),
.out_lcm_rule_5tuple_5(lcm2fsm_rule_5tuple_5),
.out_lcm_mask_5(lcm2fsm_mask_5),
.out_lcm_rule_5tuple_6(lcm2fsm_rule_5tuple_6),
.out_lcm_mask_6(lcm2fsm_mask_6),
.out_lcm_rule_5tuple_7(lcm2fsm_rule_5tuple_7),
.out_lcm_mask_7(lcm2fsm_mask_7),
.out_lcm_rule_5tuple_8(lcm2fsm_rule_5tuple_8),
.out_lcm_mask_8(lcm2fsm_mask_8),

.out_lcm_samp_freq(lcm2ssm_samp_freq),

.in_pgm2lcm_pkt_1_cnt(pgm2lcm_pkt_1_cnt),
.in_pgm2lcm_pkt_2_cnt(pgm2lcm_pkt_2_cnt),
.in_pgm2lcm_pkt_3_cnt(pgm2lcm_pkt_3_cnt),
.in_pgm2lcm_pkt_4_cnt(pgm2lcm_pkt_4_cnt),
.in_pgm2lcm_pkt_5_cnt(pgm2lcm_pkt_5_cnt),
.in_pgm2lcm_pkt_6_cnt(pgm2lcm_pkt_6_cnt),
.in_pgm2lcm_pkt_7_cnt(pgm2lcm_pkt_7_cnt),
.in_pgm2lcm_pkt_8_cnt(pgm2lcm_pkt_8_cnt),

.in_fsm2lcm_pkt_1_cnt(fsm2lcm_pkt_1_cnt),
.in_fsm2lcm_pkt_2_cnt(fsm2lcm_pkt_2_cnt),
.in_fsm2lcm_pkt_3_cnt(fsm2lcm_pkt_3_cnt),
.in_fsm2lcm_pkt_4_cnt(fsm2lcm_pkt_4_cnt),
.in_fsm2lcm_pkt_5_cnt(fsm2lcm_pkt_5_cnt),
.in_fsm2lcm_pkt_6_cnt(fsm2lcm_pkt_6_cnt),
.in_fsm2lcm_pkt_7_cnt(fsm2lcm_pkt_7_cnt),
.in_fsm2lcm_pkt_8_cnt(fsm2lcm_pkt_8_cnt),

.in_ssm2lcm_pkt_cnt(ssm2lcm_pkt_cnt),

.timestamp(timestamp),
.out_lcm_addr_shift(lcm2pgm_addr_shift),
.out_lcm_slot_shift(lcm2pgm_slot_shift),                       
.out_lcm_test_start(test_start),
.test_stop(test_stop),
.lau_update_finish(lau_update_finish),

.cnt_rst(cnt_rst),

.out_lcm_pkt_hdr_wr(lcm2ram_pkt_hdr_wr),
.out_lcm_pkt_hdr_addr(lcm2ram_pkt_hdr_addr),
.out_lcm_pkt_hdr(lcm2ram_pkt_hdr),

.out_lcm_gc_wr(lcm2ram_gc_wr),
.out_lcm_gc_addr(lcm2ram_gc_addr),
.out_lcm_gc(lcm2ram_gc),

.out_lcm_data(um2port0_data),
.out_lcm_data_wr(um2port0_data_wr),
.out_lcm_data_valid(um2port0_data_valid),
.out_lcm_data_valid_wr(um2port0_data_valid_wr)

);

pgm pgm_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),

.timestamp(timestamp),
.in_pgm_addr_shift(lcm2pgm_addr_shift),
.in_pgm_slot_shift(lcm2pgm_slot_shift), 
.in_pgm_test_start(test_start), 
.test_stop(test_stop), 
.lau_update_finish(lau_update_finish),
//.in_pgm_test_stop(lcm2pgm_test_stop), 
.gcl_ram_rd(gcl_ram_rd),
.slot_shift_cnt(slot_shift_cnt),
.slot_ID(slot_ID),
   
.in_pgm_pkt_1_len(lcm2pgm_pkt_1_len),
.in_pgm_pkt_2_len(lcm2pgm_pkt_2_len),
.in_pgm_pkt_3_len(lcm2pgm_pkt_3_len),
.in_pgm_pkt_4_len(lcm2pgm_pkt_4_len),
.in_pgm_pkt_5_len(lcm2pgm_pkt_5_len),
.in_pgm_pkt_6_len(lcm2pgm_pkt_6_len),
.in_pgm_pkt_7_len(lcm2pgm_pkt_7_len),
.in_pgm_pkt_8_len(lcm2pgm_pkt_8_len),
                  
.in_pgm_tb_1_size(lcm2pgm_tb_1_size), 
.in_pgm_tb_1_rate(lcm2pgm_tb_1_rate),
.in_pgm_tb_2_size(lcm2pgm_tb_2_size), 
.in_pgm_tb_2_rate(lcm2pgm_tb_2_rate),
.in_pgm_tb_3_size(lcm2pgm_tb_3_size), 
.in_pgm_tb_3_rate(lcm2pgm_tb_3_rate),
.in_pgm_tb_4_size(lcm2pgm_tb_4_size), 
.in_pgm_tb_4_rate(lcm2pgm_tb_4_rate),
.in_pgm_tb_5_size(lcm2pgm_tb_5_size), 
.in_pgm_tb_5_rate(lcm2pgm_tb_5_rate),
.in_pgm_tb_6_size(lcm2pgm_tb_6_size), 
.in_pgm_tb_6_rate(lcm2pgm_tb_6_rate),
.in_pgm_tb_7_size(lcm2pgm_tb_7_size), 
.in_pgm_tb_7_rate(lcm2pgm_tb_7_rate),
.in_pgm_tb_8_size(lcm2pgm_tb_8_size), 
.in_pgm_tb_8_rate(lcm2pgm_tb_8_rate),

.out_pgm_pkt_1_cnt(pgm2lcm_pkt_1_cnt),
.out_pgm_pkt_2_cnt(pgm2lcm_pkt_2_cnt),
.out_pgm_pkt_3_cnt(pgm2lcm_pkt_3_cnt),
.out_pgm_pkt_4_cnt(pgm2lcm_pkt_4_cnt),
.out_pgm_pkt_5_cnt(pgm2lcm_pkt_5_cnt),
.out_pgm_pkt_6_cnt(pgm2lcm_pkt_6_cnt),
.out_pgm_pkt_7_cnt(pgm2lcm_pkt_7_cnt),
.out_pgm_pkt_8_cnt(pgm2lcm_pkt_8_cnt),

.in_pgm_fifo_usedw(port12um_usedw), 

.out_pgm_gcl_rd(ram2pgm_gc_rd),   
.out_pgm_gcl_addr(ram2pgm_gc_addr),    
.in_pgm_gc(ram2pgm_gc),         

.out_pgm_pkt_hdr_rd(ram2pgm_pkt_hdr_rd),
.out_pgm_pkt_hdr_addr(ram2pgm_pkt_hdr_addr),
.in_pgm_pkt_hdr(ram2pgm_pkt_hdr),

.out_pgm_data(um2port1_data ),
.out_pgm_data_wr(um2port1_data_wr),
.out_pgm_data_valid(um2port1_data_valid),
.out_pgm_data_valid_wr(um2port1_data_valid_wr)
);

fsm fsm_1_inst(
.clk(clk),
.rst_n(rst_n),

//.in_fsm_test_start(test_start), 
.cnt_rst(cnt_rst),
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_1),
.lcm2fsm_5tuplemask(lcm2fsm_mask_1),

.fsm_pkt_num(fsm2lcm_pkt_1_cnt)
);

fsm fsm_2_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_2),
.lcm2fsm_5tuplemask(lcm2fsm_mask_2),

.fsm_pkt_num(fsm2lcm_pkt_2_cnt)
);

fsm fsm_3_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_3),
.lcm2fsm_5tuplemask(lcm2fsm_mask_3),

.fsm_pkt_num(fsm2lcm_pkt_3_cnt)
);

fsm fsm_4_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_4),
.lcm2fsm_5tuplemask(lcm2fsm_mask_4),

.fsm_pkt_num(fsm2lcm_pkt_4_cnt)
);

fsm fsm_5_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_5),
.lcm2fsm_5tuplemask(lcm2fsm_mask_5),

.fsm_pkt_num(fsm2lcm_pkt_5_cnt)
);

fsm fsm_6_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_6),
.lcm2fsm_5tuplemask(lcm2fsm_mask_6),

.fsm_pkt_num(fsm2lcm_pkt_6_cnt)
);

fsm fsm_7_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_7),
.lcm2fsm_5tuplemask(lcm2fsm_mask_7),

.fsm_pkt_num(fsm2lcm_pkt_7_cnt)
);

fsm fsm_8_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2fsm_5tuple(lcm2fsm_rule_5tuple_8),
.lcm2fsm_5tuplemask(lcm2fsm_mask_8),

.fsm_pkt_num(fsm2lcm_pkt_8_cnt)
);

ssm ssm_inst(
.clk(clk),
.rst_n(rst_n),

.cnt_rst(cnt_rst),
//.in_fsm_test_start(test_start), 
.pktin_data(port22um_data),
.pktin_data_wr(port22um_data_wr),

.lcm2ssm_time(timestamp),
.sample_freq(lcm2ssm_samp_freq),
.pkt_num(ssm2lcm_pkt_cnt),

.pktout_data(um2port3_data),
.pktout_data_wr(um2port3_data_wr),
.pktout_data_valid(um2port3_data_valid),
.pktout_data_valid_wr(um2port3_data_valid_wr)
);

ram_128_32 cache_gcl(    
.clka(clk),
.ena(lcm2ram_gc_wr),
.dina(lcm2ram_gc),
.wea(1'b1),
.addra(lcm2ram_gc_addr),	
   
.clkb(clk),
.enb(ram2pgm_gc_rd),
.addrb(ram2pgm_gc_addr),
.doutb(ram2pgm_gc)    
);

ram_128_64 cache_pkt_hdr(    
.clka(clk),
.ena(lcm2ram_pkt_hdr_wr),
.dina(lcm2ram_pkt_hdr),
.wea(1'b1),
.addra(lcm2ram_pkt_hdr_addr),	
   
.clkb(clk),
.enb(ram2pgm_pkt_hdr_rd),
.addrb(ram2pgm_pkt_hdr_addr),
.doutb(ram2pgm_pkt_hdr)    
);

endmodule



