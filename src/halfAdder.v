module halfAdder(input A, input B, output C, output S);

assign C = A & B;

assign S = A ^ B;

endmodule