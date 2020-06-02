module ALUDecoder(input [15:0] INSTR, input CARRY, input [15:0] Rn, input [15:0] Rm, input [15:0] Rx, output Shift_in, output Shift_Left, output Shift_Right, output BFE, output [3:0] SL, output [3:0] SR, output [1:0] RnSelect, output [2:0] RmSelect, output [1:0] RxSelect, output CINadd_sub, output add_sub, output multiplication, output BBO, output [1:0] OPSel);

wire A, B,C, D, E, F, G, H, I, J, K, L, M, N, O, P;

assign A = INSTR[15];
assign B = INSTR[14];
assign C = INSTR[13];
assign D = INSTR[12];
assign E = INSTR[11];
assign F = INSTR[10];
assign G = INSTR[9];
assign H = INSTR[8];
assign I = INSTR[7];
assign J = INSTR[6];
assign K = INSTR[5];
assign L = INSTR[4];
assign M = INSTR[3];
assign N = INSTR[2];
assign O = INSTR[1];
assign P = INSTR[0];


wire adr, adm, adi, sbr, sbm, sbi, mlr, mlm, xsl, xsr, bbo, bfe, ldr, sti;

assign adr = ~A&~B&~C&~D&E; 
assign adm = ~A&~B&~C&D&~E;
assign adi = ~A&~B&~C&D&E;
assign sbr = ~A&~B&C&~D&~E;
assign sbm = ~A&~B&C&~D&E;
assign sbi = ~A&~B&C&D&~E;    
assign mlr = ~A&~B&C&D&E;
assign mlm = ~A&B&~C&~D&~E; 
assign xsl = ~A&B&~C&~D&E; 
assign xsr = ~A&B&~C&D&~E;
assign bbo = ~A&B&~C&D&E;
assign bfe = ~A&B&C&~D;
assign ldr = A&B&~C&~D&~E;
assign sti = A&B&~C&~D&E;



/*assign Rn[i] = ((adr|sbr|mlr|bbo)&( (~M&~N&R0[i]) | (~M&N&R1[i]) | (M&~N&R2[i]) | (M&N&R3[i]) ))|( (adi|sbi)&( (~F&~G&R0[i]) | (~F&~G&R1[i]) |(F&~G&R2[i]) |(F&G&R3[i]) ) );

assign Rm[i] = ((adr|sbr|mlr|bbo|bfe|xsl|xsr)&( (~O&~P&R0[i]) | (~O&P&R1[i]) | (O&~P&R2[i]) | (O&P&R3[i]) ))|((ldr|sti)&( (~K&~L&R0[i]) | (~K&L&R1[i]) | (K&~L&R2[i]) | (K&L&R3[i]) ));

assign Rx[i] = ((adr|sbr|mlr)&( (~K&~L&R0[i]) | (~K&L&R1[i]) | (K&~L&R2[i]) | (K&L&R3[i]) )); */



assign RnSelect[1] = ((adr|sbr|mlr|bbo)&M)|((adi|sbi)&F)|((ldr|sti)&I);
assign RnSelect[0] = ((adr|sbr|mlr|bbo)&N)|((adi|sbi)&G)|((ldr|sti)&J);

assign RmSelect[2] = (adm|sbm|mlm|adi|sbi)|((ldr|sti)&~H);
assign RmSelect[1] = ((adr|sbr|mlr|bbo|bfe|xsl|xsr)&O)|((ldr|sti)&K)|((ldr|sti)&~H);
assign RmSelect[0] = ((adr|sbr|mlr|bbo|bfe|xsl|xsr)&P)|((ldr|sti)&L)|(adi|sbi);

assign RxSelect[1] = (adr|sbr|mlr)&K;
assign RxSelect[0] = (adr|sbr|mlr)&L;



assign Shift_in = (xsl|xsr)&((~G&H)|(G&~H&CARRY)|(G&H&Rm[15]));

assign Shift_Left = ((adr|sbr|mlr)&(I&~J))|((ldr|sti)&H)|xsl;

assign Shift_Right = ((adr|sbr|mlr)&(I&J))|xsr;

assign BFE = bfe;

assign SL[3] = (xsl&I)|(bfe&~E)|((adr|sbr|mlr)&Rx[3])|(((ldr|sti)&M));
assign SL[2] = (xsl&J)|(bfe&~F)|((adr|sbr|mlr)&Rx[2])|(((ldr|sti)&N));
assign SL[1] = (xsl&K)|(bfe&~G)|((adr|sbr|mlr)&Rx[1])|(((ldr|sti)&O));
assign SL[0] = (xsl&L)|(bfe&~H)|((adr|sbr|mlr)&Rx[0])|(((ldr|sti)&M));

assign SR[3] = (xsr&I)|(bfe&I)|((adr|sbr|mlr)&Rx[3]);
assign SR[2] = (xsr&J)|(bfe&J)|((adr|sbr|mlr)&Rx[2]);
assign SR[1] = (xsr&K)|(bfe&K)|((adr|sbr|mlr)&Rx[1]);
assign SR[0] = (xsr&L)|(bfe&L)|((adr|sbr|mlr)&Rx[0]);


assign CINadd_sub = ((adr|mlr)&((~G&H)|(G&~H&CARRY)|(G&H&Rm[15]))) | (sbr&((~G&~H)|(G&~H&~CARRY)|(G&H&~Rm[15]))) | (sbm|sbi);

assign add_sub = !(sbr|sbm|sbi);


assign multiplication = mlr|mlm;

assign BBO = bbo;

assign OPSel[1] = bfe|xsl|xsr;
assign OPSel[0] = ((adr|sbr|mlr)&~I&J)|bbo;


endmodule