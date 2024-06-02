module PC (
  input         clk,
  input         rst,
  input         wen,
  input  [31:0] next_pc,
  output [31:0] pc
);
  reg [31:0] pc_reg;

  always @(posedge clk) begin
    if (rst) 
      pc_reg <= 32'h80000000;
    else if (wen) 
      pc_reg <= next_pc;
  end

  assign pc = pc_reg;

endmodule
