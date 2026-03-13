module uc(input wire [7:0] opcode, input wire z, c, n, output reg s_inc, we3, wf, m_wr, r_inm, push, pop, output reg [1:0] wd, output reg [3:0] op_alu);
    always @(*) begin
        s_inc = 1'b0; // Flag incremento pc
        pop = 1'b0; // Flag de pop
        push = 1'b0; // Flag de push
        r_inm = 1'b0; // Flag inmediato
        m_wr = 1'b0; // Flag escribir memoria
        wd = 2'b00; // Mux seleccionar resultado
        we3 = 1'b0; // Flag escritura banco registros
        wf = 1'b0; //Flag alu
        op_alu = 4'b0000; //Opcode alu 
        casez (opcode) 
            8'b0010????: begin // ALU(reg)  
                wf = 1'b1;
                wd = 2'b01;
                we3 = 1'b1;
                s_inc = 1'b1;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = opcode[3:0];
            end
            8'b0011????: begin // ALU(inm)
                wf = 1'b1;
                wd = 2'b01;
                we3 = 1'b1;
                s_inc = 1'b1;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b1;
                m_wr = 1'b0;
                op_alu = opcode[3:0];
            end
            8'b00001000: begin // GUARDA EN REGISTRO(LW)
                wf = 1'b0;
                wd = 2'b10;
                we3 = 1'b1;
                s_inc = 1'b1;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b1;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00001001: begin // GUARDA EN MEMORIA(SW)
                wf = 1'b0;
                wd = 2'b01;
                we3 = 1'b0;
                s_inc = 1'b1;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b1;
                m_wr = 1'b1;
                op_alu = 4'b0000;
            end
            8'b00010010: begin // SALTO A SUBRUTINA(CALL X)
                s_inc = 1'b1;
                wd = 2'b00;
                wf = 1'b0;
                we3 = 1'b0;
                pop = 1'b0;
                push = 1'b1;
                r_inm = 1'b1;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00010011: begin // RETORNO DE SUBRUTINA (RET)
                s_inc = 1'b0;
                wd = 2'b00;
                wf = 1'b0;
                we3 = 1'b0;
                pop = 1'b1;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00010001: begin // INMEDIATO (LI)
                s_inc = 1'b1;
                wd = 2'b11;
                we3 = 1'b1;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000010: begin // SALTO INCONDICIONAL (JMP)
                s_inc = 1'b0;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000011: begin // SALTO COND SI Z ES 1 (BEQ)
                s_inc = (z == 1'b1) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000001: begin // SALTO COND SI Z ES 0 (BNE)
                s_inc = (z == 1'b0) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000100: begin // SALTO COND SI C ES 1 (BCS)
                s_inc = (c == 1'b1) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000101: begin // SALTO COND SI C ES 0 (BCN)
                s_inc = (c == 1'b0) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000111: begin // SALTO COND SI N ES 1 (BNN)
                s_inc = (n == 1'b1) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000110: begin // SALTO COND SI N ES 0 (BNS)
                s_inc = (n == 1'b0) ? 1'b0 : 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00000000: begin // INSTRUCCION NOP
                s_inc = 1'b1;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
            8'b00011111: begin // INSTRUCCION HALT
                s_inc = 1'b0;
                wd = 2'b00;
                we3 = 1'b0;
                wf = 1'b0;
                pop = 1'b0;
                push = 1'b0;
                r_inm = 1'b0;
                m_wr = 1'b0;
                op_alu = 4'b0000;
            end
        endcase
    end
endmodule