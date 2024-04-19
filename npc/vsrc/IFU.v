import "DPI-C" function int pmem_read(input int raddr);

module IFU(
    input [31:0] pc,
    output reg [31:0] instruction 
);
    always @(*) begin
        instruction = pmem_read(pc);
    end

endmodule