library verilog;
use verilog.vl_types.all;
entity memdat is
    port(
        clk             : in     vl_logic;
        m_wr            : in     vl_logic;
        a               : in     vl_logic_vector(15 downto 0);
        wd              : in     vl_logic_vector(15 downto 0);
        rd              : out    vl_logic_vector(15 downto 0)
    );
end memdat;
