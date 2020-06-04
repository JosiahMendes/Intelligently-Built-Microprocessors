module State_Output
(
	input [2:0]s,
	output FETCH, EXEC1, EXEC2

);

	assign FETCH = ~s[2] & ~s[1] & ~s[0];
	assign EXEC1 = ~s[2] & ~s[1] &  s[0];
	assign EXEC2 = ~s[2] &  s[1] & ~s[0];

endmodule 