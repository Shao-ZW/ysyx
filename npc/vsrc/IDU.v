module IDU(
    input [31:0] instruction,
    output [4:0] rs1_addr, 
    output [4:0] rs2_addr, 
    output [4:0] rd_addr,
    output [31:0] imm
    output [6:0] op_code;
);
    assign rd_addr = instruction[11:7];
    assign rs1_addr = instruction[19:15];
    assign rs2_addr = instruction[24:20];
    assign op_code = instruction[6:0];

    sign_extend sign_extend_u(
        .indata(),
        .imm(imm)
    );
    
endmodule