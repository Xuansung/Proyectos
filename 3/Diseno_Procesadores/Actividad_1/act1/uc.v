module uc(input wire [5:0] opcode, input wire z, output reg s_inc, s_inm, we3, wez, output reg [2:0] op_alu);
    always @(*) begin
        s_inc = 1'b0;
        s_inm = 1'b0;
        we3 = 1'b0;
        wez = 1'b0;
        op_alu = 3'b000;
        casez (opcode)
            6'b1?????: begin // ALU  
                wez = 1;
                op_alu = opcode[4:2];
                s_inm = 1'b0;
                we3 = 1'b1;
                s_inc = 1'b1;
            end
            6'b01????: begin // INMEDIATO
                s_inm = 1'b1;
                s_inc = 1'b1;
                we3 = 1'b1;
                op_alu = 3'b000;
                wez = 1'b0;
            end
            6'b00001?: begin // SALTO INCONDICIONAL
                s_inc = 1'b0;
                s_inm = 1'b0;
                we3 = 1'b0;
                wez = 1'b0;
                op_alu = 3'b000;
            end
            6'b0010??: begin // SALTO COND SI Z ES 1
                s_inc = (z == 1'b1) ? 1'b0 : 1'b1;
                s_inm = 1'b0;
                we3 = 1'b0;
                wez = 1'b0;
                op_alu = 3'b000;
            end
            6'b0001??: begin // SALTO COND SI Z ES 0
                s_inc = (z == 1'b0) ? 1'b0 : 1'b1;
                s_inm = 1'b0;
                we3 = 1'b0;
                wez = 1'b0;
                op_alu = 3'b000;
            end
        endcase
    end
endmodule