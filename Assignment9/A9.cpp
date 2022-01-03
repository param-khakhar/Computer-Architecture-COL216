#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

int regmap(string s){
	
		if(s == "$zero"){
			return 0;
		}
		else if(s == "$at"){
			return 1;
		}
		else if(s == "$v0"){
			return 2;
		}
		else if(s == "$v1"){
			return 3;
		}
		else if(s == "$a0"){
			return 4;
		}
		else if(s == "$a1"){
			return 5;
		}
		else if(s == "$a2"){
			return 6;
		}
		else if(s == "$a3"){
			return 7;
		}
		else if(s == "$t0"){
			return 8;
		}
		else if(s == "$t1"){
			return 9;
		}
		else if(s == "$t2"){
			return 10;
		}
		else if(s == "$t3"){
			return 11;
		}
		else if(s == "$t4"){
			return 12;
		}
		else if(s == "$t5"){
			return 13;
		}
		else if(s == "$t6"){
			return 14;
		}
		else if(s == "$t7"){
			return 15;
		}
		else if(s == "$s0"){
			return 16;
		}
		else if(s == "$s1"){
			return 17;
		}
		else if(s == "$s2"){
			return 18;
		}
		else if(s == "$s3"){
			return 19;
		}
		else if(s == "$s4"){
			return 20;
		}
		else if(s == "$s5"){
			return 21;
		}
		else if(s == "$s6"){
			return 22;
		}
		else if(s == "$s7"){
			return 23;
		}
		else if(s == "$t8"){
			return 24;
		}
		else if(s == "$t9"){
			return 25;
		}
		else if(s == "$k0"){
			return 26;
		}
		else if(s == "$k1"){
			return 27;
		}
		else if(s == "$gp"){
			return 28;
		}
		else if(s == "$sp"){
			return 29;
		}
		else if(s == "$fp"){
			return 30;
		}
		else if(s == "$ra"){
			return 31;
		}
		else{
			return -1;
		}
}

void printFA(int array [32]){
	for(int i = 0;i<32;i++){
		 cout<<"File_array["<<i<<"]:"<<array[i]<<endl;
	}
}

void printMemory(int memory [4096]){
	for(int i = 1024;i<1030;i++){
		 cout<<"memory["<<i<<"]:"<<memory[i]<<endl;
	}	
}

/*Function checks whether to vectors v1 and v2 have all the entries as 0 or not*/
/**bool check(vector <vector<string>> v1, vector <int> v2){
	bool a,b;
	a = true;
	b = true;
	if(v1.at(0).at(0) == ""){
		a = false;
	}
	if(v2.at(0) == -1){
		cout<<v2.at(0)<<endl;
		b = false;
	}	
	return (a || b);
}**/

int ToFunc(string s){
	if(s == "ADD"){
		return 32;
	}
	else if(s == "SUB"){
		return 34;
	}
	else if(s == "SLL"){
		return 0;
	}
	else if(s == "SRL"){
		return 2;
	}
	else{
		cout<<"Invalid String"<<endl;
		return -1;
	}	
}

int ToOpcode(string s){
	if(s == "ADD"){
		return 0;
	}
	else if(s == "SUB"){
		return 0;
	}
	else if(s == "SLL"){
		return 0;
	}
	else if(s == "SRL"){
		return 0;
	}
	else if(s == "SW"){
		return 43;		
	}
	else if(s == "LW"){
		return 35;
	}
	else if(s == "BNE"){
		return 5;
	}
	else if(s == "BEQ"){
		return 4;
	}		
	else if(s == "J"){
		return 2;			
	}
	else if(s == "JR"){
		return 8;
	}
	else if(s == "JAL"){
		return 3;
	}

	else if(s == "BLEZ"){
		return 6;
	}
	else if(s == "BGTZ"){
		return 7;
	}
	else if(s == ""){
		return -1;
	}
	else{
		cout<<"Invalid OpCode"<<endl;
		return -2;
	}
}

