library verilog;
use verilog.vl_types.all;
entity cd is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        r_inm           : in     vl_logic;
        s_inc           : in     vl_logic;
        we3             : in     vl_logic;
        wef             : in     vl_logic;
        push            : in     vl_logic;
        pop             : in     vl_logic;
        m_wr            : in     vl_logic;
        s_wd            : in     vl_logic;
        op_alu          : in     vl_logic_vector(3 downto 0);
        z               : out    vl_logic;
        c               : out    vl_logic;
        n               : out    vl_logic;
        opcode          : out    vl_logic_vector(7 downto 0);
        pc_salida       : out    vl_logic_vector(9 downto 0)
    );
end cd;
