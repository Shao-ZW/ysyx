module DRAM (
    input             clk,
    input             dram_en,
    input             dram_wen,
    input      [31:0] dram_addr,
    input      [31:0] dram_wdata,
    input      [3:0]  dram_wmask,
    output reg [31:0] dram_rdata
);
    import "DPI-C" function int pmem_read(input int raddr);
    import "DPI-C" function void pmem_write(input int waddr, input int wdata, input byte wmask);

    always @(*) begin
        if (dram_en) begin 
            dram_rdata = pmem_read(dram_addr);
        end
        else begin
            dram_rdata = 0;
        end
    end

    always @(posedge clk) begin
        if(dram_en & dram_wen) begin
            pmem_write(dram_addr, dram_wdata, {4'b0000, dram_wmask});
        end
    end

endmodule
