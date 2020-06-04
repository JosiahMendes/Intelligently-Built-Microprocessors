module ALUDecoder2(input [15:0] INSTR, input CARRY, input [15:0] Rn, input [15:0] Rm, input [15:0] Rx, output Shift_in, output ShiftCOUTSel, output [3:0] SL, output [3:0] SR, output [2:0] RnSelect, output [2:0] RmSelect, output [1:0] RxSelect, output CINadd_sub, output add_sub, output multiplication, output BBO, output [1:0] OPSel, output [2:0] COUTSel);

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


wire  adr, adm, adi, sbr, sbm, sbi, mlr, xsl, xsr, bbo, stk, ldr, sti;


assign adr = ~A & ~B & ~C & ~D &  E; 
assign adm = ~A & ~B & ~C &  D;
assign adi = ~A & ~B &  C & ~D & ~E;
assign sbr = ~A & ~B &  C & ~D &  E;
assign sbm = ~A & ~B &  C &  D;
assign sbi = ~A &  B & ~C & ~D & ~E;    
assign mlr = ~A &  B & ~C & ~D &  E;
assign xsl = ~A &  B & ~C &  D & ~E; 
assign xsr = ~A &  B & ~C &  D &  E;
assign bbo = ~A &  B &  C & ~D & ~E;

assign stk = ~A &  B &  C & ~D &  E;
assign ldr = ~A &  B &  C &  D & ~E;
assign sti = ~A &  B &  C &  D &  E;



assign RnSelect[2] = stk;
assign RnSelect[1] = ((adr|sbr|mlr|bbo)&M)|((adi|sbi)&F)|((ldr|sti)&I);
assign RnSelect[0] = ((adr|sbr|mlr|bbo)&N)|((adi|sbi)&G)|((ldr|sti)&J)|((adm|sbm)&E);

assign RmSelect[2] = (adm|sbm|adi|sbi)|((ldr|sti)&~H)&(stk&G);
assign RmSelect[1] = ((adr|sbr|mlr|bbo|xsl|xsr)&O)|((ldr|sti)&K)|((ldr|sti)&~H)|(stk&(G|H));
assign RmSelect[0] = ((adr|sbr|mlr|bbo|xsl|xsr)&P)|((ldr|sti)&L)|(adi|sbi)|(stk&(G|I));

assign RxSelect[1] = (adr|sbr|mlr)&K;
assign RxSelect[0] = (adr|sbr|mlr)&L;



assign Shift_in = (xsl|xsr)&((~G&H)|(G&~H&CARRY)|(G&H&Rm[15]));

assign ShiftCOUTSel = xsl;


assign SL[3] = (xsl&I)|( (adr|sbr|mlr) & I & ~J & Rx[3])|( (ldr|sti)&H&M);
assign SL[2] = (xsl&J)|( (adr|sbr|mlr) & I & ~J & Rx[2])|( (ldr|sti)&H&N);
assign SL[1] = (xsl&K)|( (adr|sbr|mlr) & I & ~J & Rx[1])|( (ldr|sti)&H&O);
assign SL[0] = (xsl&L)|( (adr|sbr|mlr) & I & ~J & Rx[0])|( (ldr|sti)&H&P);

assign SR[3] = (xsr&I)|( (adr|sbr|mlr) & I & J & Rx[3]);
assign SR[2] = (xsr&J)|( (adr|sbr|mlr) & I & J & Rx[2]);
assign SR[1] = (xsr&K)|( (adr|sbr|mlr) & I & J & Rx[3]);
assign SR[0] = (xsr&L)|( (adr|sbr|mlr) & I & J & Rx[3]);


assign CINadd_sub = ((adr|mlr)&((~G&H)|(G&~H&CARRY)|(G&H&Rm[15]))) | (sbr&((~G&~H)|(G&~H&~CARRY)|(G&H&~Rm[15]))) | (sbm|sbi|(stk&J));

assign add_sub = !(sbr|sbm|sbi|(stk&J));


assign multiplication = mlr;

assign BBO = bbo;

assign OPSel[1] = xsl|xsr;
assign OPSel[0] = ((adr|sbr|mlr)&~I&J)|bbo;

assign COUTSel[2] = (mlr&~I&J)|(sbi|sbm|sbr);
assign COUTSel[1] = xsl|xsr|(sbr&~I&J);
assign COUTSel[0] = (adr&~I&J)|(mlr&~(~I&J))|(sbm|sbi)|(sbr&~(~I&J));


endmodule