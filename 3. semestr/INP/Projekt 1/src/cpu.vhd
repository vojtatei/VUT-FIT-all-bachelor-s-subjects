-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2023 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): jmeno <login AT stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
  
  -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
  --   - nelze z vice procesu ovladat stejny signal,
  --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
  --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
  --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.

  type fsm_state is (
    state_start,

    state_findSign,
    

    state_fetch0,
    state_fetch1,
    state_decode,

    state_halt,
    state_nop,

    state_value_inc0,
    state_value_inc1,
    state_value_dec0,
    state_value_dec1,

    state_print0,
    state_print1,

    state_while_start,
    state_while_find_start,
    state_while_end,
    state_while_check,
    state_while_find_end,

    state_read0,
    state_read1,

    state_right_move,
    state_left_move

  );

  signal pstate : fsm_state;
  signal nstate: fsm_state;


  signal pc : std_logic_vector(12 downto 0);
  signal pc_inc : std_logic;
  signal pc_dec : std_logic;


  signal ptr : std_logic_vector(12 downto 0);
  signal ptr_inc : std_logic;
  signal ptr_dec :std_logic;

  signal mux_sel1 : std_logic;

  signal mux_sel2 : std_logic_vector(1 downto 0);


  begin

    pc_reg : process (CLK, RESET)
    begin
        if (RESET = '1') then
          pc <= (others => '0');
        elsif (rising_edge(CLK)) then
          if (pc_inc = '1') then
            pc <= pc + 1;
          elsif (pc_dec = '1') then
            pc <= pc - 1;
          end if;
        end if;
    end process;


    ptr_reg : process (CLK, RESET)
    begin
      if (RESET = '1') then
        ptr <= (others => '0');
      elsif (rising_edge(CLK)) then
        if (ptr_inc = '1') then
             ptr <= ptr + 1;
        elsif (ptr_dec = '1') then
             ptr <= ptr - 1;
        end if;
      end if;
    end process;

    mux1 : process (mux_sel1, pc, ptr)
    begin
      if (mux_sel1 = '0') then
        DATA_ADDR <= ptr;
      else
        DATA_ADDR <= pc;
      end if;
    end process;

    mux2 : process (mux_sel2, DATA_RDATA, IN_DATA)
    begin
      case mux_sel2 is
        when "00" => DATA_WDATA <= DATA_RDATA;
        when "01" => DATA_WDATA <= DATA_RDATA -1;
        when "10" => DATA_WDATA <= DATA_RDATA + 1;
        when "11" => DATA_WDATA <= IN_DATA;
        when others => 

      end case;
    end process;


    pstate_process : process (RESET,CLK,EN)
    begin
      if RESET = '1' then
        pstate <= state_start;
      elsif (rising_edge(CLK)) then
        if EN = '1' then
          pstate <= nstate;
        end if;
      end if;
    end process;


    nstate_process : process (pstate, IN_VLD, OUT_BUSY, DATA_RDATA)
    begin

      

      case pstate is

            when state_start =>

              --DATA_WDATA <= (others => '0');
              READY <= '0';
              DONE <= '0';
              IN_REQ <= '0';
              OUT_WE <= '0';
              DATA_RDWR <= '0';
              OUT_DATA <= DATA_RDATA;
              DATA_EN <= '1';
              mux_sel1 <= '0';
              mux_sel2   <= "11";
              ptr_inc <= '0';
              ptr_dec <= '0';
              pc_inc <= '0';
              pc_dec <= '0';
              nstate <= state_findSign;

            when state_findSign =>
                mux_sel1 <= '0';

                if DATA_RDATA = x"40" then
                  READY <= '1';
                  ptr_inc <= '0';
                  mux_sel1 <= '1';
                  --pc_inc <= '1';
                  nstate <= state_fetch0;
                else
                  ptr_inc <= '1';
                end if;

            when state_fetch0 =>
                  pc_inc <= '0';
                  ptr_inc <= '0';
                  pc_dec <= '0';
                  OUT_WE <= '0';
                  
                  
                  nstate <= state_fetch1;

            when state_fetch1 =>

                  DATA_EN <= '1';
                  DATA_RDWR <= '0';
                  mux_sel1 <= '1';
                  nstate <= state_decode;

            when state_decode =>
                case DATA_RDATA is
                  when X"00" => nstate <= state_halt;
                  when X"2B" => nstate <= state_value_inc0;
                  when X"2D" => nstate <= state_value_dec0;
                  when X"3E" => nstate <= state_right_move;
                  when X"3C" => nstate <= state_left_move;
                  when X"2E" => nstate <= state_print0;
                  when X"2C" => nstate <= state_read0;
                  when X"5B" => nstate <= state_while_start;
                  when X"5D" => nstate <= state_while_end;
                  when others => nstate <= state_nop;
                end case;

            when state_halt =>
                  nstate <= state_halt;
                  DONE <= '1';

            when state_nop =>
                  pc_inc <= '1';
                  nstate <= state_fetch0;

            when state_value_inc0 =>
                  DATA_EN <= '1';
                  DATA_RDWR <= '0';
                  mux_sel1 <= '0';
                  mux_sel2 <= "10";
                  pc_inc <= '1';
                  nstate <= state_value_inc1;

            when state_value_inc1 =>
                  DATA_RDWR <= '1';
                  DATA_EN <= '1';
                  pc_inc <= '0';
                  nstate <= state_fetch0;

            when state_value_dec0 =>
                  DATA_EN <= '1';
                  DATA_RDWR <= '0';
                  pc_inc <= '1';
                  mux_sel1 <= '0';
                  mux_sel2 <= "01";
                  nstate <= state_value_dec1;

            when state_value_dec1 =>
                  DATA_RDWR <= '1';
                  DATA_EN <= '1';
                  pc_inc <= '0';
                  nstate <= state_fetch0;

            when state_left_move =>
                  ptr_dec <= '1';
                  pc_inc <= '1';
                  nstate <= state_fetch0;

            when state_right_move =>
                  ptr_inc <= '1';
                  pc_inc <= '1';
                  nstate <= state_fetch0;


                  
            when state_print0 =>
                  DATA_RDWR <= '0';
                  mux_sel1 <= '0';
                  DATA_EN <= '1';
                  pc_inc <= '0';     
                  nstate <= state_print1;

            when state_print1 =>
                  
                if OUT_BUSY = '0' then
                  OUT_WE <= '1';
                  pc_inc <= '1';
                  OUT_DATA <= DATA_RDATA;
                  nstate <= state_fetch0;
                else
                  nstate <= state_print0;
                end if;

            when state_read0 =>
                DATA_EN <= '1';
                DATA_RDWR <= '0';
                pc_inc <= '0';
                mux_sel1 <= '0';
                nstate <= state_read1;
            
            when state_read1 =>
              IN_REQ <= '1';
                  if IN_VLD = '1' then                  
                    mux_sel2 <= "11";
                    DATA_RDWR <= '1';
                    DATA_EN <= '1';
                    pc_inc <= '1';
                    IN_REQ <= '0';
                    nstate <= state_fetch0;
                  else
                  
                    nstate <= state_read0;
                  end if;

   

              when state_while_start =>
                  -- Předpokládáme, že jsme narazili na '['
                  pc_dec <= '0';
                  mux_sel1 <= '0';
                  pc_inc <= '1'; -- Posuneme program counter o jeden dopředu
                  nstate <= state_while_check;
        
              when state_while_check =>
                  if DATA_RDATA = "00000000" then
                      -- Hodnota je nula, přeskočíme tělo while cyklu
                      nstate <= state_while_find_end; -- Přechod do stavu pro hledání odpovídající ']'
                      pc_inc <= '0';
                  else
                      -- Hodnota není nula, pokračujeme v těle while cyklu
                      nstate <= state_fetch0; -- Zpracování další instrukce v těle while cyklu
                  end if;
              
              when state_while_find_end =>
                  pc_inc <= '1';
                  -- Kontrola, zda aktuální znak je ']'
                  if DATA_RDATA = X"5D" then
                      nstate <= state_fetch0; -- Pokračujeme za while cyklem
                  else
                      nstate <= state_while_find_end; -- Pokračujeme v hledání
                  end if;




              when state_while_end =>
                      -- Pokud je hodnota na aktuální adrese ukazatele nenulová, vrátíme se na začátek while cyklu
                  if DATA_RDATA /= "00000000" then
                          -- Hledáme odpovídající '['
                      nstate <= state_while_find_start;

                  else
                          -- Pokud je hodnota nulová, pokračujeme za while cyklem
                      pc_inc <= '1';
                      nstate <= state_fetch0;
                  end if;
              
              when state_while_find_start =>
                  pc_dec <= '1'; -- Posuneme program counter o jeden zpět
                  if DATA_RDATA = X"5B" then
                      -- Našli jsme odpovídající '[', vrátíme se na začátek while cyklu
                      --pc_dec <= '0';
                      nstate <= state_while_start;
                  else
                      -- Pokračujeme v hledání odpovídající '['
                     -- pc_dec <= '1';
                      nstate <= state_while_find_start;
                  end if;



        end case;
            

    end process;




      
  

      
      
end behavioral;



