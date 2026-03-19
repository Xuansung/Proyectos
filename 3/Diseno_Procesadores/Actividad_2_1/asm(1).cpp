/*
 * asm.cpp
 * Ensamblador para ISA de 32 bits - Actividad 2
 * Diseño de Procesadores
 *
 * ============================================================
 * FORMATO DE INSTRUCCIONES (todos los campos en orden MSB..LSB)
 * ============================================================
 *
 *  ALU reg   [31:28]=0010  [27:24]=Op  [23:20]=RD  [19:16]=R2  [15:12]=R1  [11:0]=0
 *  ALU imm   [31:28]=0011  [27:24]=Op  [23:20]=R2  [19:16]=R1  [15:0]=imm(16b)
 *  LW        [31:24]=00001000  [23:20]=R2  [19:16]=R1  [15:0]=offset(16b)
 *  SW        [31:24]=00001001  [23:20]=R2  [19:16]=R1  [15:0]=offset(16b)
 *  LI        [31:24]=00010001  [23:20]=R2  [19:16]=0   [15:0]=imm(16b)
 *  CALL      [31:24]=00010010  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  RET       [31:24]=00010011  [23:0]=0
 *  JMP       [31:24]=00000010  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BEQ       [31:24]=00000011  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BNE       [31:24]=00000001  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BCS       [31:24]=00000100  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BCN       [31:24]=00000101  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BNN       [31:24]=00000110  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  BNS       [31:24]=00000111  [23:16]=0   [15:6]=dest(10b)  [5:0]=0
 *  HALT      [31:0]=todos 1s
 *  NOP       [31:0]=todos 0s
 *
 * ============================================================
 * OPERACIONES ALU (campo Op[27:24], segun alu.v)
 * ============================================================
 *  0000 = mova  / movai   -> y = a
 *  0001 = nota  / notai   -> y = ~a
 *  0010 = add   / addi    -> y = a + b
 *  0011 = sub   / subi    -> y = a - b
 *  0100 = and   / andi    -> y = a & b
 *  0101 = or    / ori     -> y = a | b
 *  0110 = nega  / negai   -> y = -a
 *  0111 = negb  / negbi   -> y = -b
 *  1000 = movb  / movbi   -> y = b
 *
 * ============================================================
 * SINTAXIS DEL ENSAMBLADOR
 * ============================================================
 *  etiqueta:                        ; define simbolo = contador actual
 *  SYM equ valor                    ; define simbolo con valor constante
 *  ; comentario hasta fin de linea
 *
 *  ALU registro:   add  RD, R2, R1
 *  ALU inmediato:  addi R2, R1, imm
 *  Memoria:        lw   R2, offset(R1)
 *                  sw   R2, offset(R1)
 *  Carga inm.:     li   R2, imm
 *  Saltos/call:    jmp  destino  |  beq destino  |  call destino ...
 *  Sin operandos:  ret  |  nop  |  halt
 *
 *  Registros: R0..R15 (insensible a mayusculas)
 *  Inmediatos: decimal, 0xHEX o 0OCT (negativos con -)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================
 *  Constantes del ISA
 * ============================================================ */
#define INSTSIZE      32    /* Bits por instruccion                          */
#define REGSIZE        4    /* Bits para numero de registro (R0..R15)        */
#define IMMSIZE16     16    /* Bits de inmediato (LW/SW/LI/ALU imm)         */
#define DESTDIRSIZE   10    /* Bits de direccion en saltos/call [15:6]       */

/* Tamano maximo del programa: 2^DESTDIRSIZE palabras */
#define MAXPROGRAMLEN (1 << DESTDIRSIZE)

/* ============================================================
 *  Constantes del ensamblador
 * ============================================================ */
#define MAXLINE       256
#define MAXSYMBOLLEN   50
#define MAXSYMBREFS  1000
#define MAXSYMBOLS   1000

/* ============================================================
 *  Tipo de instruccion (determina como se leen los operandos)
 * ============================================================ */
typedef enum {
    T_ALU_REG,   /* mnem RD, R2, R1          */
    T_ALU_IMM,   /* mnem R2, R1, imm         */
    T_MEM,       /* lw/sw R2, offset(R1)     */
    T_LI,        /* li R2, imm               */
    T_BRANCH,    /* jmp/beq/... dest         */
    T_NOOPER     /* ret, nop, halt           */
} InstrType;

