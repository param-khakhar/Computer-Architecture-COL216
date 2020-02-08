----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 17.10.2019 14:12:49
-- Design Name: 
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
        clk      : in std_logic
    );
end main;

architecture Behavioral of main is

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
-- Memory declarations

type State_type is (A,B,E,F);
signal state : State_type := E;
type Memory_type is array (0 to 4095) of std_logic_vector (0 to 31);
signal Memory_array : Memory_type;
	
-- File declarations

type File_type is array (0 to 31) of std_logic_vector (0 to 31);
signal File_array : File_type;
	
begin
    process(clk)
    begin
            case state is 
                when E => 
                    Memory_array(0) <= "10100100001000000000000000000010";
                    Memory_array(1) <= "00000000000000000000000000000111";
                    Memory_array(5) <= "00000000000000000000000000000110";                    
                    state <= F;
                when F =>
                    tx_data1 <= Memory_array(instr);    
                    state <= A;                 
                when A => 
                     File_array(0) <= "00000000000000000000000000000011";
                     File_array(1) <= "00000000000000000000000000000010";          
                     op_code <= tx_data1(0 to 5);
                     rs <= tx_data1(6 to 10);
                     rt <= tx_data1(11 to 15);
                     rd <= tx_data1(16 to 20);
                     shamt <= tx_data1(21 to 25);
                     func <= tx_data1(26 to 31);
                     const <= tx_data1(16 to 31);
                     instr <= instr + 1;
                     state <= B;            
            when B =>             
                case op_code is
                    when "100011" =>
                        --lw
                        File_array(to_integer(unsigned(rs))) <= Memory_array(to_integer(unsigned(std_logic_vector(to_unsigned(to_integer(unsigned(const)) + to_integer(unsigned(File_array(to_integer(unsigned(rt))))), rd_addr2'length)))));
                    when "101001" =>
                        --sw
                        Memory_array(to_integer(unsigned(std_logic_vector(to_unsigned(to_integer(unsigned(const)) + to_integer(unsigned(File_array(to_integer(unsigned(rt))))), rd_addr2'length))))) <= File_array(to_integer(unsigned(rs)));
                    when "000000" =>                    
                        case func is 
                            when "100000" =>
                                --add
                                File_array(to_integer(unsigned(rd))) <= File_array(to_integer(unsigned(rs))) + File_array(to_integer(unsigned(rt)));             
                            when "100010" =>
                                --sub
                                File_array(to_integer(unsigned(rd))) <= File_array(to_integer(unsigned(rs))) - File_array(to_integer(unsigned(rt)));
                            when "000000" =>
                                --sll
                                File_array(to_integer(unsigned(rd))) <=  std_logic_vector(shift_left(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));
                           when "000010" =>
                                --srl
                                File_array(to_integer(unsigned(rd))) <=  std_logic_vector(shift_right(signed(File_array(to_integer(unsigned(shamt)))), to_integer(signed(File_array(to_integer(unsigned(rt)))))));                    
                            when others =>
                
                           end case;
                        when others =>
                        
                       end case;                                    
                    state <= A;
            end case;
    end process;
end Behavioral;
















