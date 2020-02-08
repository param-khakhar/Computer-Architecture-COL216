## Mini-Processor in VHDL - Lab Assignment 3

- The VHDL implementation uses a 2D array:
    1. Memory - 4096 x 32
    2. Register File - 32 x 32

- The implementation comprises of several states and the processor is modelled as an FSM. The following steps take place:
    1. The instruction is read from the memory one at a time.
    2. The instruction is than parsed, and approapriate values are assigned to the signals such as opcode, rs, rt, rd, shamt, const etc. 
    3. Then depending on the opcode, we classify the instructions into two sets namely, add, sub, shift-left and shift-right and load-word and save-word. 
    4. The values are retrieved from the different data-containers as required, and also modified and stored at the desired locations.
    5. Optimizations are made by reducing the number of states and reducing the number of assignments in order to execute the instructions in the smallest number of clock cycles.
    

                                                
        