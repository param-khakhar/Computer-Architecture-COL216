#include <bits/stdc++.h>
using namespace std;

/* Typical Representation of a Cache Element */
struct Block{
    int valid = 0;          // The valid bit. 0 denotes absence and 1 denotes presence.
    int tag = 0;            // The tag bits help in computing the exact memory element which is present in the set.
    int data = -1;          // The data fetched from the memory.
    int dirty = 0;          // Denotes whether the block is updated or not.
};

/* Representation of an element of a Cache */
struct CacheE{
    int NS;                 // NS: The number of sets, i.e its value would be equal to the associativity.
    map<int,Block> HP;      // The High Priority Lines. The integer denotes the index of the last instruction which used this block.
    map<int,Block> LP;      // The Low Priority Lines. The integer denotes the index of the last instruction which used this block.
};

/* The function initializes the Cache which is a (struct CacheE array []) depending on the parameters provided as arguments.*/
void init(struct CacheE c[], int length, int associativity){

    for(int i = 0;i<length;i++){

        c[i].NS = associativity;
        // if(associativity == 1){
        //     c[i].HP[-1] = {0,0,0,0}; 
        // }
        // else{
        //     for(int j = 0;j<associativity/2;j++){
        //         c[i].HP[-1] = {0,0,0,0};
        //         c[i].LP[-1] = {0,0,0,0};
        //     }
        // }
    }
}

/* The function checks whether the adress queried in the cache is present or not. */
bool search(struct CacheE c[], int memory[], int length, int adr, int cur, int T){

    int la = adr % length;
    int ns = c[la].NS;

    /* Check the high priority and low priority low blocks within each set by checking the presence. */

    map<int, Block> :: iterator it1 = c[la].HP.begin();
    map<int, Block> :: iterator it2 = c[la].LP.begin();

    /* Iterate over High Priority Block */
    while(it1 != c[la].HP.end()){
        Block b = it1->second;
        if(b.valid && b.tag == (adr / length)){

            // Change the index and again reinsert into the map, after its removal. No need to check for LRU.
            
            if(it1->second.dirty == 1){
                memory[adr] = it1->second.data;
            }
            c[la].HP.erase(it1->first);
            c[la].HP[cur] = b;
            return true;
        }
        it1++;
    }        

    /* Iterate over Low Priority Block */
    while(it2 != c[la].LP.end()){
        Block b = it2->second;
        if(b.valid && b.tag == (adr / length)){

            // Code for updating the low priority to high priority, updating the high priority block and LRU check.
            if((cur - it2->first) <= T){
                c[la].LP.erase(it2->first);
                c[la].HP[cur] = b;
            }
            // else{
            //     c[la].LP[cur] = b;
            // }
            return true;
        }
        it2++;
    }
    return false;
}

/* In case of a read miss, this function fetches the data from the memory and returns a boolean denoting the success or failure.*/
bool fetch(struct CacheE cache [], int length, int memory [], int adr, int curr){

    int cache_size = length;
    int cadr = adr % cache_size;
    map<int,Block> :: iterator it1 = cache[cadr].LP.begin();
    if(it1->second.dirty == 1){
        memory[cadr + length*it1->second.tag] = it1->second.data;
    }
    cache[cadr].LP[curr] = {1, adr / cache_size, memory[adr]};
    return true;
}

/* The function updates the value (write hit) present in the High Priority line or the Lower Priority Line, and thereby updating the dirty to be 1
and changing the key as well. */

bool update(struct CacheE c[], int length, int dat, int adr, int cur){

    int cadr = adr % length;
    map<int, Block> :: iterator it1 = c[cadr].HP.begin();
    while(it1 != c[cadr].HP.end()){
        Block b = it1->second;  
        if(b.tag == (adr / length)){
            c[cadr].HP.erase(it1->first);
            c[cadr].HP[cur] = {1,(adr/length),dat,1};
            return true;
        }
        it1++;
    }
    map<int, Block> :: iterator it2 = c[cadr].LP.begin();
    while(it2 != c[cadr].LP.end()){
        Block b = it2->second;
        if(b.tag == (adr / length)){
            c[cadr].LP.erase(it2->first);
            c[cadr].LP[cur] = {1,(adr/length),dat,1};
            return true;
        }
        it2++;
    }
    return false;
}

/* The function inserts the new block (write miss) in the Low Priority Block in case of a  */
bool insert(struct CacheE c[], int memory [], int data, int length, int adr, int curr){
    int cadr = adr % length;
    map<int,Block> :: iterator it1 = c[cadr].LP.begin();
    if(it1 != c[cadr].LP.end() && it1->second.dirty == 1){
        memory[cadr + length*(it1->second.tag)] = it1->second.data;
    }
    c[cadr].LP[curr] = {1,(adr / length),data,1};
    return true;
}