/* ============================================================
 *  Tabla de instrucciones
 *
 *  opcode8 : 8 bits mas significativos [31:24] como cadena de '0'/'1'
 *            (para ALU incluye los 4 bits de Op en [27:24])
 *  type    : como procesar los operandos
 * ============================================================ */
struct InstrDef {
    const char* mnemonic;
    char        opcode8[9]; /* 8 chars + '\0' */
    InstrType   type;
};

static const InstrDef instrTable[] = {
    /* ---- ALU registro [31:28]=0010, Op en [27:24] ---- */
    { "mova",  "00100000", T_ALU_REG },  /* y = a            */
    { "nota",  "00100001", T_ALU_REG },  /* y = ~a           */
    { "add",   "00100010", T_ALU_REG },  /* y = a + b        */
    { "sub",   "00100011", T_ALU_REG },  /* y = a - b        */
    { "and",   "00100100", T_ALU_REG },  /* y = a & b        */
    { "or",    "00100101", T_ALU_REG },  /* y = a | b        */
    { "nega",  "00100110", T_ALU_REG },  /* y = -a           */
    { "negb",  "00100111", T_ALU_REG },  /* y = -b           */
    { "movb",  "00101000", T_ALU_REG },  /* y = b            */
    /* ---- ALU inmediato [31:28]=0011, Op en [27:24] ---- */
    { "movai", "00110000", T_ALU_IMM },  /* y = a            */
    { "notai", "00110001", T_ALU_IMM },  /* y = ~a           */
    { "addi",  "00110010", T_ALU_IMM },  /* y = a + b        */
    { "subi",  "00110011", T_ALU_IMM },  /* y = a - b        */
    { "andi",  "00110100", T_ALU_IMM },  /* y = a & b        */
    { "ori",   "00110101", T_ALU_IMM },  /* y = a | b        */
    { "negai", "00110110", T_ALU_IMM },  /* y = -a           */
    { "negbi", "00110111", T_ALU_IMM },  /* y = -b           */
    { "movbi", "00111000", T_ALU_IMM },  /* y = b            */
    /* ---- Memoria ---- */
    { "lw",    "00001000", T_MEM     },  /* [31:24] = 0x08   */
    { "sw",    "00001001", T_MEM     },  /* [31:24] = 0x09   */
    /* ---- Carga inmediata ---- */
    { "li",    "00010001", T_LI      },  /* [31:24] = 0x11   */
    /* ---- Saltos y llamada ---- */
    { "call",  "00010010", T_BRANCH  },  /* [31:24] = 0x12   */
    { "jmp",   "00000010", T_BRANCH  },  /* [31:24] = 0x02   */
    { "beq",   "00000011", T_BRANCH  },  /* [31:24] = 0x03   */
    { "bne",   "00000001", T_BRANCH  },  /* [31:24] = 0x01   */
    { "bcs",   "00000100", T_BRANCH  },  /* [31:24] = 0x04   */
    { "bcn",   "00000101", T_BRANCH  },  /* [31:24] = 0x05   */
    { "bnn",   "00000110", T_BRANCH  },  /* [31:24] = 0x06   */
    { "bns",   "00000111", T_BRANCH  },  /* [31:24] = 0x07   */
    /* ---- Sin operandos ---- */
    { "ret",   "00010011", T_NOOPER  },  /* [31:24] = 0x13   */
    { "halt",  "11111111", T_NOOPER  },  /* todos 1s         */
    { "nop",   "00000000", T_NOOPER  },  /* todos 0s         */
};

#define NUMINSTR (int)(sizeof(instrTable) / sizeof(instrTable[0]))

/* ============================================================
 *  Memoria de programa y buffer de instruccion activa
 *
 *  Cada instruccion se representa como una cadena de INSTSIZE
 *  caracteres '0'/'1'.  El bit 31 ocupa el indice 0.
 * ============================================================ */
static char progmem[MAXPROGRAMLEN][INSTSIZE + 1];
static char instrucc[INSTSIZE + 1];

/* ============================================================
 *  Tablas de simbolos y referencias pendientes de resolver
 * ============================================================ */
struct SymbRef {
    char Symbol[MAXSYMBOLLEN + 1];
    int  LineRef;  /* linea fuente donde aparece la referencia  */
    int  PosRef;   /* indice en progmem de la instruccion       */
    int  BitMSB;   /* bit mas significativo del campo (notac. ISA) */
    int  Size;     /* numero de bits del campo                  */
};

