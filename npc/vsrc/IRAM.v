import "DPI-C" function int pmem_read(input int raddr);

module IRAM(
    input             iram_en,
    input      [31:0] iram_addr,
    output reg [31:0] inst
);
    always @(*) begin
        if (iram_en) begin 
            inst = pmem_read(iram_addr);
        end
        else begin
            inst = 0;
        end
    end

endmodule