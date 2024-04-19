module CPU(
    input clk,

);
    wire [31:0] instruction;
    wire [31:0] rs1_addr, rs2_addr, rd_addr;
    wire [31:0] imm;
    wire [31:0] rs1_data, rs2_data, rd_data;
    
    RegisterFile #(5, 32) RegisterFile_u(
        .clk(clk),
        .wdata(rd_addr),
        .waddr(rd_data),
        .wen(wen),
        .raddr1(rs1_addr),
        .raddr2(rs2_addr),
        .rdata1(rs1_data),
        .rdata2(rs2_data)
    );

    IFU IFU_u(
        .pc(pc),
        .instruction(instruction)
    );

    Control Control_u(
        .instruction(instruction);
    );

    IDU IDU_u(
        .instruction(instruction),
        .rs1_addr(rs1_addr),
        .rs2_addr(rs2_addr),
        .rd_addr(rd_addr),
        .imm(imm)
    );

    EXU EXU_u(
        .rs1_data(rs1_data),
        .rs2_data(rs2_data),
        .imm(imm),


    );

    MEMU MEMU_u(
        .raddr(),
        .waddr(),
        .wdata(),
        .wen(),
        .ren(),
        .rdata()
    );

endmodule
