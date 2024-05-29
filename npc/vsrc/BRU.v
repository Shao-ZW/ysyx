module BRU(
    input  [7:0]  type,
    input  [31:0] src1, 
    input  [31:0] src2,
    input  [31:0] pc,
    input  [31:0] imm,
    output [31:0] target,
    output        taken
);
    wire inst_jal;
    wire inst_jalr;
    wire inst_beq;
    wire inst_bne;
    wire inst_blt;
    wire inst_bge;
    wire inst_bltu;
    wire inst_bgeu;

    wire [31:0] jalr_target
    wire [31:0] other_target;

    wire [31:0] adder_a;
    wire [31:0] adder_b;
    wire [31:0] adder_res;
    wire        adder_cin;
    wire        adder_cout;
    
    wire altb;
    wire altub;
    wire zero;

    wire beq;
    wire bne;
    wire blt;
    wire bge;
    wire bltu;
    wire bgeu;


    assign inst_jal  = type[0];
    assign inst_jalr = type[1];
    assign inst_beq  = type[2];
    assign inst_bne  = type[3];
    assign inst_blt  = type[4];
    assign inst_bge  = type[5];
    assign inst_bltu = type[6];
    assign inst_bgeu = type[7];

    assign jalr_target  = src1 + imm;
    assign other_target = pc + imm;
    
    assign adder_a = src1;
    assign adder_b = ~src2;
    assign adder_cin = 1'b1;
    assign (adder_cout, adder_res) = adder_a + adder_b + adder_cin;

    assign altb  = (src1[31] & ~src2[31]) | ( ~(src1[31] ^ src2[31]) & adder_res[31]);
    assign altub = ~adder_cout;
    assign zero  = ~(| adder_res);

    assign beq  = zero;
    assign bne  = ~zero;
    assign blt  = altb;
    assign bge  = ~altub;
    assign bltu = altub;
    assign bgeu = ~altb;

    assign taken =  ((inst_jal | inst_jalr) & 1'b1)
                    | (inst_beq & beq)
                    | (inst_bne & bne)
                    | (inst_blt & blt)
                    | (inst_bge & bge)
                    | (inst_bltu & bltu)
                    | (inst_bgeu & bgeu);

    assign target = inst_jalr ? jalr_target : other_target;

endmodule