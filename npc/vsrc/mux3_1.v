module mux3_1 #(parameter WIDTH = 32) (
    input  [WIDTH-1:0] in0,
    input  [WIDTH-1:0] in1,
    input  [WIDTH-1:0] in2,
    input  [2:0]       sel,
    output [WIDTH-1:0] out
);
    assign out =  ({32{sel[0]}} & in0)
                | ({32{sel[1]}} & in1)
                | ({32{sel[2]}} & in2);
    
endmodule
