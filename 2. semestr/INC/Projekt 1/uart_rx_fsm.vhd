-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Vojtěch Teichmann (xteich02)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
       CLK : in std_logic;
       RST : in std_logic;
       DIN : in std_logic;
       CNT : in std_logic_vector(4 downto 0);
       CNT2 : in std_logic_vector(3 downto 0);
       RX_EN : out std_logic;
       CNT_EN : out std_logic;
       DOUT_VLD : out std_logic
    );
end entity;



architecture behavioral of UART_RX_FSM is

type STATE_TYPE is (WAIT_START_BIT, WAIT_FIRST_BIT, RECEIVE_DATA, WAIT_STOP_BIT, DATA_VALID);
signal state : STATE_TYPE := WAIT_START_BIT;

begin

RX_EN <= '1' when state = RECEIVE_DATA else '0';
CNT_EN <= '1' when state = WAIT_FIRST_BIT  or state = RECEIVE_DATA or state = WAIT_STOP_BIT else '0';
DOUT_VLD <= '1' when state = DATA_VALID else '0';


    curent_state_logic : process(CLK,RST) begin
        if rising_edge(CLK) then
            if RST = '1' then
                state <= WAIT_START_BIT;
            else
                case state is
                    when WAIT_START_BIT => if DIN = '0' then
                        state <= WAIT_FIRST_BIT;
                    end if;
                    when WAIT_FIRST_BIT => if CNT = "10000" then
                        state <= RECEIVE_DATA;
                    end if;
                    when RECEIVE_DATA =>  if CNT2 = "1000" then
                      state <= WAIT_STOP_BIT;
                    end if;

                    when WAIT_STOP_BIT => if DIN = '1' then
                      state <= DATA_VALID;
                    end if;
                    when DATA_VALID =>
                        state <= WAIT_START_BIT;
                when others => state <= WAIT_START_BIT;
                end case;
            end if;

        end if;
    end process curent_state_logic;

end architecture;
