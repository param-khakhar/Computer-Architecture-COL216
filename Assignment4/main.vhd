----------------------------------------------------------------------------------
-- Company: COL216
-- Engineer: Manav Modi, Param Khakhar
-- 
-- Create Date: 17.10.2019 14:12:49
-- Design Name: Assignment 3
-- Module Name: main - Behavioral
-- Project Name: Processor in VHDL executing asm commands. 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;
use ieee.std_logic_arith;
use ieee.std_logic_signed.all;

package pkg is
    type File_type is array (0 to 31) of std_logic_vector (0 to 31);
end package;

library work;
use work.pkg.all;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity main is
    Port(
        clk      : in std_logic;
        switch  : in std_logic;
        seg: out std_logic_vector(6 downto 0);
        check : out std_logic;
        an : out std_logic_vector(3 downto 0)
    );
end main;

architecture Behavioral of main is
signal checkcontrol: std_logic := '0';
Signal seg0: std_logic_vector(6 downto 0);
Signal seg1: std_logic_vector(6 downto 0);
Signal seg2: std_logic_vector(6 downto 0);
Signal seg3: std_logic_vector(6 downto 0);
Signal tx_data1 : std_logic_vector(0 to 31);-- := "00000001000010010101000000100000";
Signal op_code: std_logic_vector(0 to 5) ;--:= "000000";
Signal func: std_logic_vector(0 to 5) ;--:= "000000";
Signal rs: std_logic_vector(0 to 4) := "00000";
Signal rt: std_logic_vector(0 to 4) := "00000";
Signal rd: std_logic_vector(0 to 4) := "00000";
Signal shamt: std_logic_vector(0 to 4);-- := "00000";
Signal const: std_logic_vector(0 to 15);-- := "0000000000000000";
Signal instr: integer := 0; 
Signal rd_addr2 : std_logic_vector(0 to 11);-- := "000000000000";
Signal offset : std_logic_vector(0 to 11) := "010000000000";
Signal clkd: std_logic:= '0';
Signal count: integer:= 1;
Signal count2: integer:= 1;
signal counter: std_logic_vector(15 downto 0):= "0000000000000000";
signal inpo: std_logic_vector(15 downto 0):= "0000000000000000";
-- Seven segment display declarations
--signal seg: std_logic_vector(6 downto 0);
signal inp0: std_logic_vector(3 downto 0) := "0000";
signal inp1: std_logic_vector(3 downto 0) := "0000";
signal inp2: std_logic_vector(3 downto 0) := "0000";
signal inp3: std_logic_vector(3 downto 0) := "0000";
signal ano : std_logic_vector(3 downto 0) := "0000";
signal final : std_logic_vector(3 downto 0) := "0000";
signal inp: std_logic_vector(31 downto 0) := "00000000000000000000000000000000";
signal tan: std_logic_vector(3 downto 0) := "0000";
signal clkslow: std_logic:= '0';

-- Memory module declarations
signal ena : std_logic := '1';
signal wea : STD_LOGIC_VECTOR(0 DOWNTO 0);
signal addra : STD_LOGIC_VECTOR(11 DOWNTO 0) := "000000000000";
signal dina : STD_LOGIC_VECTOR(31 DOWNTO 0);
signal douta : std_logic_vector(31 downto 0);
signal enb : std_logic;
signal web : std_logic_vector(0 downto 0);
signal addrb: std_logic_vector(11 downto 0);
signal dinb: std_logic_vector(31 downto 0);
signal doutb: std_logic_vector(31 downto 0);

Component blk_mem_gen_0 PORT(    
   clka : IN STD_LOGIC;
   ena : IN STD_LOGIC;
   wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
  -- addra iterates over the individual instructions of the memory
   addra : IN STD_LOGIC_VECTOR(11 DOWNTO 0);
   dina : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
   douta : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
   clkb : IN STD_LOGIC;
   enb : IN STD_LOGIC;
   web : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
   addrb : IN STD_LOGIC_VECTOR(11 DOWNTO 0);
   dinb : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
   doutb : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)); END COMPONENT;	        
    
-- Memory declarations
type State_type is (A,B,C,F,LW0,LW1,LW2,LW3,LW4,SW0,SW1,SW2,SW3,SW4);
signal state : State_type := F;

type State_type1 is (A1,B1,C1,D1);
signal state1 : State_type1 := A1;
	
type File_type is array (0 to 31) of std_logic_vector (0 to 31);
signal File_array : File_type;
	