struct SymbEnt {
    char Symbol[MAXSYMBOLLEN + 1];
    int  Value;
    int  LineDef;
};

static struct SymbRef tablaR[MAXSYMBREFS];
static int numRefs = 0;

static struct SymbEnt tablaS[MAXSYMBOLS];
static int numSymb = 0;

/* ============================================================
 *  Utilidades de conversion y acceso a tablas
 * ============================================================ */

/*
 * convBin: escribe 'numchars' bits de 'number' en 'destStr'.
 * MSB queda en destStr[0]. No agrega '\0'.
 */
static void convBin(unsigned int number, char* destStr, int numchars) {
    for (int i = numchars - 1; i >= 0; i--) {
        destStr[i] = (char)('0' + (number & 1u));
        number >>= 1;
    }
}

/*
 * writeField: escribe 'size' bits con MSB en el bit 'msb' del ISA
 * (bit 31 = mas significativo) sobre el buffer global 'instrucc'.
 */
static void writeField(int msb, int size, unsigned int value) {
    int startIdx = INSTSIZE - 1 - msb; /* bit 31 -> indice 0 */
    convBin(value, instrucc + startIdx, size);
}

/*
 * writeFieldInMem: igual que writeField pero sobre progmem[pos].
 * Se usa al resolver referencias forward.
 */
static void writeFieldInMem(int pos, int msb, int size, unsigned int value) {
    int startIdx = INSTSIZE - 1 - msb;
    convBin(value, progmem[pos] + startIdx, size);
}

static int findMnemonic(const char* str) {
    for (int i = 0; i < NUMINSTR; i++)
        if (strcmp(instrTable[i].mnemonic, str) == 0)
            return i;
    return -1;
}

static void addSymbRef(const char* sym, int line, int pos, int msb, int size) {
    if (numRefs >= MAXSYMBREFS) {
        printf("ERROR: tabla de referencias llena al anadir '%s'\n", sym);
        return;
    }
    strncpy(tablaR[numRefs].Symbol, sym, MAXSYMBOLLEN);
    tablaR[numRefs].Symbol[MAXSYMBOLLEN] = '\0';
    tablaR[numRefs].LineRef = line;
    tablaR[numRefs].PosRef  = pos;
    tablaR[numRefs].BitMSB  = msb;
    tablaR[numRefs].Size    = size;
    numRefs++;
}

static void addSymbol(const char* sym, int value, int srcline) {
    if (numSymb >= MAXSYMBOLS) {
        printf("ERROR: tabla de simbolos llena al anadir '%s'\n", sym);
        return;
    }
    strncpy(tablaS[numSymb].Symbol, sym, MAXSYMBOLLEN);
    tablaS[numSymb].Symbol[MAXSYMBOLLEN] = '\0';
    tablaS[numSymb].Value   = value;
    tablaS[numSymb].LineDef = srcline;
    numSymb++;
}

static int getSymbolIdx(int lineadef) {
    for (int i = 0; i < numSymb; i++)
        if (tablaS[i].LineDef == lineadef)
            return i;
    return -1;
}

static int getSymbolValue(const char* sym) {
    for (int i = 0; i < numSymb; i++)
        if (strcmp(tablaS[i].Symbol, sym) == 0)
            return tablaS[i].Value;
    return -1;
}

static void setSymbolValue(int idx, int value) {
    tablaS[idx].Value = value;
}

/* ============================================================
 *  Funciones de lectura del fichero fuente
 * ============================================================ */

/*
 * eatComment: consume desde el caracter actual hasta fin de linea.
 * Precondicion: el ultimo caracter leido era ';'.
 * Devuelve 0 si llego a EOF, 1 en caso contrario.
 */
static int eatComment(FILE* f) {
    int c;
    while ((c = fgetc(f)) != EOF && c != '\n')
        ;
    return (c == EOF) ? 0 : 1;
}

/*
 * eatWhitespaceAndComments: avanza hasta el primer caracter util
 * (no espacio, no comentario). Actualiza *linecount.
 * Devuelve 1 si hay contenido, 0 si EOF.
 */
static int eatWhitespaceAndComments(FILE* f, int* linecount) {
    int c;
    for (;;) {
        c = fgetc(f);
        if (c == EOF)  return 0;
        if (c == '\n') { (*linecount)++; continue; }
        if (isspace(c)) continue;
        if (c == ';')  {
            if (!eatComment(f)) return 0;
            (*linecount)++;
            continue;
        }
        ungetc(c, f);
        return 1;
    }
}

