library verilog;
use verilog.vl_types.all;
entity uc is
    port(
        opcode          : in     vl_logic_vector(7 downto 0);
        z               : in     vl_logic;
        c               : in     vl_logic;
        n               : in     vl_logic;
        s_inc           : out    vl_logic;
        wd              : out    vl_logic;
        we3             : out    vl_logic;
        wf              : out    vl_logic;
        m_wr            : out    vl_logic;
        r_inm           : out    vl_logic;
        push            : out    vl_logic;
        pop             : out    vl_logic;
        op_alu          : out    vl_logic_vector(3 downto 0)
    );
end uc;
