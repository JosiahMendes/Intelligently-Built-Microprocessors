module JMRDecoder(
    input [15:0] INSTR,
    input CARRY,
    input [15:0] Rn,
    input [15:0] Rx,
    output JUMP
);

wire [3:0] x_value = INSTR[7:4];
wire x_zero = !x_value[3] & !x_value[2] & !x_value[1] & !x_value[0];
wire x_one = !x_value[3] & !x_value[2] & !x_value[1] & x_value[0];
wire x_two = !x_value[3] & !x_value[2] & x_value[1] & !x_value[0];
wire x_three = !x_value[3] & !x_value[2] & x_value[1] & x_value[0];
wire x_four = !x_value[3] & x_value[2] & !x_value[1] & !x_value[0];
wire x_five = !x_value[3] & x_value[2] & !x_value[1] & x_value[0];
wire x_six = !x_value[3] & x_value[2] & x_value[1] & !x_value[0];
wire x_seven = !x_value[3] & x_value[2] & x_value[1] & x_value[0];
wire x_eight = x_value[3] & !x_value[2] & !x_value[1] & !x_value[0];
wire x_nine = x_value[3] & !x_value[2] & !x_value[1] & x_value[0];
wire x_ten = x_value[3] & !x_value[2] & x_value[1] & !x_value[0];
wire x_eleven = x_value[3] & !x_value[2] & x_value[1] & x_value[0];
wire x_twelve = x_value[3] & x_value[2] & !x_value[1] & !x_value[0];
wire x_thirteen = x_value[3] & x_value[2] & !x_value[1] & x_value[0];
wire x_fourteen = x_value[3] & x_value[2] & x_value[1] & !x_value[0];
wire x_fifteen = x_value[3] & x_value[2] & x_value[1] & x_value[0];
wire Rn_xth_bit = x_zero&Rn[0] | x_one&Rn[1] | x_two&Rn[2] | x_three&Rn[3] | x_four&Rn[4] | x_five&Rn[5] | x_six&Rn[6] | x_seven&Rn[7] | x_eight&Rn[8] | x_nine&Rn[9] | x_ten&Rn[10] | x_eleven&Rn[11] | x_twelve&Rn[12] | x_thirteen&Rn[13] | x_fourteen&Rn[14] | x_fifteen&Rn[15];

wire [2:0] condinstr = INSTR[10:8];
wire cond_zero = !condinstr[2] & !condinstr[1] & !condinstr[0];
wire cond_one = !condinstr[2] & !condinstr[1] & condinstr[0];
wire cond_two = !condinstr[2] & condinstr[1] & !condinstr[0];
wire cond_three = !condinstr[2] & condinstr[1] & condinstr[0];
wire cond_four = condinstr[2] & !condinstr[1] & !condinstr[0];
wire cond_five = condinstr[2] & !condinstr[1] & condinstr[0];
wire cond_six = condinstr[2] & condinstr[1] & !condinstr[0];
wire cond_seven = condinstr[2] & condinstr[1] & condinstr[0];

wire Rn_zero = !(Rn[15]|Rn[14]|Rn[13]|Rn[12]|Rn[11]|Rn[10]|Rn[9]|Rn[8]|Rn[7]|Rn[6]|Rn[5]|Rn[4]|Rn[3]|Rn[2]|Rn[1]|Rn[0]);
wire Rn_equal_Rx = !((Rn[15]^Rx[15])|(Rn[14]^Rx[14])|(Rn[13]^Rx[13])|(Rn[12]^Rx[12])|(Rn[11]^Rx[11])|(Rn[10]^Rx[10])|(Rn[9]^Rx[9])|(Rn[8]^Rx[8])|(Rn[7]^Rx[7])|(Rn[6]^Rx[6])|(Rn[5]^Rx[5])|(Rn[4]^Rx[4])|(Rn[3]^Rx[3])|(Rn[2]^Rx[2])|(Rn[1]^Rx[1])|(Rn[0]^Rx[0]));
wire Rn_less_than_Rx = !Rn[15]&Rx[15] | !(Rn[15]^Rx[15])&(!Rn[14]&Rx[14] | !(Rn[14]^Rx[14])&(!Rn[13]&Rx[13] | !(Rn[13]^Rx[13])&(!Rn[12]&Rx[12] | !(Rn[12]^Rx[12])&(!Rn[11]&Rx[11] | !(Rn[11]^Rx[11])&(!Rn[10]&Rx[10] | !(Rn[10]^Rx[10])&(!Rn[9]&Rx[9] | !(Rn[9]^Rx[9])&(!Rn[8]&Rx[8] | !(Rn[8]^Rx[8])&(!Rn[7]&Rx[7] | !(Rn[7]^Rx[7])&(!Rn[6]&Rx[6] | !(Rn[6]^Rx[6])&(!Rn[5]&Rx[5] | !(Rn[5]^Rx[5])&(!Rn[4]&Rx[4] | !(Rn[4]^Rx[4])&(!Rn[3]&Rx[3] | !(Rn[3]^Rx[3])&(!Rn[2]&Rx[2] | !(Rn[2]^Rx[2])&(!Rn[1]&Rx[1] | !(Rn[1]^Rx[1])&!Rn[0]&Rx[0]))))))))))))));

assign JUMP = (cond_zero & Rn_zero) | 
            (cond_one & !Rn_zero) | 
            (cond_two & Rn_equal_Rx) |
            (cond_three & !Rn_equal_Rx) |
            (cond_four & Rn_less_than_Rx) |
            (cond_five & (Rn_equal_Rx | Rn_less_than_Rx)) |
            (cond_six & Rn_xth_bit) |
            (cond_seven & CARRY);

endmodule