module PC (
  input         clk,
  input         rst,
  input         wen,
  input  [31:0] next_pc,
  output        rst_done,
  output [31:0] pc
);
  reg [31:0] pc_reg;
  reg rst_done_reg;

  always @(posedge clk) begin
    if (rst) begin
      pc_reg <= 32'h80000000;
      rst_done_reg <= 1'b0;
    end
    else if(!rst_done_reg) begin
      rst_done_reg <= 1'b1;
    end
    else if (wen) begin
      pc_reg <= next_pc;
    end
  end

  assign pc = pc_reg;
  assign rst_done = rst_done_reg;
endmodule
