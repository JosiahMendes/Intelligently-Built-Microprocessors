module NOT16(input [15:0] A, output [15:0] B);

assign B[15] = !A[15];
assign B[14] = !A[14];
assign B[13] = !A[13];
assign B[12] = !A[12];
assign B[11] = !A[11];
assign B[10] = !A[10];
assign B[9] = !A[9];
assign B[8] = !A[8];
assign B[7] = !A[7];
assign B[6] = !A[6];
assign B[5] = !A[5];
assign B[4] = !A[4];
assign B[3] = !A[3];
assign B[2] = !A[2];
assign B[1] = !A[1];
assign B[0] = !A[0];

endmodule