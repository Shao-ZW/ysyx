module DRAM_read_ctrl(
    input  [31:0] dram_rdata,
    input  [31:0] dram_raddr,
    input  [4:0]  load_type,
    output [31:0] load_data
);
    wire inst_lb;
    wire inst_lh;
    wire inst_lw;
    wire inst_lbu;
    wire inst_lhu;

    wire [31:0] rdata;
    wire [31:0] lb_data;
    wire [31:0] lh_data;
    wire [31:0] lw_data;
    wire [31:0] lbu_data;
    wire [31:0] lhu_data;


    assign inst_lb  = load_type[0];
    assign inst_lh  = load_type[1];
    assign inst_lw  = load_type[2];
    assign inst_lbu = load_type[3];
    assign inst_lhu = load_type[4];

    assign rdata = dram_rdata >> {dram_raddr[1:0], 3'b0};
    assign lb_data = {{24{rdata[7]}}, rdata[7:0]};
    assign lh_data = {{16{rdata[15]}}, rdata[15:0]};
    assign lw_data = rdata;
    assign lbu_data = {24'h0, rdata[7:0]};
    assign lhu_data = {16'h0, rdata[15:0]};

    assign load_data =  ({32{inst_lb}}  & lb_data)
                      | ({32{inst_lh}}  & lh_data)
                      | ({32{inst_lw}}  & lw_data)
                      | ({32{inst_lbu}} & lbu_data)
                      | ({32{inst_lhu}} & lhu_data);

endmodule
