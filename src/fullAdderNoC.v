module fullAdderNoC(input A, input B, input C_in, output S);

assign S = A ^ B ^ C_in;

endmodule