/*
 * readReg: lee un registro Rn (case-insensitive).
 * Salta espacios y coma previa.
 * Devuelve 1 si OK, 0 si error.
 */
static int readReg(FILE* f, int* reg, int srcline) {
    int c;
    do { c = fgetc(f); } while (c == ' ' || c == '\t' || c == ',');
    if (c == EOF || (c != 'r' && c != 'R')) {
        if (c != EOF) ungetc(c, f);
        printf("ERROR linea %d: se esperaba registro (Rn)\n", srcline);
        return 0;
    }
    if (fscanf(f, "%d", reg) != 1 || *reg < 0 || *reg > 15) {
        printf("ERROR linea %d: numero de registro invalido\n", srcline);
        return 0;
    }
    return 1;
}

/*
 * readImmOrSym: lee un inmediato entero (dec/hex/oct) o un simbolo.
 * Si el simbolo no esta definido aun se registra como referencia forward.
 * msb/size describen el campo destino dentro de la instruccion.
 * Devuelve 1 si OK, 0 si error.
 */
static int readImmOrSym(FILE* f, int* val, int srcline,
                        int instrPos, int msb, int size) {
    int c;
    do { c = fgetc(f); } while (c == ' ' || c == '\t' || c == ',');
    if (c == EOF) {
        printf("ERROR linea %d: se esperaba inmediato o simbolo\n", srcline);
        return 0;
    }
    ungetc(c, f);

    long posfile = ftell(f);

    /* Intentar como numero (admite signo, hex 0x, octal 0) */
    if (fscanf(f, " %i", val) == 1)
        return 1;

    /* Intentar como simbolo */
    fseek(f, posfile, SEEK_SET);
    char sym[MAXSYMBOLLEN + 1];
    if (fscanf(f, " %50[^ ,\n\t;()]", sym) == 1) {
        int sv = getSymbolValue(sym);
        if (sv != -1) {
            *val = sv;
        } else {
            addSymbRef(sym, srcline, instrPos, msb, size);
            *val = 0;
        }
        return 1;
    }

    printf("ERROR linea %d: se esperaba inmediato o simbolo\n", srcline);
    return 0;
}

/* ============================================================
 *  Codificacion de instrucciones
 * ============================================================ */

/*
 * processMnemonic: dado el token (en minusculas) ya leido, codifica
 * la instruccion en 'instrucc' y lee sus operandos del fichero.
 * counter = direccion (indice en progmem) de esta instruccion.
 * *code = true si se emite codigo maquina.
 */
