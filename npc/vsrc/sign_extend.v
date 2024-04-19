module sign_extend(
    input signed [n-1:0] input_data,  // 输入的有符号数，假设n位
    output signed [m-1:0] extended_data  // 扩展后的有符号数，假设m位
);

    integer i;

    always @* begin
        if (input_data[n-1] == 1'b0) begin  // 如果输入数据的最高位为0，则为正数，执行零扩展
            extended_data = {m-n{1'b0}, input_data};
        end
        else begin  // 否则为负数，执行符号扩展
            extended_data = {m-n{1'b1}, input_data};
        end
    end

endmodule
