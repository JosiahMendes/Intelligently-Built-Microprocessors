module PCIncrement (input[10:0] in, output[10:0] out);

assign out = in +11'b00000000001;

endmodule