/* Function checks if there are any excess elements collectively in both High and Low priority lines */
void check(struct CacheE c[], int memory [], int length, int e, int curr, int T){

    for(int i = 0;i<length;i++){

            
        map<int, Block> :: iterator it1 = c[i].HP.begin();

        while(it1 != c[i].HP.end()){
            if((curr - it1->first) > T){
                Block b = it1->second;
                c[i].LP[it1->first] = {b.valid,b.tag,b.data,b.dirty};
                c[i].HP.erase(it1->first);
                it1++;
            }
            else{
                break;
            }
        }

            
        map<int,Block> :: iterator it2 = c[i].LP.begin();
        while(c[i].HP.size() + c[i].LP.size() > e && it2 != c[i].LP.end()){
            
            if(it2->second.dirty){
                memory[it2->second.tag*length + i] = it2->second.data;
            }
            c[i].LP.erase(it2->first);
            it2++;
        }
    }
}

int main(int argc, char** argv)
{

    /*Variable Initializations */
    int CS = 0;             // Cache Size: Decides the number of unique sets which can co-exist at a time in the cache.
    int BS = 0;             // Block Size: Size of the data stored + Size of Tag Bit + Size of Valid Bit + Size of Dirty Bit.
    int A = 0;              // Associativity: Number of blocks within a set.
    int T = 0;              // T: Priority Change parameter.
    
    int a = 0;
	ifstream infile(argv[1]);
	string line;
	vector<vector<string>> input;

    /* Input preprocessing */
	while(getline(infile,line)){
		vector<string> regVector;

		size_t prev = 0, pos;
    	while ((pos = line.find_first_of(" ,", prev)) != std::string::npos){
        	if (pos > prev)
            	regVector.push_back(line.substr(prev, pos-prev));
        	prev = pos+1;
    	}
    	if (prev < line.length())
        	regVector.push_back(line.substr(prev, std::string::npos));

        if(a <= 4){
            stringstream ss(regVector[0]);
            if(a == 0){
                ss >> CS;
            }
            else if(a == 1){
                ss >> BS;
            }
            else if(a == 2){
                ss >> A;
            }
            else if(a == 3){
                ss >> T;
            }
            else{
                break;
            }
            a++;
        }
    }
    int CL = CS / (A * BS);         // CL: Cache Length. Denotes the length of the cache which is calculated by dividing Cache Size / (A * Block Size).
    int DB = BS*8 - (1+1+4);        // DB: Data Bits Available for storage of data. Therefore the max magnitude possible for
                                    // storing data would be 2 ^ DB.
    int ms = 1;                     // ms : Max magnitude of the data.
    for(int i = 0;i<DB;i++){
        ms = ms*2;
    }

    if(DB <= 0){
        cout<<"No size available for Data Storage.\n";
        return 1;
    }
    /*Initializations for Hierarchy*/

    /* Memory Declarations:
    Memories are declared as 2D integer arrays. 
    The length of the cache memory is equal to the 
    The width of the memory would be equal to that of the block size. Fixing the length would also fix the size of the
    tag bits. */

    CacheE cache[CL];
    init(cache,CL,A);

    int memory[16 * CL];    //The size of the main memory is 16 times the one of the cache memory.

    /* For initializing memory, each index stores its index i.e.
        memory[0] = 0;
        memory[1] = 1;
        memory[2] = 2...etc */

    for(int i = 0;i<16*CL;i++){
        memory[i] = i;
    }

    int index = 0;          // Denotes the ith instruction.
    int reads = 0;          // Counts the number of reads.
    int writes = 0;         // Counts the number of writes.
    int reads_m = 0;        // Counts the number of read-misses.
    int writes_m = 0;       // Counts the number of write-misses.


    /* Processing the instructions till an empty line is recieved.*/
	while(getline(infile,line)){

		vector<string> regVector;

		size_t prev = 0, pos;
    	while ((pos = line.find_first_of(" ,", prev)) != std::string::npos){
        	if (pos > prev)
            	regVector.push_back(line.substr(prev, pos-prev));
        	prev = pos+1;
    	}
    	if (prev < line.length())
        	regVector.push_back(line.substr(prev, std::string::npos));

        input.push_back(regVector);

        if(regVector[1] == "W"){
            
            stringstream ss0(regVector[0]);
            stringstream ss1(regVector[2]);

            int adr,data;
            ss0 >> adr;
            ss1 >> data;
            if(data >= ms){
                cout<<"Data Overflow. Instruction can't be executed\n";
                continue;
            }
            //cout<<"ADDRESS:"<<adr<<" "<<"Data:"<<data<<"\n";
            int mem_size = sizeof(memory)/sizeof(memory[0]);
            if(adr >= mem_size || adr < 0){
                cout<<"Invalid Address\n";
                continue;
            }
            else{
                bool find = search(cache, memory, CL,adr,index,T);
                //cout<<"Here\n";
                if(find){
                    update(cache, CL, data, adr, index);
                }
                else{
                    writes_m++;
                    bool ins = insert(cache, memory, data, CL, adr, index);
                    
                }
            }
            writes++;
        }
        else if(regVector[1] == "R"){
            
            stringstream ss(regVector[0]);
            int adr;
            ss >> adr;
            if(adr >= 16 * CL || adr < 0){
                cout<<"Invalid Address\n";
                continue;
            }
            bool find = search(cache, memory, CL, adr, index, T);
            if(find){
                reads++;
            }
            else{
                bool res = fetch(cache, CL, memory, adr, index);
                if(res){
                    reads_m++;
                    reads++;
                }
            }
        }
        else{
            cout<<"Invalid Instruction \n";
        }

        
        check(cache,memory,CL,A,index,T);
        // cout<<"\n";
        // for(int i = 0;i<CL;i++){

        //     CacheE c = cache[i];
        //     map<int,Block> hp = c.HP;
        //     map<int, Block> lp = c.LP;

        //     map<int,Block> :: iterator it1 = hp.begin();
        //     map<int,Block> :: iterator it2 = lp.begin();
        //     int a = 0;
        //     int b = 0;
        //     while(it1 != hp.end()){
        //         Block b1 = it1->second;
        //         cout<<"H:"<<i<<" "<<b1.data<<", "<<b1.tag<<", "<<b1.valid<<", "<<b1.dirty<<"\n";
        //         it1++;
        //         a++;
        //     }
        //     while(it2 != lp.end()){
        //         Block b2 = it2->second;
        //         cout<<"L:"<<i<<" "<<b2.data<<", "<<b2.tag<<", "<<b2.valid<<", "<<b2.dirty<<"\n";
        //         it2++;
        //         b++;
        //     }
        //     int s = a+b;
        //     if(s < A){
        //         while(s < A){
        //                 cout<<"U:"<<i<<" "<<"0"<<", "<<"0"<<", "<<"0"<<", "<<"0"<<"\n";
        //                 s++;
        //         } 
        //     }
        // }
        // cout<<"\n";
        index++;
    }

    cout<<"#Set-No, Priority, Data, Tag, Valid-Status(Valid = 1), Dirty-Status(dirty = 1)\n";
//    cout<<"\n";
    for(int i = 0;i<CL;i++){

        CacheE c = cache[i];
        map<int,Block> hp = c.HP;
        map<int, Block> lp = c.LP;

        map<int,Block> :: iterator it1 = hp.begin();
        map<int,Block> :: iterator it2 = lp.begin();
        int a = 0;
        int b = 0;
        // cout<<"Size: HP"<<hp.size()<<" "<<"Size: LP"<<" "<<lp.size()<<"\n";
        while(it1 != hp.end()){
            Block b1 = it1->second;
            cout<<i<<" H"<<"  "<<b1.data<<", "<<b1.tag<<", "<<b1.valid<<", "<<b1.dirty<<"\n";
            it1++;
            a++;
        }
        while(it2 != lp.end()){
            Block b2 = it2->second;
            cout<<i<<" L"<<"  "<<b2.data<<", "<<b2.tag<<", "<<b2.valid<<", "<<b2.dirty<<"\n";
            it2++;
            b++;
        }
        int s = a+b;
        if(s < A){
           while(s < A){
                cout<<i<<" U"<<"  "<<"0"<<", "<<"0"<<", "<<"0"<<", "<<"0"<<"\n";
                s++;
           } 
        }
    }

    cout<<"Cache Statistics:\n";
    cout<<"Number of Accesses = "<<index<<"\n";
    cout<<"Number of Reads = "<<reads<<"\n";
    cout<<"Number of Read Hits = "<<reads-reads_m<<"\n";
    cout<<"Number of Read Misses = "<<reads_m<<"\n";
    cout<<"Number of Writes = "<<writes<<"\n";
    cout<<"Number of Write Hits = "<<writes - writes_m <<"\n";
    cout<<"Number of Write Misses = "<<writes_m<<"\n";
    cout<<"Hit Ratio = "<<(reads+writes-reads_m-writes_m)/float(index)<<"\n";

    return 0;
}