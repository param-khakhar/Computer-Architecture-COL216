#include <bits/stdc++.h>
using namespace std;

int main(){
    // The numbers to be added are read as a and b.
    string a,b;
    cin>>a>>b;

    // The sign bits for both the numbers.
    int signa = a[0] - '0';
    int signb = b[0] - '0';

    // The variable result stores the addition result after the stage-3;
    string result = "";
    // The variable contains the carry while performing bit-wise addition.
    int carry = 0;

    // The exponent bits for the strings would be the numbers at indices 1 to 8.
    string expa = a.substr(1,8);
    string expb = b.substr(1,8);

    // The significand part of the two strings would be the bits with index 9 to 32.
    string siga = a.substr(9,23);
    string sigb = b.substr(9,23);

    //Three Zeroes are appended at the end of the binary number which are Guard(G), Round(R) and Sticky(S). This would help in
    //improving the precision as well as aid in the rounding procedure.

    siga.insert(23,3,'0');
    sigb.insert(23,3,'0');

    //The variable clock contains the number of clock cycles passed.
    int clock = 0;

    // The variable finish denotes the completion of the computation process.
    bool finish = false;

    // The entire computation is carried out in 4 stages and therefore, the variable state denotes the current
    // stage of the computation.
    int stage = 1;

    //The variable res stores the result in string after the addition.
    string res;

    //The integer value of the exponents.
    int expai,expbi;

    //Sign of the result.
    int signr;

    //Variables left and right denote the side of the shift afer calculating the sum.
    bool left = false, right = false;

    //Sticky bits represent the presence of 1 in the part which has been truncated during the matching of exponents.
    int sticky = 0;

    //In each iteration of the while loop, a particular stage would be carried out.
    while(!finish){

        if(stage == 1){

            /*Stage-1: 
            Exponent Comparison. In this step the exponents of the two numbers would be compared. The exponent in the
            representation would be the 8 bits with index 1 to 8. The string having the lower exponent has been right shifted
            and the first 24 bits have been extracted followed by the right shift. Three extra bits are introduced namely 
            Ground (G), Round (R) and Sticky (S) (in the decreasing order of their significance). Bits G and R are usual bits
            and they are simply used for increased precision. The sticky bits represent whether any of the truncated bits to its 
            right is 1 or not.*/

            expai = stoi(expa,0,2);
            expbi = stoi(expb,0,2);

            if(expai == 255 || expbi == 255){
                if(siga == "00000000000000000000000000" || sigb == "00000000000000000000000000"){
                    cout<<"Infinity"<<"\n";
                    cout<<clock+1<<"\n";
                    break;
                }
                else{
                    cout<<"NaN"<<"\n";
                    cout<<clock+1<<"\n";
                    break;
                }
            }
            if((expai == 0 && siga != "00000000000000000000000000") || (expbi == 0 && sigb != "00000000000000000000000000")){
                cout<<"Denormalized Number"<<"\n";
                cout<<clock+1<<"\n";
                break;
            }

            int extra;
            if(expai < expbi){
                extra = expbi - expai;
                siga.insert(0,1,'1');
                siga.insert(0,extra,'0');

                if(siga.size() > 27){
                    sticky = 0;
                    for(int i = 27;i < siga.size();i++){
                        sticky = (siga[i] - '0') ^ sticky;
                    }
                }
                if(sticky){
                    siga = siga.substr(0,26);
                    siga.insert(25,1,'1');
                }
                else{
                    siga = siga.substr(0,27);
                }
                sigb.insert(0,1,'1');
                expai += extra;
            }
            else{
                extra = expai - expbi;
                sigb.insert(0,1,'1');
                sigb.insert(0,extra,'0');
                sticky = 0;
                if(sigb.size() > 27){
                    for(int i = 27;i<sigb.size();i++){
                        sticky = sticky ^ (sigb[i]-'0');
                    }
                }
                if(sticky){
                    sigb = sigb.substr(0,26);
                    sigb.insert(25,1,'1');
                }
                else{
                    sigb = sigb.substr(0,27);
                }
                siga.insert(0,1,'1');
                expbi += extra;
            }
            stage += 1;
            clock += 1;
            //cout<<"Stage:1-"<<siga<<" "<<sigb<<"\n";  
            //cout<<siga.size()<<" "<<sigb.size()<<"\n";          
        }

        else if(stage == 2){

            /*Stage - 2: Here, the significands would be added as the exponents are equalled and the right shifts
            followed by truncating have been completed. The operands are converted in the decimal form for performing
            the operation, and the sign bit is stored for future utility.*/

            long long ai = stoi(siga,0,2);
            long long bi = stoi(sigb,0,2);

            //cout<<ai<<" "<<bi<<endl;
            //cout<<siga<<" "<<sigb<<endl;
            if(signa == 1)
                ai = ai*-1;
            if(signb == 1)
                bi = bi*-1;

        //    cout<<ai<<" "<<bi<<"\n";
            long long resi = ai + bi;
            
            signr = 0;

            if(resi < 0){
                signr = 1;
                resi = resi * -1;
            }

            // The result of the operation would be atmost 28 bits, therefore, the conversion is carried out using 28 bits.
            res = std :: bitset<28> (resi).to_string();
            //cout<<siga<<" "<<sigb<<endl;
            //cout<<"Stage-2:"<<res<<endl;
            //cout<<resi<<endl;

            clock += 1;
            stage += 1;
        }

        else if(stage == 3){

            /*Stage-3: The significands have been added, and we have the carry and the result variables containing the results
            of the stage-2. In this stage, the normalizing procedure would have to be executed.*/

            //cout<<res<<endl;
            
            if(res[0] == '1'){
                //Shift Right and increase the exponent by 1.
                right = true;

                if(res[27] == '1'){
                    res = res.substr(0,26);
                    res.insert(25,1,'1');
                }
                else{
                    res = res.substr(0,27);
                }

                expai += 1;
                if(expai >= 255){
                    //Condition for Overflow
                    cout<<"OverFlow Exception"<<endl;
                    cout<<clock+1<<endl;
                    break;
                }
                //cout<<"Stage-3:"<<res<<endl;
            }

            else{
                //Shift Left and decrease the exponent accordingly.
                left = true;
                res = res.substr(1,27);
                //cout<<clock<<endl;
                if(res[0] == '0'){
                    int one = 0;
                    //cout<<expai<<endl;
                    //cout<<"Stage-3A:"<<res<<endl;
                    while(one < 27 && res[one] != '1')
                        one++;
                    expai -= one;
                    //Check for Underflow
                    //cout<<"Expai:"<<expai<<"\n";
                    if(expai < 1){
                        cout<<"UnderFlow Exception"<<endl;
                        cout<<clock+1<<"\n";
                        break;
                    }
                    else{
                        //Normalization.
                        if(one < 26){
                            res = res.substr(one,27-one);
                            res.insert(26-one,one,'0');
                //            cout<<"Norm:"<<res<<endl;
                        }
                    }
                }
                //else{
                    //cout<<"Stage-3:"<<res<<"\n";
                //}
            }
            clock += 1;
            stage = 4;           
        }
        else{
            /*Stage-4: In this step, the numbers are rounded, from the normalizing step, the 27 bit numbers are recieved and
            the rounding procedure accordingly to the procedure described in the README.*/

            int g = res[24] - '0';
            int r = res[25] - '0';
            int s = res[26] - '0';

            if(right){
                if(g == 0){
                    //Do Nothing
                    //cout<<"Stage-4:"<<res<<endl;

                    res = res.substr(1,23);

                    if(siga == sigb && res == "00000000000000000000000"){
                        string rese = "00000000";
                        res.insert(0,1,'0');
                    }
                    else{
                        string rese = std :: bitset<8> (expai).to_string();
                        res.insert(0,rese);
                        if(signr)
                            res.insert(0,1,'1');
                        else
                            res.insert(0,1,'0');
                    }
                    finish = true;
                    clock += 1;
                    cout<<res<<"\n";
                    //cout<<res.size()<<"\n";
                    cout<<clock<<"\n";                
                }
                else{
                    if(r || s){
                        //Round Up
                        res = res.substr(0,24);            
                        long long temp = stoi(res,0,2);
                        res = std :: bitset<25> (temp+1).to_string();
                        res.insert(24,2,'0');
                        stage = 3;
                    }
                    else{
                        //Tie. Rounding to the nearest even.

                        if(res[23] - '0' == 0){
                            res = res.substr(1,23);

                            if(siga == sigb && res == "00000000000000000000000"){
                                string rese = "00000000";
                                res.insert(0,1,'0');
                            }
                            else{
                                string rese = std :: bitset<8> (expai).to_string();
                                res.insert(0,rese);
                                if(signr)
                                    res.insert(0,1,'1');
                                else
                                    res.insert(0,1,'0');
                            }
                            finish = true;
                            clock += 1;
                            cout<<res<<"\n";
                            cout<<clock<<"\n";
                        }
                        else{
                            //Round Up
                            res = res.substr(0,24);
                            long long temp = stoi(res,0,2);
                            res = std :: bitset<28> (temp+1).to_string();
                            stage = 3;                        
                        }

                    }
                }
            }
            else{
                //Left Shift. A 27 bit number with the last 3 bits G,R and S.

                if(g == 0){
                    //Do Nothing
                    //cout<<"Stage:4-"<<res<<endl;
                    res = res.substr(1,23);
                    if(siga == sigb && res == "00000000000000000000000"){
                        string rese = "00000000";
                        res.insert(0,rese);
                        res.insert(0,1,'0');
                    }
                    else{
                        string rese = std :: bitset <8> (expai).to_string();
                        res.insert(0,rese);
                        if(signr)
                            res.insert(0,1,'1');
                        else
                            res.insert(0,1,'0');
                    }
                    finish = true;
                    clock += 1;
                    cout<<res<<"\n";                    
                    cout<<clock<<"\n";

                }                
                else{
                    clock += 1;
                    if(r || s){
                        //Round Up
                        res = res.substr(0,24);
                        //cout<<"S4:subst:"<<res<<endl;
                        long long temp = stoi(res,0,2);                    //cout<<res<<"\n";
                        res = std :: bitset<25> (temp+1).to_string();
                        res.insert(24,2,'0');
                        //cout<<res<<endl;
                        stage = 3;
                    }
                    else{
                        //Tie. Rounding to the nearest even.

                        if(res[23] - '0' == 0){

                            res = res.substr(1,23);
                            if(siga == sigb && res == "00000000000000000000000"){
                                string rese = "00000000";
                                res.insert(0,1,'0');
                            }
                            else{
                                string rese = std :: bitset<8> (expai).to_string();
                                res.insert(0,rese);
                                if(signr)
                                    res.insert(0,1,'1');
                                else
                                    res.insert(0,1,'0');
                            }
                            finish = true;
                            clock += 1;
                            cout<<res<<"\n";
                            cout<<clock<<"\n";

                        }
                        else{
                            //Round Up
                            res = res.substr(0,24);
                            long long temp = stoi(res,0,2);
                            res = std :: bitset<28> (temp+1).to_string();
                            stage = 3;                        
                        }

                    }
                }
            }
        }
    }
    return 0;
}
