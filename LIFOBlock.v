module LIFOBlock#(parameter depth = 16, log2_depth = log2(depth), log2_depthp1 = log2(depth+1))(
	input[15:0] data,
	input reset, clock, push, pop,

	output reg empty, full,
	output [15:0] q,
	output reg [log2_depthp1-1:0] count
	);

function integer log2;
	input [31:0] value;
	begin
		value = value-1;
		for (log2=0; value>0; log2=log2+1)
			value = value>>1;
	end
endfunction



wire writing = push && ( count < depth || pop);
wire reading = pop && count > 0;

reg[log2_depthp1-1:0] next_count;

always @(*)
	if(reset)
		next_count = 0;
	else if (writing && !reading)
		next_count = count+1;
	else if (reading && !writing)
		next_count = count-1;
	else
		next_count = count;

always @(posedge clock)
	count <= next_count;

always @(posedge clock)
	full <= next_count == depth;
	
always @(posedge clock)
	empty <= next_count == 0;
	

wire[5:0] stackPointer = writing ? count[log2_depth-1:0] : (count[log2_depth-1:0])-1;

reg [15:0] stack[depth-1:0];

always @(posedge clock)
	if (writing && !reading)
		stack[stackPointer] <= q;

reg[15:0] reader;
always @(posedge clock)
	if(reading)
		reader <= stack[stackPointer];

	
reg [15:0] writer;
always @(posedge clock)
	if(writing)
		writer <= data;

reg muxSelector;
always @(posedge clock)
	if(reset)
		muxSelector <= 0;
	else if(writing)
		muxSelector <= 0;
	else if (reading)
		muxSelector <= 1;
		
assign q = muxSelector ? reader : writer;
	
endmodule