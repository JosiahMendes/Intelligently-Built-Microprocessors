//Adapted from https://www.beyond-circuits.com/wordpress/2009/10/lifo/ with permission from author, Pete Johnson
//Adapted from https://esrd2014.blogspot.com/p/last-in-first-out-buffer.html

module LIFOBlock #(parameter depth = 16, log2_depth = 4, log2_depthp1 = 5)
(
	input[15:0] data,
	input clock, push, pop,

	output reg empty, full,
	output [15:0] q

);


reg [log2_depthp1-1:0] count;
reg[log2_depthp1-1:0] next_count;

wire writing = push && (pop || count < depth );
wire reading = pop && count > 0;

always @(*)
	if (writing && !reading)
		next_count = count+1; //Combinational logic to determine next state of stack. 
	else if (reading && !writing)
		next_count = count-1;
	else
		next_count = count;

always @(posedge clock)
	count <= next_count;

always @(posedge clock)
	full <= next_count == depth; //Checks if stack is Full.
	
always @(posedge clock)
	empty <= next_count == 0; //Checks if stack is empty.
	

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
	if(writing)
		muxSelector <= 0;
	else if (reading)
		muxSelector <= 1;
		
assign q = muxSelector ? reader : writer;
	
endmodule