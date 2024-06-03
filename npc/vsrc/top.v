module top(
    input clk,
    input rst
);
    wire        iram_en;
    wire [31:0] inst;
    wire [31:0] iram_addr;
    
    wire        dram_en;
    wire        dram_wen;
    wire [31:0] dram_rdata;
    wire [31:0] dram_addr;
    wire [31:0] dram_wdata;
    wire [3:0]  dram_wmask;
    
    CPU u_CPU(
        .clk(clk),
        .rst(rst),
        .inst(inst),
        .dram_rdata(dram_rdata),
        .iram_en(iram_en),
        .iram_addr(iram_addr),
        .dram_en(dram_en),
        .dram_wen(dram_wen),
        .dram_addr(dram_addr),
        .dram_wdata(dram_wdata),
        .dram_wmask(dram_wmask)
    );

    // DRAM u_DRAM(
    //     .clk(clk),
    //     .dram_en(dram_en),
    //     .dram_wen(dram_wen),
    //     .dram_addr(dram_addr),
    //     .dram_wdata(dram_wdata),
    //     .dram_wmask(dram_wmask),
    //     .dram_rdata(dram_rdata)
    // );

    IRAM u_IRAM(
        .iram_en(iram_en),
        .iram_addr(iram_addr),
        .inst(inst)
    );

endmodule
