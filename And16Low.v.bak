module AND16Low
(
	input [15:0]data,
	output eq
);

	wire [15:0]low;

	assign low = ~data;
	
	assign eq = &low;
	
endmodule