module Decrement (input[15:0] in, input enable, output reg [15:0] out);
always @ (*) begin
	if (enable)
	out = in - 16'b0000000000000001;
	else
	out = in;
end
endmodule