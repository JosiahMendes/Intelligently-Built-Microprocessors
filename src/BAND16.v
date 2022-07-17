module BAND16(input [15:0] A, input [15:0] B, output [15:0] X);

assign X[15] = A[15]&B[15];
assign X[14] = A[14]&B[14];
assign X[13] = A[13]&B[13];
assign X[12] = A[12]&B[12];
assign X[11] = A[11]&B[11];
assign X[10] = A[10]&B[10];
assign X[9] = A[9]&B[9];
assign X[8] = A[8]&B[8];
assign X[7] = A[7]&B[7];
assign X[6] = A[6]&B[6];
assign X[5] = A[5]&B[5];
assign X[4] = A[4]&B[4];
assign X[3] = A[3]&B[3];
assign X[2] = A[2]&B[2];
assign X[1] = A[1]&B[1];
assign X[0] = A[0]&B[0];

endmodule