int main(int argc, char** argv){
	ifstream infile(argv[1]);
	string line;

	/*Splittting the memory into instruction and data memory*/
	int data_memory [4096];
	int file_array [32];

	/*Initialiazing the clock (denotes the number of clock cycles executed) to be 0*/
	int clock = 0;

	/*Indicates number of instructions executed so far*/
	float numInstr = 0;

	for(int i=0;i<32;i++){
		file_array[i] = 0;
	}

	for(int i=0;i<4096;i++){
		data_memory[i] = 0;
	}
	/*File Array Initializations*/
	file_array[29] = 4095; //$sp
	
	file_array[31] = 0;
	file_array[8] = -4;
	file_array[9] = 2;
	file_array[10] = 3;
	file_array[0] = 0;
	
	/*input is the 2d vector (vector<vector<string>>) which contains each instruction stored as a vector. It is equivalent to
	the instruction memory.*/
	vector<vector<string>> input;

	/*PC stands for Program Counter*/
	int pc = 0;	
	while(getline(infile,line)){
		vector<string> regVector;

		/**The instruction is split based on the spaces and the individual strings are processed:
		 * Instructions are split in the following fashion
		 * ADD $t4 $t0 $t1 
		 * J 15
		 * SW $ra 0 $sp **/

		size_t prev = 0, pos;
    	while ((pos = line.find_first_of(" ,()", prev)) != std::string::npos){
        	if (pos > prev)
            	regVector.push_back(line.substr(prev, pos-prev));
        	prev = pos+1;
    	}
    	if (prev < line.length())
        	regVector.push_back(line.substr(prev, std::string::npos));
		input.push_back(regVector);
	}
	/*for(int i = 0;i<input.size();i++){
		for(int j = 0;j<input.at(i).size();j++){
			cout<<input.at(i).at(j)<<" ";
		}
		cout<<endl;
	}
	cout<<"A8"<<endl;*/

	/**Respective registers(implemented as vectors) to be used in the pipelined datapath between two stages. The stages are:*
	 * IF: Instruction Fetch
	 * ID: Instruction Decode
	 * EX: Execute and Address Calculation
	 * MEM: Memory Access
	 * WB: Write Back 
	 */

	int n = input.size();				//No. of Instructions

	vector<vector<string>> IfId;		// The registers separating the various pipeline stages are represented by their
										// corresponding states at their left and right.
	vector<string> temp;
	temp.push_back("");
	IfId.push_back(temp);

	vector<int> IdEx;
	IdEx.push_back(-1);

	vector<int> ExMem;
	ExMem.push_back(-1);

	vector<int> MemWb;
	MemWb.push_back(-1);


	cout<<"$t0 "<<file_array[8]<<endl;
	cout<<"$t1 "<<file_array[9]<<endl;
	cout<<"$t2 "<<file_array[10]<<endl;
	cout<<"$t3 "<<file_array[11]<<endl;
	cout<<"$t4 "<<file_array[12]<<endl;
	cout<<"$t5 "<<file_array[13]<<endl;
	cout<<"$t6 "<<file_array[14]<<endl;
	cout<<"$t7 "<<file_array[15]<<endl;
	cout<<"$sp "<<file_array[29]<<endl;

/**A do while loop construct is used for implementing the pipelined version of the processor. The condition of 
 * termination is when the program counter equals n+4 which denotes the completion of all the stages of the last instruction. 
 * The variable clock is incremented at the beginning of each iteration.*/

	do{
		/*Incrementing the clock cycle count*/
		clock++;		
		cout<<"Clock:"<<clock<<endl;

		/*Write Back. The following contains the code for writing back to the RegisterFile if required. Due to the faster read
		and write speed of the register, write back and reading can be achieved in the same clock cycle.*/

		numInstr++;
		if(MemWb.at(0) == 0){
			if(MemWb.at(3) == 32){
					file_array[MemWb.at(2)] = MemWb.at(1);
					cout<<"Write Back"<<":ADD"<<endl;
				}
			else if(MemWb.at(3) == 34){
					file_array[MemWb.at(2)] = MemWb.at(1);
					cout<<"Write Back"<<":SUB"<<endl;
				}
			else if(MemWb.at(3) == 0){
					file_array[MemWb.at(2)] = MemWb.at(1);
					cout<<"Write Back"<<":SLL"<<endl;
				}
			else if(MemWb.at(3) == 2){
					file_array[MemWb.at(2)] = MemWb.at(1);
					cout<<"Write Back"<<":SRL"<<endl;
				}
			else{
				cout<<"Invalid Function"<<endl;
			}
		}
		else if(MemWb.at(0) == 43){

			cout<<"Write Back"<<":"<<"SW"<<endl;
		}
		else if(MemWb.at(0) == 35){
			file_array[MemWb.at(2)] = MemWb.at(1);
			cout<<"Write Back"<<":"<<"LW"<<endl;											
		}
		else if(MemWb.at(0) == 5){
			//int n = MemWb.at(1);
			// if(n!=-1){
			// 	pc = n;
			// 	ExMem.clear();
			// 	ExMem.push_back(-1);
			// 	IdEx.clear();
			// 	IdEx.push_back(-1);
			// 	IfId.clear();
			// 	vector<string> temp;
			// 	temp.push_back("");
			// 	IfId.push_back(temp);
			// }
			cout<<"Write Back"<<":BNE"<<endl;
		}
		else if(MemWb.at(0) == 4){
			// int n = MemWb.at(1);
			// if(n != -1){
			// 	pc = n;
			// 	ExMem.clear();
			// 	ExMem.push_back(-1);
			// 	IdEx.clear();
			// 	IdEx.push_back(-1);
			// 	IfId.clear();
			// 	vector<string> temp;
			// 	temp.push_back("");
			// 	IfId.push_back(temp);
			// }
			cout<<"Write Back"<<":BEQ"<<endl;	
		}			
		else if(MemWb.at(0) == 2){
			//pc = MemWb.at(1);
			cout<<"Write Back"<<":J"<<endl;			
			// ExMem.clear();
			// ExMem.push_back(-1);
			// IdEx.clear();
			// IdEx.push_back(-1);
			// IfId.clear();
			// vector<string> temp;
			// temp.push_back("");
			// IfId.push_back(temp);
		}
		else if(MemWb.at(0) == 8){
			// pc = file_array[MemWb.at(1)];
			// cout<<"pc "<<pc<<endl;
			cout<<"Write Back"<<":JR"<<endl;
			// ExMem.clear();
			// ExMem.push_back(-1);
			// IdEx.clear();
			// IdEx.push_back(-1);
			// IfId.clear();
			// vector<string> temp;
			// temp.push_back("");
			// IfId.push_back(temp);
		}
		else if(MemWb.at(0) == 3){
			//pc = MemWb.at(1);
			file_array[31] = MemWb.at(3);
			cout<<"$ra "<<file_array[31]<<endl;
			cout<<"Write Back"<<":JAL"<<endl;
			// ExMem.clear();
			// ExMem.push_back(-1);
			// IdEx.clear();
			// IdEx.push_back(-1);
			// IfId.clear();
			// vector<string> temp;
			// temp.push_back("");
			// IfId.push_back(temp);
		}
		else if(MemWb.at(0) == 6){
			// int n = MemWb.at(1);
			// if(n != -1){
			// 	pc = n;
			// 	ExMem.clear();
			// 	ExMem.push_back(-1);
			// 	IdEx.clear();
			// 	IdEx.push_back(-1);
			// 	IfId.clear();
			// 	vector<string> temp;
			// 	temp.push_back("");
			// 	IfId.push_back(temp);
			// }
			cout<<"Write Back"<<":BLEZ"<<endl;			
		}
		else if(MemWb.at(0) == 7){
			// int n = MemWb.at(1);
			// if(n != -1){
			// 	pc = MemWb.at(1);
			// 	ExMem.clear();
			// 	ExMem.push_back(-1);
			// 	IdEx.clear();
			// 	IdEx.push_back(-1);
			// 	IfId.clear();
			// 	vector<string> temp;
			// 	temp.push_back("");
			// 	IfId.push_back(temp);
			// }
			cout<<"Write Back"<<":BGTZ"<<endl;	
		}
		else if(MemWb.at(0) == -1){
			//Do nothing! None of the instruction has reached this stage.
			numInstr--;
			cout<<"Write Back:NOP"<<endl;
		}
		else{
			cout<<"Invalid Opcode"<<endl;
		}
	

/**Memory Access. Any instruction requiring values stored in the memory dealt here. Others just passed on to the next stage.**/
		
		MemWb.clear();
		if(ExMem.at(0) == 0){
			if(ExMem.at(3) == 32){
					MemWb.push_back(ExMem.at(0));
					MemWb.push_back(ExMem.at(1));
					MemWb.push_back(ExMem.at(2));
					MemWb.push_back(ExMem.at(3));
					cout<<"Memory Access"<<":ADD"<<endl;
				}
			else if(ExMem.at(3) == 34){
					MemWb.push_back(ExMem.at(0));
					MemWb.push_back(ExMem.at(1));
					MemWb.push_back(ExMem.at(2));
					MemWb.push_back(ExMem.at(3));
					cout<<"Memory Access"<<":SUB"<<endl;
				}
			else if(ExMem.at(3) == 0){
					MemWb.push_back(ExMem.at(0));			
					MemWb.push_back(ExMem.at(1));
					MemWb.push_back(ExMem.at(2));
					MemWb.push_back(ExMem.at(3));					
					cout<<"Memory Access"<<":SLL"<<endl;
				}
			else if(ExMem.at(3) == 2){
					MemWb.push_back(ExMem.at(0));				
					MemWb.push_back(ExMem.at(1));
					MemWb.push_back(ExMem.at(2));
					MemWb.push_back(ExMem.at(3));			
					cout<<"Memory Access"<<":SRL"<<endl;
				}
			else{
				cout<<"Invalid Function"<<endl;
			}
		}
		else if(ExMem.at(0) == 43){
			int address = ExMem.at(2);
			int result = ExMem.at(1);
			data_memory[address] = file_array[result];			//Saving data to the data memory.
			MemWb.push_back(ExMem.at(0));						
			MemWb.push_back(result);
			MemWb.push_back(address);
			cout<<"Memory Access"<<":"<<"SW"<<endl;	
		}
		else if(ExMem.at(0) == 35){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(data_memory[ExMem.at(1)]);
			MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":LW"<<endl;								
		}
		else if(ExMem.at(0) == 5){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
		//	MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":BNE"<<endl;
		}
		else if(ExMem.at(0) == 4){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
		//	MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":BEQ"<<endl;
		}
								
		else if(ExMem.at(0) == 2){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
			MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":J"<<endl;			
		}
		else if(ExMem.at(0) == 8){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
			MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":JR"<<endl;
		}
		else if(ExMem.at(0) == 3){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
			MemWb.push_back(ExMem.at(2));
			MemWb.push_back(ExMem.at(3));
			cout<<"Memory Access"<<":JAL"<<endl;
		}
		else if(ExMem.at(0) == 6){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
		//	MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":BLEZ"<<endl;
		}
		else if(ExMem.at(0) == 7){
			MemWb.push_back(ExMem.at(0));
			MemWb.push_back(ExMem.at(1));
		//	MemWb.push_back(ExMem.at(2));
			cout<<"Memory Access"<<":BGTZ"<<endl;
		}
		else if(ExMem.at(0) == -1){
			MemWb.push_back(-1);
			cout<<"Memory Access:NOP"<<endl;
		}
		else{
			cout<<"Invalid Opcode"<<endl;
		}

		/*Execute and Memory Address Calculation*/
		/*Values are stored in the register IdEx.
		Format of the values opcode rs rt rd shamt func
							 opcode rs rd offset
							 opcode const*/


		ExMem.clear();
		if(IdEx.at(0) == 0){
			if(IdEx.at(5) == 32){
					int result = IdEx.at(1) + IdEx.at(2);
					int address = IdEx.at(3);
					ExMem.push_back(IdEx.at(0));
					ExMem.push_back(result);
					ExMem.push_back(address);
					ExMem.push_back(IdEx.at(5));
					cout<<"Execute"<<":ADD"<<endl;
				}
			else if(IdEx.at(5) == 34){
					int result = IdEx.at(1) - IdEx.at(2);
					int address = IdEx.at(3);
					ExMem.push_back(IdEx.at(0));
					ExMem.push_back(result);
					ExMem.push_back(address);
					ExMem.push_back(IdEx.at(5));
					cout<<"Execute"<<":SUB"<<endl;
				}
			else if(IdEx.at(5) == 0){
					int result = IdEx.at(1) << IdEx.at(4);
					int address = IdEx.at(3);
					ExMem.push_back(IdEx.at(0));			
					ExMem.push_back(result);
					ExMem.push_back(address);
					ExMem.push_back(IdEx.at(5));
					cout<<"Execute"<<":SLL"<<endl;
				}
			else if(IdEx.at(5) == 2){
					int result = IdEx.at(1) >> IdEx.at(4);
					int address = IdEx.at(3);
					ExMem.push_back(IdEx.at(0));				
					ExMem.push_back(result);
					ExMem.push_back(address);
					ExMem.push_back(IdEx.at(5));			
					cout<<"Execute"<<":SRL"<<endl;
				}
			else{
				cout<<"Invalid Function"<<endl;
			}
		}
		else if(IdEx.at(0) == 43){
			int address = IdEx.at(1)+IdEx.at(3);
			int result = IdEx.at(2);
			ExMem.push_back(IdEx.at(0));				
			ExMem.push_back(result);
			ExMem.push_back(address);
			cout<<"Execute"<<":"<<"SW"<<endl;	
		}
		else if(IdEx.at(0) == 35){
			ExMem.push_back(IdEx.at(0));
			int address = IdEx.at(1)+IdEx.at(3);
			int reg = IdEx.at(2);
			ExMem.push_back(address);
			ExMem.push_back(reg);
			cout<<"Execute"<<":LW"<<endl;								
		}
		else if(IdEx.at(0) == 5){
			ExMem.push_back(IdEx.at(0));
			int result;
			if(IdEx.at(1) != IdEx.at(2)){
				result = IdEx.at(3);
				pc = result;
			}
			ExMem.push_back(result);
			cout<<"Execute"<<":BNE"<<endl;
		}
		else if(IdEx.at(0) == 4){
			ExMem.push_back(IdEx.at(0));
			int result = 0;
			if(IdEx.at(1) == IdEx.at(2)){
				result = IdEx.at(3);
				pc = result;
			}
			ExMem.push_back(result);
			cout<<"Execute"<<":BEQ"<<endl;
		}
								
		else if(IdEx.at(0) == 2){
			ExMem.push_back(IdEx.at(0));
			int result = IdEx.at(1);
			pc = result;
			int address = -1;
			ExMem.push_back(result);
			ExMem.push_back(address);
			cout<<"Execute"<<":J"<<endl;			
		}
		else if(IdEx.at(0) == 8){
			ExMem.push_back(IdEx.at(0));
			int address = -1;
			int result = IdEx.at(1);
			pc = result;
			ExMem.push_back(result);
			ExMem.push_back(address);
			cout<<"Execute"<<":JR"<<endl;
		}
		else if(IdEx.at(0) == 3){
			ExMem.push_back(IdEx.at(0));
			int result = IdEx.at(1);
			int address = IdEx.at(3);
			ExMem.push_back(result);
			ExMem.push_back(IdEx.at(2));
			ExMem.push_back(address);
			pc = result;
			cout<<"Execute"<<":JAL"<<endl;
		}
		else if(IdEx.at(0) == 6){
			ExMem.push_back(IdEx.at(0));
			int result,address;
			address = -1;
			result = 0;
			if(IdEx.at(1) <= 0){
				result = IdEx.at(2);	
				pc = result;
			}
			ExMem.push_back(result);
			cout<<"Execute"<<":BLEZ"<<endl;
		}
		else if(IdEx.at(0) == 7){
			ExMem.push_back(IdEx.at(0));
			int result,address;
			address = -1;
			if(IdEx.at(1) >= 0){
				result = IdEx.at(2);
				pc = result;					
			}
			ExMem.push_back(result);
			ExMem.push_back(address);
			cout<<"Execute"<<":BGTZ"<<endl;
		}
		else if(IdEx.at(0) == -1){
			ExMem.push_back(-1);
			cout<<"Execute:NOP"<<endl;
		}
		else{
			cout<<"Invalid Opcode"<<endl;
		}

		/*Instruction Decode and Register File Read
		  Func and opcode are used interchangeably.*/

		IdEx.clear();

		if(IfId.size()>1){

				vector<string> regVector = IfId.at(0);			
			/*Data Hazard Detection. Need to check whether the decoded values of rs, rt which are now being read aren't clashing
			with those of the write address of the instructions computed before. If there are conflicts, then pipeline would be 
			stalled, the instructions would wait until the correct values have been written.*/

			/*Conditions which are checked:
				IfId.RegisterRs = ExMem.registerRd
				IfId.RegisterRt = ExMem.regiterRd
				IfId.RegisterRs = MemWb.registerRd
				IfId.RegisterRt = MemWb.registerRd
			*/

			/*Assignment-9: Forwarding is used when the instruction isn't a load word (LW) as the value would get saved in the
			last stage after the Memory Access. For rest other arithmetic instructions, we can pipeline the computed result from
			the intermediate registers, ExMem and MemWb. The detection and forwarding is checked and done for each of the
			individual instructions.*/

			/*Control Hazard Detection. For all the branching instructions, the pipeline is stalled for a single cycles or in
			words a NOP is introduced at the earliest stage of the pipeline. Only done for those instructions where branching
			is involved.*/

			int opcode,shamt,func,constant;
			opcode = ToOpcode(regVector.at(0));
			if(opcode == 0){
				int rs,rt,rd;

				rd = regmap(regVector.at(1));
				if(regVector.size()>2){
					rs = regmap(regVector.at(2));
				}			
				if(regVector.size()>3){
					rt = regmap(regVector.at(3));
				}				
				int FE1 = 0;
				int FE2 = 0;
				int FM1 = 0;
				int FM2 = 0;
				if(ExMem.size() > 2){
					if(rs == ExMem.at(2)){
						if(ExMem.at(0) != 35){
							rs = ExMem.at(1);
							FE1 = 1;
						}
						else{
						IdEx.push_back(-1);
						continue;
						}
					}

					if(rt == ExMem.at(2)){
						if(ExMem.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = ExMem.at(1);
							FE2 = 1;
						}
					}
				}
				if(MemWb.size() > 2){
					if(rs == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;}
						else{
							rs = MemWb.at(1);
							FM1 = 1;
						}
					}

					if(rt == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = MemWb.at(1);
							FM2 = 1;
						}
					}
				}

				int func = ToFunc(regVector.at(0));
				if(func == 32){

					IdEx.push_back(opcode);
					if(FE1 || FM1)
						IdEx.push_back(rs);
					else
						IdEx.push_back(file_array[rs]);
					
					if(FE2 || FM2)
						IdEx.push_back(rt);
					else
						IdEx.push_back(file_array[rt]);

					IdEx.push_back(rd);
					IdEx.push_back(0);
					IdEx.push_back(func);

					cout<<"Decode"<<":ADD"<<endl;
				}
				else if(func == 34){

					IdEx.push_back(opcode);
					if(FE1 || FM1)
						IdEx.push_back(rs);
					else
						IdEx.push_back(file_array[rs]);
					
					if(FE2 || FM2)
						IdEx.push_back(rt);
					else
						IdEx.push_back(file_array[rt]);
					IdEx.push_back(rd);
					IdEx.push_back(0);
					IdEx.push_back(func);				
					cout<<"Decode"<<":SUB"<<endl;
				}
				else if(func == 0){
					int shamt;			
					stringstream ss(regVector.at(3));
					ss >> shamt;
					IdEx.push_back(opcode);
					if(FE1 || FM1)
						IdEx.push_back(rs);
					else
						IdEx.push_back(file_array[rs]);
					IdEx.push_back(0);
					IdEx.push_back(rd);	
					IdEx.push_back(shamt);
					IdEx.push_back(func);				
					cout<<"Decode"<<":SLL"<<endl;
				}
				else if(func == 2){
					int shamt;
					stringstream ss(regVector.at(3));
					ss >> shamt;
					IdEx.push_back(opcode);
					if(FE1 || FM1)
						IdEx.push_back(rs);
					else
						IdEx.push_back(file_array[rs]);
					IdEx.push_back(0);
					IdEx.push_back(rd);
					IdEx.push_back(shamt);
					IdEx.push_back(func);			
					cout<<"Decode"<<":SRL"<<endl;
				}
				else{
					cout<<"Invalid Function"<<endl;
				}			
			}
			else if(opcode == 43){
				int rt,offset,rs;
				rt = regmap(regVector.at(1));
				rs = regmap(regVector.at(3));
				stringstream ss(regVector.at(2));
				ss >> offset;
				int FE = 0,FM = 0;
				if(ExMem.size() > 2 && rs == ExMem.at(2)){
					if(ExMem.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = ExMem.at(1);
						FE = 1;
					}
				}

				if(MemWb.size() > 2 && rs == MemWb.at(2)){
					if(MemWb.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = MemWb.at(1);
						FM = 1;
					}
				}
				IdEx.push_back(opcode);
				if(FE || FM)
					IdEx.push_back(rs);
				else
					IdEx.push_back(file_array[rs]);
				IdEx.push_back(rt);
				IdEx.push_back(offset);
				cout<<"Decode"<<":"<<"SW"<<endl;	
			}
			else if(opcode == 35){
				int rt,offset,rs;
				rt = regmap(regVector.at(1));
				rs = regmap(regVector.at(3));
				stringstream ss(regVector.at(2));
				ss >> offset;
				int FE = 0, FM = 0;
				if(ExMem.size() > 2 && rs == ExMem.at(2)){
					
					if(ExMem.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = ExMem.at(1);
						FE = 1;
					}
				}

				if(MemWb.size() > 2 && rs == MemWb.at(2)){
					if(MemWb.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = MemWb.at(1);
						FM = 1;
					}
				}
				IdEx.push_back(opcode);
				if(FE || FM)
					IdEx.push_back(rs);
				else
					IdEx.push_back(file_array[rs]);
				IdEx.push_back(rt);
				IdEx.push_back(offset);
				cout<<"Decode:"<<"LW"<<endl;
			}

			else if(opcode == 5){
				int rs,offset,rt;
				rt = regmap(regVector.at(2));
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(3));
				ss >> offset;
				int FE1 = 0,FE2 = 0,FM1 = 0,FM2 = 0;
				if(ExMem.size() > 2){
					if(rs == ExMem.at(2)){
						if(ExMem.at(0) != 35){
							rs = ExMem.at(1);
							FE1 = 1;
						}
						else{
						IdEx.push_back(-1);
						continue;
						}
					}

					if(rt == ExMem.at(2)){
						if(ExMem.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = ExMem.at(1);
							FE2 = 1;
						}
					}
				}
				if(MemWb.size() > 2){
					if(rs == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;}
						else{
							rs = MemWb.at(1);
							FM1 = 1;
						}
					}

					if(rt == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = MemWb.at(1);
							FM2 = 1;
						}
					}
				}
				IdEx.push_back(opcode);
				if(FE1 || FM1)
					IdEx.push_back(rs);
				else
					IdEx.push_back(file_array[rs]);
				if(FE2 || FM2)
					IdEx.push_back(rt);
				else
					IdEx.push_back(file_array[rt]);
				IdEx.push_back(offset);
				IdEx.push_back(stoi(IfId.at(1).at(0)));			
				cout<<"Decode"<<":BNE"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;
			}

			else if(opcode == 4){
				int rs,offset,rt;
				rt = regmap(regVector.at(2));
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(3));
				ss >> offset;
				int FE1 = 0,FE2 = 0,FM1 = 0,FM2 = 0;
				if(ExMem.size() > 2){
					if(rs == ExMem.at(2)){
						if(ExMem.at(0) != 35){
							rs = ExMem.at(1);
							FE1 = 1;
						}
						else{
						IdEx.push_back(-1);
						continue;
						}
					}

					if(rt == ExMem.at(2)){
						if(ExMem.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = ExMem.at(1);
							FE2 = 1;
						}
					}
				}
				if(MemWb.size() > 2){
					if(rs == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;}
						else{
							rs = MemWb.at(1);
							FM1 = 1;
						}
					}

					if(rt == MemWb.at(2)){
						if(MemWb.at(0) == 35){
							IdEx.push_back(-1);
							continue;
						}
						else{
							rt = MemWb.at(1);
							FM2 = 1;
						}
					}
				}
				IdEx.push_back(opcode);
				if(FE1 || FM1)
					IdEx.push_back(rs);
				else		
					IdEx.push_back(file_array[rs]);

				if(FE2 || FM2)
					IdEx.push_back(rt);
				else
					IdEx.push_back(file_array[rt]);
				IdEx.push_back(offset);
				IdEx.push_back(stoi(IfId.at(1).at(0)));				
				cout<<"Decode"<<":BEQ"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;				
			}
									
			else if(opcode == 2){
				int address;
				stringstream ss(regVector.at(1));
				ss>>address;
				IdEx.push_back(opcode);
				IdEx.push_back(address);
				cout<<"Decode"<<":J"<<endl;	

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;
			}

			else if(opcode == 8){

				if(ExMem.size() > 2 && ExMem.at(2) == 31){
					IdEx.push_back(-1);
					continue;
				}
				if(MemWb.size() > 2 && MemWb.at(2) == 31){
					IdEx.push_back(-1);
					continue;
				}
				IdEx.push_back(opcode);
				IdEx.push_back(file_array[31]);

//				cout<<"ra "<<file_array[31]<<endl; 
				cout<<"Decode"<<":JR"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;				
			}

			else if(opcode == 3){
				//file_array[31] = i+1;
				int address;
				stringstream ss(regVector.at(1));
				ss>>address;
				IdEx.push_back(opcode);
				IdEx.push_back(address);
				IdEx.push_back(31);
				IdEx.push_back(pc);				
				cout<<"Decode"<<":JAL"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;
			}

			else if(opcode == 6){
				int rs,offset;
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(2));
				ss >> offset;
				int FE = 0, FM = 0;
				if(ExMem.size() > 2 && rs == ExMem.at(2)){
					if(ExMem.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = ExMem.at(1);
						FE = 1;
					}
				}
				if(MemWb.size() > 2 && rs == MemWb.at(2)){
					if(MemWb.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = MemWb.at(1);
						FM = 1;
					}
				}
				IdEx.push_back(opcode);
				if(FE || FM)
					IdEx.push_back(rs);
				else
					IdEx.push_back(file_array[rs]);
				IdEx.push_back(offset);
				IdEx.push_back(stoi(IfId.at(1).at(0)));
				cout<<"Decode"<<":BLEZ"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;			
			}

			else if(opcode == 7){
				int rs,offset;
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(2));
				ss >> offset;
				int FE = 0, FM = 0;
				if(ExMem.size() > 2 && rs == ExMem.at(2)){
					if(ExMem.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = ExMem.at(1);
						FE = 1;
					}
				}

				if(MemWb.size() > 2 && rs == MemWb.at(2)){
					if(MemWb.at(0) == 35){
						IdEx.push_back(-1);
						continue;
					}
					else{
						rs = MemWb.at(1);
						FM = 1;
					}
				}

				IdEx.push_back(opcode);
				if(FE || FM)
					IdEx.push_back(rs);
				else
					IdEx.push_back(file_array[rs]);
				IdEx.push_back(offset);
				IdEx.push_back(stoi(IfId.at(1).at(0)));
				cout<<"Decode"<<":BGTZ"<<endl;

				vector<string> temp;
				temp.push_back("");
				IfId.clear();
				IfId.push_back(temp);
				continue;
			}
			else if(opcode == -1){
				//Do Nothing
				IdEx.push_back(-1);
			}
			else{
				cout<<"Invalid Opcode"<<endl;
			}
		}
		else{
			IdEx.push_back(-1);
			cout<<"Decode:NOP"<<endl;
		}

	/*Instruction Fetch */
		//cout<<numInstr<<endl;
		if(pc<n){
			/*The Vector IfId is a vector of vector strings which contains the instruction along with the type cast program
			counter*/
			cout<<"Fetch:"<<pc<<endl;
			vector<string> regVector = input.at(pc);
			pc++;
			IfId.clear();
			IfId.push_back(regVector);
			vector<string> temp;
			temp.push_back(to_string(pc));
			IfId.push_back(temp);

		}
		else{
			/*Vector with empty string*/
			IfId.clear();
			vector<string> temp;
			temp.push_back("");
			IfId.push_back(temp);
			cout<<"Fetch:NOP"<<endl;
			pc++;
		}
		cout<<endl;
	}
	while(pc<n+4);
	
	cout<<"$t0 "<<file_array[8]<<endl;
	cout<<"$t1 "<<file_array[9]<<endl;
	cout<<"$t2 "<<file_array[10]<<endl;
	cout<<"$t3 "<<file_array[11]<<endl;
	cout<<"$t4 "<<file_array[12]<<endl;
	cout<<"$t5 "<<file_array[13]<<endl;
	cout<<"$t6 "<<file_array[14]<<endl;
	cout<<"$t7 "<<file_array[15]<<endl;
	cout<<"$sp "<<file_array[29]<<endl;
	cout<<"No. of Instructions per Cycle: " << numInstr/clock << endl;
	return 0;
}

