module GenPC (
    input         rst,
    input  [31:0] pc,
    input  [31:0] jump_target,
    input         jump_taken,
    output [31:0] next_pc
);
    assign next_pc = (rst == 1'b1) ? pc :
                     (jump_taken == 1'b1) ? jump_target : 
                                            pc + 32'd4;  

endmodule
