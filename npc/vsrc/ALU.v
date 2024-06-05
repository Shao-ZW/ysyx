module ALU (
    input  [9:0]  op,
    input  [31:0] src1,
    input  [31:0] src2,
    output [31:0] res
);
    wire op_add;
    wire op_sub;
    wire op_slt;
    wire op_sltu;
    wire op_and;
    wire op_or;
    wire op_xor;
    wire op_sll;
    wire op_srl;
    wire op_sra;

    assign op_add  = op[0];
    assign op_sub  = op[1];
    assign op_slt  = op[2];
    assign op_sltu = op[3];
    assign op_and  = op[4];
    assign op_or   = op[5];
    assign op_xor  = op[6];
    assign op_sll  = op[7];
    assign op_srl  = op[8];
    assign op_sra  = op[9];

    wire [31:0] add_sub_res;
    wire [31:0] slt_res;
    wire [31:0] sltu_res;
    wire [31:0] and_res;
    wire [31:0] or_res;
    wire [31:0] xor_res;
    wire [31:0] sll_res;
    wire [31:0] srl_res;
    wire [31:0] sra_res;

    assign and_res = src1 & src2;
    assign or_res  = src1 | src2;
    assign xor_res = src1 ^ src2;
    assign sll_res = src1 << src2[4:0];
    assign srl_res = src1 >> src2[4:0];
    assign sra_res = ($signed(src1)) >>> src2[4:0];
    
    wire [31:0] adder_a;
    wire [31:0] adder_b;
    wire        adder_cin;
    wire [31:0] adder_res;
    wire        adder_cout;
    wire [32:0] adder_tmp;

    assign adder_a   = src1;
    assign adder_b   = (op_sub | op_slt | op_sltu) ? ~src2 : src2;
    assign adder_cin = (op_sub | op_slt | op_sltu) ? 1'b1 : 1'b0;
    assign adder_tmp = {1'b0, adder_a} + {1'b0, adder_b} + {32'b0, adder_cin};

    assign adder_res  = adder_tmp[31:0];
    assign adder_cout = adder_tmp[32];

    assign add_sub_res = adder_res;

    assign slt_res[31:1] = 31'b0;
    assign slt_res[0]    = (src1[31] & ~src2[31]) | (~(src1[31] ^ src2[31]) & adder_res[31]);

    assign sltu_res[31:1] = 31'b0;
    assign sltu_res[0]    = ~adder_cout; // a + 2 ^ 32 - b

    assign res =  ({32{op_add | op_sub}} & add_sub_res)
                | ({32{op_slt}}  & slt_res)
                | ({32{op_sltu}} & sltu_res)
                | ({32{op_and}}  & and_res)
                | ({32{op_or}}   & or_res)
                | ({32{op_xor}}  & xor_res)
                | ({32{op_sll}}  & sll_res)
                | ({32{op_srl}}  & srl_res)
                | ({32{op_sra}}  & sra_res);
endmodule
