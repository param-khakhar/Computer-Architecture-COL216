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

int main(int argc, char** argv){
	ifstream infile(argv[1]);
	string line;
	//Memory Initializations
	int memory [4096];

	int file_array [32];

	for(int i=0;i<32;i++){
		file_array[i] = 0;
	}

	for(int i=0;i<4096;i++){
		memory[i] = 0;
	}
	//File Array Initializations
	file_array[29] = 4095; //$sp
	file_array[31] = 0;
	file_array[8] = -4;
	file_array[9] = 2;
	file_array[10] = 3;
	file_array[0] = 0;


	vector<vector<string>> input;
	int clock = 0;

	// Value of Delays for each instruction

	int d_add = 5;
	int d_sub = 5;
	int d_sll = 5;
	int d_srl = 5;
	int d_sw = 5;
	int d_lw = 5;
	int d_bne = 5;
	int d_beq = 5;
	int d_j = 5;
	int d_jal = 5;
	int d_blez = 5;
	int d_bgtz = 5;
	int d_jr = 5;

	int pc = 0;	
	while(getline(infile,line)){
		vector<string> regVector;
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
	}*/
	
	int n = input.size();
	for(int i = 0; i < n;i++){
		//break;
		//printFA(file_array);
		vector<string> regVector = input.at(i);
		if(regVector.size() == 4){
			if(regVector.at(0) == "ADD"){
				int rs,rt,rd;
				rd = regmap(regVector.at(1));
				rs = regmap(regVector.at(2));			
				rt = regmap(regVector.at(3));
				file_array[rd] = file_array[rs] + file_array[rt];
				pc = pc + 4;
				clock += d_add;
				cout<<i<<":ADD"<<endl;
			}
			else if(regVector.at(0) == "SUB"){
				int rs,rt,rd;
				rd = regmap(regVector.at(1));
				rs = regmap(regVector.at(2));			
				rt = regmap(regVector.at(3));
				file_array[rd] = file_array[rs] - file_array[rt];
				pc = pc + 4;
				clock += d_sub;
				cout<<i<<":SUB"<<endl;
			}
			else if(regVector.at(0) == "SLL"){
				int rs,shamt,rd;
				rd = regmap(regVector.at(1));
				rs = regmap(regVector.at(2));			
				stringstream ss(regVector.at(3));
				ss >> shamt;
				file_array[rd] = file_array[rs] << shamt;
				pc = pc + 4;
				clock += d_sll;
				cout<<i<<":SLL"<<endl;
			}
			else if(regVector.at(0) == "SRL"){
				int rs,shamt,rd;
				rd = regmap(regVector.at(1));
				rs = regmap(regVector.at(2));			
				stringstream ss(regVector.at(3));
				ss >> shamt;
				file_array[rd] = file_array[rs] >> shamt;
				pc = pc + 4;
				clock += d_srl;
				cout<<i<<":SRL"<<endl;
			}
			else if(regVector.at(0) == "SW"){
				int rt,offset,rs;
				rt = regmap(regVector.at(1));
				rs = regmap(regVector.at(3));
				stringstream ss(regVector.at(2));
				ss >> offset;
				//cout<<file_array[rs]+offset<<endl;
				//cout<<rs<<endl;
				//cout<<file_array[rs]<<endl;
				//cout<<offset<<endl;
				memory[file_array[rs]+offset] = file_array[rt];
				cout<<i<<":"<<"SW"<<endl;
				//break;
				clock += d_sw;
				pc = pc + 4;				
			}
			else if(regVector.at(0) == "LW"){
				int rt,offset,rs;
				rt = regmap(regVector.at(1));
				rs = regmap(regVector.at(3));
				stringstream ss(regVector.at(2));
				ss >> offset;
				cout<<i<<":LW"<<endl;
				file_array[rt] = memory[file_array[rs]+offset];
				clock += d_lw;
				pc = pc + 4;				
			}
			else if(regVector.at(0) == "BNE"){
				int rs,offset,rt;
				rt = regmap(regVector.at(2));
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(3));
				if(file_array[rs] != file_array[rt]){
					i = offset - 1;
				}
				pc = pc + 4;
				clock += d_bne;
				cout<<i<<":BNE"<<endl;
			}
			else if(regVector.at(0) == "BEQ"){
				int rs,offset,rt;
				rt = regmap(regVector.at(2));
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(3));
				if(file_array[rs] == file_array[rt]){
					i = offset - 1;
				}
				pc = pc + 4;
				clock += d_beq;
				cout<<i<<":BEQ"<<endl;
			}									
			else{
				cout<<regVector.at(0)<<endl;
				cout<<"Invalid"<<endl;
				break;
			}
		}
		else if(regVector.size() == 2){
			if(regVector.at(0) == "J"){
				int address;
				stringstream ss(regVector.at(1));
				ss>>address;
				cout<<i<<":J"<<endl;
				clock += d_j;
				i = address-1;				
			}
			else if(regVector.at(0) == "JR"){
				cout<<i<<":JR"<<endl;
				clock += d_jr;
				i = file_array[31] - 1;
			}
			else if(regVector.at(0) == "JAL"){
				file_array[31] = i+1;
				int address;
				stringstream ss(regVector.at(1));
				ss>>address;
				cout<<i<<":JAL"<<endl;
				clock += d_jal;
				//cout<<address<<endl;
				//break;
				i = address-1;
			}
			else{
				cout<<"Invalid Instruction"<<endl;
				break;
			}			
		}
		else if(regVector.size() == 3){

			if(regVector.at(0) == "BLEZ"){
				int rs,offset;
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(2));
				ss >> offset;
				if(file_array[rs] <= 0){
					i = offset - 1;					
				}
				clock += d_blez;
				pc = pc + 4;
				cout<<i<<":BLEZ"<<endl;
			}
			else if(regVector.at(0) == "BGTZ"){
				int rs,offset;
				rs = regmap(regVector.at(1));			
				stringstream ss(regVector.at(2));
				ss >> offset;
				if(file_array[rs] > 0){
					i = offset - 1;
				}
				pc = pc + 4;
				clock += d_bgtz;
				cout<<i<<":BGTZ"<<endl;
			}
			else{
				cout<<"Invalid Instruction"<<endl;
				break;
			}						
		}
		else{
			cout<<"Invalid Instruction"<<endl;
			break;
		}
	}
	printFA(file_array);
	printMemory(memory);

	//Statistics which need to be printed:
	cout <<clock<<endl;
	float f = (float)n/(float)clock;
	cout<<"Average IPC: "<<f<<endl;
	cout<<"Total Instructions:"<<n<<"\n";
	return 0;
}