static void processMnemonic(FILE* f, const char* token, bool* code,
                             int srcline, int counter) {

    /* --- Pseudo-instruccion EQU --- */
    if (strcmp(token, "equ") == 0) {
        *code = false;
        int cte;
        if (fscanf(f, " %i", &cte) != 1) {
            printf("ERROR linea %d: operando invalido para 'equ'\n", srcline);
            return;
        }
        int idx = getSymbolIdx(srcline);
        if (idx < 0)
            printf("AVISO linea %d: 'equ' sin etiqueta previa\n", srcline);
        else
            setSymbolValue(idx, cte);
        return;
    }

    /* --- Buscar en tabla --- */
    int id = findMnemonic(token);
    if (id < 0) {
        printf("ERROR linea %d: mnemónico no reconocido '%s'\n", srcline, token);
        *code = false;
        return;
    }

    *code = true;

    /* Inicializar la instruccion a ceros */
    memset(instrucc, '0', INSTSIZE);
    instrucc[INSTSIZE] = '\0';

    const InstrDef* instr = &instrTable[id];

    /* Escribir los 8 bits superiores [31:24] */
    memcpy(instrucc, instr->opcode8, 8);

    /* HALT: todos los bits a '1' */
    if (instr->type == T_NOOPER && strcmp(instr->mnemonic, "halt") == 0) {
        memset(instrucc, '1', INSTSIZE);
        instrucc[INSTSIZE] = '\0';
        return;
    }

    /* --- Leer y codificar operandos --- */
    int rd, r1, r2, imm, c;

    switch (instr->type) {

    /*
     * ALU registro: mnem RD, R2, R1
     * [31:24]=opcode8(incluye Op)  [23:20]=RD  [19:16]=R2  [15:12]=R1  [11:0]=0
     */
    case T_ALU_REG:
        if (!readReg(f, &rd, srcline)) return;
        if (!readReg(f, &r2, srcline)) return;
        if (!readReg(f, &r1, srcline)) return;
        writeField(23, REGSIZE, (unsigned)rd);
        writeField(19, REGSIZE, (unsigned)r2);
        writeField(15, REGSIZE, (unsigned)r1);
        break;

    /*
     * ALU inmediato: mnem R2, R1, imm
     * [31:24]=opcode8(incluye Op)  [23:20]=R2  [19:16]=R1  [15:0]=imm(16b)
     */
    case T_ALU_IMM:
        if (!readReg(f, &r2, srcline)) return;
        if (!readReg(f, &r1, srcline)) return;
        if (!readImmOrSym(f, &imm, srcline, counter, 15, IMMSIZE16)) return;
        writeField(23, REGSIZE,   (unsigned)r2);
        writeField(19, REGSIZE,   (unsigned)r1);
        writeField(15, IMMSIZE16, (unsigned)(imm & 0xFFFF));
        break;

    /*
     * Memoria: lw/sw R2, offset(R1)
     * [31:24]=opcode8  [23:20]=R2  [19:16]=R1  [15:0]=offset(16b)
     */
    case T_MEM:
        if (!readReg(f, &r2, srcline)) return;
        if (!readImmOrSym(f, &imm, srcline, counter, 15, IMMSIZE16)) return;
        /* consumir '(' */
        do { c = fgetc(f); } while (c == ' ' || c == '\t');
        if (c != '(') {
            printf("ERROR linea %d: se esperaba '(' en %s\n",
                   srcline, instr->mnemonic);
            return;
        }
        if (!readReg(f, &r1, srcline)) return;
        /* consumir ')' */
        do { c = fgetc(f); } while (c == ' ' || c == '\t');
        if (c != ')') {
            printf("ERROR linea %d: se esperaba ')' en %s\n",
                   srcline, instr->mnemonic);
            return;
        }
        writeField(23, REGSIZE,   (unsigned)r2);
        writeField(19, REGSIZE,   (unsigned)r1);
        writeField(15, IMMSIZE16, (unsigned)(imm & 0xFFFF));
        break;

    /*
     * LI: li R2, imm
     * [31:24]=opcode8  [23:20]=R2  [19:16]=0  [15:0]=imm(16b)
     */
    case T_LI:
        if (!readReg(f, &r2, srcline)) return;
        if (!readImmOrSym(f, &imm, srcline, counter, 15, IMMSIZE16)) return;
        writeField(23, REGSIZE,   (unsigned)r2);
        writeField(15, IMMSIZE16, (unsigned)(imm & 0xFFFF));
        break;

    /*
     * Saltos y CALL: mnem dest
     * [31:24]=opcode8  [23:16]=0  [15:6]=dest(10b)  [5:0]=0
     */
    case T_BRANCH:
        if (!readImmOrSym(f, &imm, srcline, counter, 15, DESTDIRSIZE)) return;
        writeField(15, DESTDIRSIZE, (unsigned)(imm & 0x3FF));
        break;

    /*
     * Sin operandos: ret, nop (halt ya fue tratado antes)
     * El opcode8 ya fue copiado; el resto queda a '0'.
     */
    case T_NOOPER:
        break;
    }
}

/* ============================================================
 *  Procesado de una linea del fichero fuente
 *
 *  Devuelve:
 *    2 -> linea procesada con normalidad (termino con '\n')
 *    1 -> EOF con instruccion posiblemente procesada
 *    0 -> EOF sin nada util
 * ============================================================ */