begin

    M1: blk_mem_gen_0 PORT map(clka => clk,
                                ena => '1',
                                wea => "0" ,
                                addra => addra,
                                dina => dina,
                                douta => douta,
                                clkb => clk,
                                enb => enb,
                                web => web,
                                addrb => addrb,
                                dinb => dinb,
                                doutb => doutb);
                                
    --S7: sevenSeg PORT map(
      --                        v0 => inp0,
        --                      v1 => inp1,
          --                    v2 => inp2,
            --                  v3 => inp3,
              --                clk => clkd,
                --              z0 => cath0,
                  --            z1 => cath1,
                    --          z2 => cath2,
                      --        z3 => cath3,                  
                        --      a => ano);
    --c1000: freq1khz PORT map( 
      --                        clok => clk,
        --                      reset => '0',
          --                    red_clk => clkd   
            --                 );
    
    process(clk)
    begin
        if(clk'event and clk = '1') then 
        count <= count + 1;
        count2 <= count2 + 1;
            --if count = 50000 then
            if count = 50000 then
                clkd <= not clkd;
                count <= 1;
            end if;
            if count2 = 5000  then
                count2 <= 1;
                clkslow <= not clkslow;
            end if;
        end if;
    end process;
    
    process(clkd)
    begin
        if (clkd'event and clkd = '1') then
            case state1 is
                when A1 => tan <= "0111";
                    state1 <= b1;
                when B1 => tan <= "1011";
                    state1 <= c1;
                when C1 => tan <= "1101";
                    state1 <= d1;
                when D1 => tan <= "1110";
                    state1 <= a1;
                end case;
      end if;
    end process;
    check <= checkcontrol;    
    process(clkslow)
    begin
        if(rising_edge(clkslow)) then
        if not (douta = "000000000000000000000000000000000000") then
         --if(clk'event) then
         counter <= counter + 1;
            case state is 
                when F =>
                    --inp0 <= inp(3 downto 0);
                    --inp1 <= inp(7 downto 4);
                    --inp2 <= inp(11 downto 8);
                    --inp3 <= inp(15 downto 12);
                    File_array(0) <= "00000000000000000000000000000001";
                    File_array(1) <= "00000000000000000000000000001011";   
                    tx_data1 <= douta;
                    state <= A;  
                    --checkcontrol <= '1';               

                when A =>
                     op_code <= tx_data1(0 to 5);
                     rs <= tx_data1(6 to 10);
                     rt <= tx_data1(11 to 15);
                     rd <= tx_data1(16 to 20);
                     shamt <= tx_data1(21 to 25);
                     func <= tx_data1(26 to 31);
                     const <= tx_data1(16 to 31);
                     state <= B;
                     --checkcontrol <= '0';
            when B =>
                    
                case op_code is
                    when "100011" =>
                        --lw
                        checkcontrol <= '1';
                        state <= LW0;
                        enb <= '1';
                        web <= "0";
                        addrb <= std_logic_vector(to_unsigned(to_integer(unsigned(const)) + to_integer(unsigned(File_array(to_integer(unsigned(rs))))), rd_addr2'length));
                        
                    when "101011" =>
                        --sw
                        checkcontrol <= '1';
                        state <= SW0;
                        enb <= '1';
                        web <= "1";
                        addrb <= std_logic_vector(to_unsigned(to_integer(unsigned(const)) + to_integer(unsigned(File_array(to_integer(unsigned(rs))))), rd_addr2'length));
                          dinb <= File_array(to_integer(unsigned(rt)));
                          inp <= File_array(to_integer(unsigned(rt)));
                    when "000000" =>                    
                        case func is 
                            when "100000" =>
                                --add
                                checkcontrol <= '1';
                                state <= C;
                                inp <= File_array(to_integer(unsigned(rs))) + File_array(to_integer(unsigned(rt)));     
                                File_array(to_integer(unsigned(rd))) <= File_array(to_integer(unsigned(rs))) + File_array(to_integer(unsigned(rt)));
                                             
                            when "100010" =>
                                --sub
                                checkcontrol <= '1';
                                state <= C;
                                inp <= File_array(to_integer(unsigned(rs))) - File_array(to_integer(unsigned(rt)));
                                File_array(to_integer(unsigned(rd))) <= File_array(to_integer(unsigned(rs))) - File_array(to_integer(unsigned(rt)));
                            when "000000" =>
                                --sll
                                --checkcontrol <= '1';
                                inp <= std_logic_vector(shift_left(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));
                                state <= C;     
                                File_array(to_integer(unsigned(rd))) <=  std_logic_vector(shift_left(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));
                            when "000010" =>
                                --srl
                                checkcontrol <= '1';
                                inp <= std_logic_vector(shift_right(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));
                                state <= C;     
                                File_array(to_integer(unsigned(rd))) <=  std_logic_vector(shift_right(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));                    
                            when others =>
                                state <= C;                   
                           end case;
                        when others =>
                            state <= C;    
                       end case;
                       when LW1 => 
                            state <= LW2;
                            
                       when LW2 =>
                            tx_data1 <= douta;            
                            state <= LW3;
                            
                       when LW3 =>
                            state <= LW4;
                                 
                       when LW4 =>
                            File_array(to_integer(unsigned(rt))) <= doutb;
                            inp <= doutb;
                            state <= C;
                            
                       when SW0 => 
                            state <= SW1;
                            
                       when SW1 =>
                       
                            web <= "0";
                            state <= Sw2;
                       
                            
                       when SW2 =>
                       
                            tx_data1 <= douta;
                            state <= SW3;          
                       
                       when SW3 =>
                            state <= SW4;
                            
                       
                       when SW4 =>
--                            dinb <= File_array(to_integer(unsigned(rt)));
                            state <= C;
                       
                       when C =>
                       --inp <= File_array(to_integer(unsigned(rd)));

                            addra <= addra + 1;
                            state <= F;
                            
                       when LW0 => 
                            state <= LW1;
                            
                        when others =>                                       
            end case;
            end if;
      end if;
    end process;
    with switch select inpo <=
        inp(15 downto 0) when '1',
        counter when '0',
        "1111111111111111" when others;
    inp0 <= inpo(3 downto 0);
    inp1 <= inpo(7 downto 4);
    inp2 <= inpo(11 downto 8);
    inp3 <= inpo(15 downto 12);
            with inp0 select seg0 <=
            "0000001" when "0000",
            "1001111" when "0001",
            "0010010" when "0010",
            "0000110" when "0011",
            "1001100" when "0100",
            "0100100" when "0101",
            "0100000" when "0110",
            "0001111" when "0111",
            "0000000" when "1000",
            "0000100" when "1001",
            "0000010" when "1010",
            "1100000" when "1011",
            "0110001" when "1100",
            "1000010" when "1101",
            "0110000" when "1110",
            "0111000" when "1111",
            "1111111" when others;
            with inp1 select seg1 <=
            "0000001" when "0000",
            "1001111" when "0001",
            "0010010" when "0010",
            "0000110" when "0011",
            "1001100" when "0100",
            "0100100" when "0101",
            "0100000" when "0110",
            "0001111" when "0111",
            "0000000" when "1000",
            "0000100" when "1001",
            "0000010" when "1010",
            "1100000" when "1011",
            "0110001" when "1100",
            "1000010" when "1101",
            "0110000" when "1110",
            "0111000" when "1111",
            "1111111" when others;
            with inp2 select seg2 <=
            "0000001" when "0000",
            "1001111" when "0001",
            "0010010" when "0010",
            "0000110" when "0011",
            "1001100" when "0100",
            "0100100" when "0101",
            "0100000" when "0110",
            "0001111" when "0111",
            "0000000" when "1000",
            "0000100" when "1001",
            "0000010" when "1010",
            "1100000" when "1011",
            "0110001" when "1100",
            "1000010" when "1101",
            "0110000" when "1110",
            "0111000" when "1111",
            "1111111" when others;
            with inp3 select seg3 <=
            "0000001" when "0000",
            "1001111" when "0001",
            "0010010" when "0010",
            "0000110" when "0011",
            "1001100" when "0100",
            "0100100" when "0101",
            "0100000" when "0110",
            "0001111" when "0111",
            "0000000" when "1000",
            "0000100" when "1001",
            "0000010" when "1010",
            "1100000" when "1011",
            "0110001" when "1100",
            "1000010" when "1101",
            "0110000" when "1110",
            "0111000" when "1111",
            "1111111" when others;
            
            with tan select seg <=
                seg0 when "1110",
                seg1 when "1101",
                seg2 when "1011",
                seg3 when "0111",
               "0101010" when others;
            an <= tan;
--            with inp0 select seg <=
--            "0000001" when "0000",
--            "1001111" when "0001",
--            "0010010" when "0010",
--            "0000110" when "0011",
--            "1001100" when "0100",
--            "0100100" when "0101",
--            "0100000" when "0110",
--            "0001111" when "0111",
--            "0000000" when "1000",
--            "0000100" when "1001",
--            "0000010" when "1010",
--            "1100000" when "1011",
--            "0110001" when "1100",
--            "1000010" when "1101",
--            "0110000" when "1110",
--            "0111000" when "1111",
--            "1111111" when others;
            
end Behavioral;
















