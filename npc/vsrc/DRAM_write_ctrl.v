module DRAM_write_ctrl(
    input  [31:0] wdata,
    input  [31:0] dram_waddr,
    input  [2:0]  store_type,
    output [3:0]  dram_wmask,
    output [31:0] dram_wdata
);
    wire inst_sb;
    wire inst_sh;
    wire inst_sw;

    wire sb_mask;
    wire sh_mask;
    wire sw_mask;

    assign inst_sb = store_type[0];
    assign inst_sh = store_type[1];
    assign inst_sw = store_type[2];

    assign sb_mask = 4'b0001 << dram_waddr[1:0];
    assign sh_mask = 4'b0011 << dram_waddr[1:0];
    assign sw_mask = 4'b1111;

    assign dram_wdata = wdata << {dram_waddr[1:0], 3'b0};
    
    assign dram_wmask =  ({32{inst_sb}} & sb_mask)
                       | ({32{inst_sh}} & sh_mask)
                       | ({32{inst_sw}} & sw_mask); 
endmodule