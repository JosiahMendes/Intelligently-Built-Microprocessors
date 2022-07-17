module Decoder_PipelinedCleanedUp(
	input [15:0] INSTR,
	output reg [1:0]out_sel,
	
	input fe, e1, e2, eq, stackFull, stackEmpty, jmrCond,
	
	output  instr_wren, instr_rden, 
	output data_wren, data_rden, 
	output pc_sload, pc_cnten, 
	output r0en, r1en, r2en, r3en, 
	output extra1,
	
	output carry_en,
	
	output reg [1:0] mux1_sel, 
	output mux2_sel,
	output reg[1:0] pcmux_sel,
	
	output pushEn, popEn
	
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


wire stp, adr, adm, adi, sbr, sbm, sbi, mlr, xsl, xsr, bbo, stk, ldr, sti, ldi, sta, lda, jmr, jmp, jeq, jnq;

assign stp = ~A & ~B & ~C & ~D & ~E;

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
assign ldi =  A & ~B & ~C;
assign sta =  A & ~B &  C;
assign lda =  A &  B & ~C;

assign jmr =  A &  B &  C & ~D & ~E;
assign jmp =  A &  B &  C & ~D &  E;
assign jeq =  A &  B &  C &  D & ~E;
assign jnq =  A &  B &  C &  D &  E;

wire psh, pop;

assign psh = stk & ~F;
assign pop = stk & F;


assign pc_cnten = (fe|e2)|(e1 & ~extra1 & ~stp);


assign pc_sload = e1 & ((jmp)|(jeq & eq)|(jnq & !eq)|(jmr & jmrCond)|(pop & G & ~H & ~I & !stackEmpty));

assign instr_wren = 0;
assign instr_rden = fe|(e1&~extra1)|e2;

assign data_wren = (sta&e1)|(sti&e1);
assign data_rden = 1;

assign r0en = (ldi & ~D & ~E & e1) | (lda & ~D & ~E & e2) | (ldr & ~F & ~G & e2) | (pop & ~G & ~H & ~I & e1 & !stackEmpty) | ((adr|sbr|mlr|bbo|xsl|xsr) & ~M & ~N & e1) | ((adi|sbi) & ~F & ~G & e1) | ((adm|sbm) & ~E & e2);
assign r1en = (ldi & ~D &  E & e1) | (lda & ~D &  E & e2) | (ldr & ~F &  G & e2) | (pop & ~G & ~H &  I & e1 & !stackEmpty) | ((adr|sbr|mlr|bbo|xsl|xsr) & ~M &  N & e1) | ((adi|sbi) & ~F &  G & e1) | ((adm|sbm) &  E & e2);
assign r2en = (ldi &  D & ~E & e1) | (lda &  D & ~E & e2) | (ldr &  F & ~G & e2) | (pop & ~G &  H & ~I & e1 & !stackEmpty) | ((adr|sbr|mlr|bbo|xsl|xsr) &  M & ~N & e1) | ((adi|sbi) &  F & ~G & e1);
assign r3en = (ldi &  D &  E & e1) | (lda &  D &  E & e2) | (ldr &  F &  G & e2) | (pop & ~G &  H &  I & e1 & !stackEmpty) | ((adr|sbr|mlr|bbo|xsl|xsr) &  M &  N & e1) | ((adi|sbi) &  F &  G & e1);


//assign mux1_sel = (ldi&e1);
assign mux2_sel = (ldr&e1)|(sti&e1);
assign extra1 = ((lda)|(ldr)|(adm)|(sbm))&e1;

assign carry_en = ((adr|sbr|mlr|xsl|xsr) & e1 & F) | ((adi|sbi) & e1) | ((adm|sbm) & e2);

assign pushEn = (psh & e1);
assign popEn = (pop & e1);

always @(*)
	if (ldi&e1)
		mux1_sel[1:0] = 2'b01;
	else if (((adr|sbr|mlr|bbo|xsl|xsr|adi|sbi)&e1)|((adm|sbm)&e2))
		mux1_sel[1:0] = 2'b10;
	else if (pop & e1 & ~G & !stackEmpty)
		mux1_sel[1:0] = 2'b11;
	else 
		mux1_sel[1:0] = 2'b00;

	
always @(*)
	if (sta&e1)
		out_sel[1:0] = INSTR[12:11];
	else if (sti&e1)
		out_sel[1:0] = INSTR[10:9];
	else if (jmr&e1)
		out_sel[1:0] = INSTR[1:0];
	else 
		out_sel[1:0] = 2'b0;


always @(*)
	if (jmr & e1) 
		pcmux_sel[1:0] = 2'b01;
	else if (pop & e1 & G & ~H & ~I & !stackEmpty)
		pcmux_sel[1:0] = 2'b10;
	else 
		pcmux_sel[1:0] = 2'b00;



endmodule
