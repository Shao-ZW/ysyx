module ECU(

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