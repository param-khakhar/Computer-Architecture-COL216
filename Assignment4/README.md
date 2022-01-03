**Assignment 4**

The assignment is an extension to the assignment 3 as the instructions are now read from the memory (i.e instead of the test-bench). A coe file is made which contains the instructions which need to be executed. A block memory module is generated from the ip catalog available in vivado and the coe file is loaded in it. A component of the same has been instantiated in the main file and instructions are read from it. A true dual port version is used in oeder to read from the memory and write to the memory. Along with this, code is written in order to display the last 16 bits of the instruction on the 7 segment display and also to display the total number of clock cycles required to execute all the instructions depending on whether the switch is on or off.


