module uc(input wire q0,qu, start, clk, input reg [3:0] M, A, input reg [2:0] Q, output wire CargaQ, CargaM, ResetA, CargaA,ResetQu, DesplazaA,Suma, Resta, Fin)
 reg[2:0] state, nextstate;
 
 parameter S0 = 3'b000;
 parameter S1 = 3'b001;
 parameter S2 = 3'b010;
 parameter S3 = 3'b011;
 parameter S4 = 3'b100;
 parameter S5 = 3'b101;
 parameter S6 = 3'b110;
 parameter S7 = 3'b111;
 
 always @(posedge clk, posedge start) begin
 if (start)
	state <= 0;
 else
	state <= nextstate;
 end
 
 always @(*) // (*) significa cualquier cambio en alguna variable del bloque
	case (state)
		S0: nextstate = S1;
		S1: nextstate = S2;
		S2: nextstate = S3;
		S3: nextstate = S4;
		S4: nextstate = S5;
		S5: nextstate = S6;
		S6: nextstate = S7;
		S7: nextstate = S7;
		default: nextstate = S0;
	endcase
	
	assign ResetA = (state == S0)? 1:0;
	assign ResetQu = (state == S0)? 1:0;
	assign CargaQ = (state == S0)? 1:0;
	assign CargaM = (state == S0)? 1:0;
	assign Resta = ((q0 == 1 & qu == 0) & (state == S1 | state == S3 | state == S5))? 1:0;
	assign Suma = ((~Resta) & (state == S1 | state == S3 | state == S5))? 1:0;
	assign CargaA = ((q0 == 1 & qu == 0) | (q0 == 0 & qu == 1) & (state == S1 | state == S3 | state == S5))? 1:0;
	assign DesplazaA = ((state == S2) | (state == S4) | (state == S6))? 1:0;
	assign Fin = (state == S7)? 1:0;

end
	
