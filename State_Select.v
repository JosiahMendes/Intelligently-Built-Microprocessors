module State_Select
(
	input e1,
	input [2:0] c,
	output [2:0] n
);

	assign n[2] = 0;
	assign n[1] = e1 & c[0];
	assign n[0] = ~c[2] & ~c[1] & ~c[0];
	
endmodule 