
module fullAdder(input A, input B, input C_in, output C_out, output S);

assign C_out = (A & B)|(B & C_in)|(A & C_in);

assign S = A ^ B ^ C_in;

endmodule