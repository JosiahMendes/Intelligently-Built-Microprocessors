module ShiftDecoder(input Shift_Left, input Shift_Right, input BFE, input [11:4] INSTR, output [3:0] X, output [3:0] Y, output [3:0] Z);

assign X[3] = (Shift_Left&INSTR[7])|(BFE&!INSTR[11]);
assign X[2] = (Shift_Left&INSTR[6])|(BFE&!INSTR[10]);
assign X[1] = (Shift_Left&INSTR[5])|(BFE&!INSTR[9]);
assign X[0] = (Shift_Left&INSTR[4])|(BFE&!INSTR[8]);

wire [3:0] sum;
wire [3:0] A;
wire [3:0] B;

assign A[3] = !INSTR[11];
assign A[2] = !INSTR[10];
assign A[1] = !INSTR[9];
assign A[0] = !INSTR[8];

assign B[3] = INSTR[7];
assign B[2] = INSTR[6];
assign B[1] = INSTR[5];
assign B[0] = INSTR[4];

assign sum = A+B;

assign Y[3] = (Shift_Right&INSTR[7])|(BFE&sum[3]);
assign Y[2] = (Shift_Right&INSTR[6])|(BFE&sum[2]);
assign Y[1] = (Shift_Right&INSTR[5])|(BFE&sum[1]);
assign Y[0] = (Shift_Right&INSTR[4])|(BFE&sum[0]);

assign Z[3] = BFE&INSTR[7];
assign Z[2] = BFE&INSTR[6];
assign Z[1] = BFE&INSTR[5];
assign Z[0] = BFE&INSTR[4];



endmodule