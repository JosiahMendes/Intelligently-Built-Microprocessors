module ShiftDecoder3(input Shift_Left, input Shift_Right, input [3:0] SL, input [3:0] SR, output [3:0] X, output [3:0] Y);

assign X[3] = Shift_Left&SL[3];
assign X[2] = Shift_Left&SL[2];
assign X[1] = Shift_Left&SL[1];
assign X[0] = Shift_Left&SL[0];


assign Y[3] = Shift_Right&SR[3];
assign Y[2] = Shift_Right&SR[2];
assign Y[1] = Shift_Right&SR[1];
assign Y[0] = Shift_Right&SR[0];

endmodule