static int processLine(FILE* f, bool* code, int* currentline, int counter) {
    char  token[MAXLINE + 1];
    char* pt        = token;
    int   numread   = 0;
    bool  isSymbol  = false;
    bool  isMnemonic = false;

    for (;;) {
        int c = fgetc(f);

        /* EOF */
        if (c == EOF) {
            if (isMnemonic) {
                *pt = '\0';
                processMnemonic(f, token, code, *currentline, counter);
                return 1;
            }
            return 0;
        }

        /* Comentario */
        if (c == ';') {
            if (isMnemonic) {
                *pt = '\0';
                processMnemonic(f, token, code, *currentline, counter);
            }
            eatComment(f);
            return 2;
        }

        /* Fin de etiqueta */
        if (c == ':' && !isSymbol) {
            if (numread > 0) {
                *pt = '\0';
                addSymbol(token, counter, *currentline);
            } else {
                printf("AVISO linea %d: etiqueta vacia\n", *currentline);
            }
            isSymbol  = true;
            isMnemonic = false;
            pt = token;
            numread = 0;
            continue;
        }

        /* Fin de linea */
        if (c == '\n') {
            if (isMnemonic) {
                *pt = '\0';
                processMnemonic(f, token, code, *currentline, counter);
            }
            return 2;
        }

        /* Espacio: puede ser separador entre etiqueta y mnemonico,
           o separador entre mnemonico y operandos */
        if (isspace(c)) {
            if (isMnemonic) {
                /* Fin del token de mnemonico: procesarlo.
                   processMnemonic consume sus propios operandos,
                   luego buscamos el fin de linea. */
                *pt = '\0';
                processMnemonic(f, token, code, *currentline, counter);
                isMnemonic = false;
                int c2;
                do {
                    c2 = fgetc(f);
                    if (c2 == EOF)  return 1;
                    if (c2 == ';') { eatComment(f); return 2; }
                    if (c2 == '\n') return 2;
                } while (1);
            }
            continue;
        }

        /* Caracter valido: forma parte del token actual */
        if (!isMnemonic) isMnemonic = true;
        *pt++ = (char)tolower((unsigned char)c);
        numread++;

        if (numread > MAXLINE) {
            printf("ERROR linea %d: token demasiado largo\n", *currentline);
            exit(1);
        }
    }
}

/* ============================================================
 *  Funcion principal de ensamblado
 * ============================================================ */
static void ensambla(const char* srcfilename, const char* dstfilename) {

    FILE* infile = fopen(srcfilename, "rb");
    if (!infile) {
        printf("ERROR: no se puede abrir '%s'\n", srcfilename);
        exit(1);
    }

    /* Inicializar memoria de programa: cada posicion = "00...0\0" */
    for (int i = 0; i < MAXPROGRAMLEN; i++) {
        memset(progmem[i], '0', INSTSIZE);
        progmem[i][INSTSIZE] = '\0';
    }

    numRefs = 0;
    numSymb = 0;

    int counter     = 0;
    int currentline = 1;

    /* ---- Pasada unica: generar codigo y registrar referencias ---- */
    for (;;) {
        if (!eatWhitespaceAndComments(infile, &currentline))
            break;

        memset(instrucc, '0', INSTSIZE);
        instrucc[INSTSIZE] = '\0';

        bool codEmitido = false;
        int res = processLine(infile, &codEmitido, &currentline, counter);

        if (codEmitido && counter < MAXPROGRAMLEN) {
            memcpy(progmem[counter], instrucc, INSTSIZE + 1);
            counter++;
        }

        if (res == 2)
            currentline++;
        else
            break;
    }
    fclose(infile);

    /* ---- Resolucion de referencias forward ---- */
    for (int i = 0; i < numRefs; i++) {
        int value = getSymbolValue(tablaR[i].Symbol);
        if (value == -1) {
            printf("ERROR: simbolo '%s' (linea %d) no resuelto\n",
                   tablaR[i].Symbol, tablaR[i].LineRef);
            continue;
        }
        writeFieldInMem(tablaR[i].PosRef, tablaR[i].BitMSB,
                        tablaR[i].Size,   (unsigned)value);
    }

    /* ---- Escritura del fichero de salida ---- */
    FILE* outfile = fopen(dstfilename, "w");
    if (!outfile) {
        printf("ERROR: no se puede crear '%s'\n", dstfilename);
        exit(1);
    }
    for (int i = 0; i < counter; i++)
        fprintf(outfile, "%s\n", progmem[i]);
    fclose(outfile);

    printf("Ensamblado OK: %d instrucciones -> '%s'\n", counter, dstfilename);
}

/* ============================================================
 *  main
 * ============================================================ */
int main(int argc, char* argv[]) {
    const char* inputFile  = "test.asm";
    const char* outputFile = "test.mem";

    if (argc == 2) {
        inputFile = argv[1];
    } else if (argc == 3) {
        inputFile  = argv[1];
        outputFile = argv[2];
    } else if (argc > 3) {
        printf("Uso: %s [entrada.asm] [salida.mem]\n", argv[0]);
        return EXIT_FAILURE;
    }

    ensambla(inputFile, outputFile);
    return 0;
}
