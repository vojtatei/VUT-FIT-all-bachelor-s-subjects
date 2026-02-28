-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Vojtěch Teichmann (xteich02)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
signal cnt : std_logic_vector(4 downto 0);
signal cnt2 : std_logic_vector(3 downto 0);
signal rx_en : std_logic;
signal cnt_en : std_logic;
signal DVLD : std_logic;

begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        CNT => cnt,
        CNT2 => cnt2,
        RX_EN => rx_en,
        CNT_EN => cnt_en,
        DOUT_VLD => DVLD
    );
    DOUT_VLD <= DVLD;

    pcnt: process(CLK,RST, cnt_en, rx_en) begin
        if (RST = '1' or cnt_en = '0') then --Pokud je RST 1 nebo cnt_en 0, vynuluji počítadlo
            cnt <= "00000";
        elsif rising_edge(CLK) then
            if (cnt(4) = '1') and (rx_en = '1') then --Pokud je nejvyšší bit počítadla roven 1 a signal rx_en roven 1, nastavím počítadlo na hodnotu 00001
                    cnt <= "00001";
            else
                    cnt <= cnt + 1; --V opačném případě přičtu k hodnotě počítadla 1
            end if;
        end if;
    end process pcnt;

    pcnt2: process(CLK,RST,rx_en) begin
        if (RST = '1' or rx_en = '0') then -- Pokud je signál RST roven 1 nebo signál rx_en roven 0, vynuluji druhé počítadlo
            cnt2 <= "0000";
        elsif rising_edge(CLK) then
            if (cnt(4) = '1') and (rx_en = '1') then
                    cnt2 <= cnt2 + 1; -- Pokud je nejvyšší bit počítadla roven 1 a signal rx_en roven 1, tak přičtu k počítadlu 2 + 1
                end if;
        end if;
    end process pcnt2;

    mul_reg : process (CLK, RST, cnt2, rx_en, DIN) begin
        if (RST = '1') then
            DOUT <= "00000000"; --vynuluji DOUT pokud je RST 1
        elsif rising_edge(CLK) then
            if (cnt(4) = '1') and (rx_en = '1') then
                        case cnt2 is
                            when "0000" => DOUT(0) <= DIN;
                            when "0001" => DOUT(1) <= DIN;
                            when "0010" => DOUT(2) <= DIN;
                            when "0011" => DOUT(3) <= DIN;
                            when "0100" => DOUT(4) <= DIN;
                            when "0101" => DOUT(5) <= DIN;
                            when "0110" => DOUT(6) <= DIN;
                            when "0111" => DOUT(7) <= DIN;
                            when others => null;
                            end case;
                end if;
        end if;
    end process mul_reg;

end architecture;
