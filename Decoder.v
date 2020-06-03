module Decoder (
	input [15:0] INSTR,
	output reg [15:0] q,
	output reg [1:0]out_sel,
	
	input fe, e1, e2, 
	
	output  instr_wren, instr_rden, data_wren, data_rden, pc_sload, pc_cnten, r0en, r1en, r2en, r3en, extra1, mux1_sel, mux2_sel, mux3_sel 
	
	
	
	);



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


wire stp, adr, adm, adi, sbr, sbm, sbi, mlr, bfe, xsl, xsr, bbo, stk, ldr, sti, ldi, sta, lda, jmr, jmp, jeq, jnq;

assign stp = ~A & ~B & ~C & ~D & ~E;

assign adr = ~A & ~B & ~C & ~D &  E; 
assign adm = ~A & ~B & ~C &  D & ~E;
assign adi = ~A & ~B & ~C &  D &  E;
assign sbr = ~A & ~B &  C & ~D & ~E;
assign sbm = ~A & ~B &  C & ~D &  E;
assign sbi = ~A & ~B &  C &  D & ~E;    
assign mlr = ~A & ~B &  C &  D &  E;
assign bfe = ~A &  B & ~C & ~D; 
assign xsl = ~A &  B & ~C &  D & ~E; 
assign xsr = ~A &  B & ~C &  D &  E;
assign bbo = ~A &  B &  C & ~D & ~E;

assign stk = ~A &  B &  C & ~D &  E;
assign ldr = ~A &  B &  C &  D & ~E;
assign sti =  A &  B &  C &  D &  E;
assign ldi =  A & ~B & ~C;
assign sta =  A & ~B &  C;
assign lda =  A &  B & ~C;

assign jmr =  A &  B &  C & ~D & ~E;
assign jmp =  A &  B &  C & ~D &  E;
assign jeq =  A &  B &  C &  D & ~E;
assign jnq =  A &  B &  C &  D &  E;


assign pc_cnten = e1 & (adr | adm | adi | sbr| sbm | sbi | mlr | xsl | xsr | bbo | bfe | ldi| sta | ldr | sti | stk| lda);
assign pc_sload = e1 & (jmp);

assign instr_wren = 0;
assign instr_rden = fe;

assign data_wren = (sta&e1);
assign data_rden = 1;

assign r0en = (ldi & ~D & ~E & e1) | (lda & ~D & ~E & e2) | (ldr & ~F & ~G & e2);
assign r1en = (ldi & ~D &  E & e1) | (lda & ~D &  E & e2) | (ldr & ~F &  G & e2);
assign r2en = (ldi &  D & ~E & e1) | (lda &  D & ~E & e2) | (ldr &  F & ~G & e2);
assign r3en = (ldi &  D &  E & e1) | (lda &  D &  E & e2) | (ldr &  F &  G & e2);

assign mux1_sel = (ldi&e1);
assign mux2_sel = (ldr&e1);
assign mux3_sel = (ldr);
assign extra1 = (lda)|(ldr);


always @(*)
	if ((ldi&e1)|(lda&e1)|(sta&e1)) begin 
		 q[15:11] =  5'b0;
		 q[10:0] = INSTR[10:0];
	end else 
		q[15:0] = INSTR[10:0];
	
always @(*)
	if (sta&e1)
		out_sel[1:0] = INSTR[12:11];
	else if (ldr&e1)
		out_sel[1:0] = INSTR[7:6];
	else 
		out_sel[1:0] = 2'b0;



endmodule
