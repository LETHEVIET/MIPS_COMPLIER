#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

enum Command_Format{
    r,
    i,
    j,
};

map<string, Command_Format> Command{
    //R Format
    {"add", r},
    {"addu", r},
    {"and", r},
    {"jr", r},
    //I Format
    {"addi", i},
    {"addiu", i},
    {"andi", i},
    {"beq", i},
    {"bne", i},
    {"lbu", i},
    {"lhu", i},
    {"lui", i},
    {"lw", i},
    {"sw", i},
    //J Format
    {"j", j},
    {"jal", j},
};

map<string, string> Register{
    {"$0", "00000"},
	{"$zero", "00000"},
	{"$at", "00001"},
	{"$v0", "00010"},
	{"$v1", "00011"},
	{"$a0", "00100"},
	{"$a1", "00101"},
	{"$a2", "00110"},
	{"$a3", "00111"},
	{"$t0", "01000"},
	{"$t1", "01001"},
	{"$t2", "01010"},
	{"$t3", "01011"},
	{"$t4", "01100"},
	{"$t5", "01101"},
	{"$t6", "01110"},
	{"$t7", "01111"},
	{"$s0", "10000"},
	{"$s1", "10001"},
	{"$s2", "10010"},
	{"$s3", "10011"},
	{"$s4", "10100"},
	{"$s5", "10101"},
	{"$s6", "10110"},
	{"$s7", "10111"},
	{"$t8", "11000"},
	{"$t9", "11001"},
	{"$k0", "11010"},
	{"$k1", "11011"},
	{"$gp", "11100"},
	{"$sp", "11101"},
	{"$fp", "11110"},
	{"$ra", "11111"},
    
	{"$1", "00001"},
	{"$2", "00010"},
	{"$3", "00011"},
	{"$4", "00100"},
	{"$5", "00101"},
	{"$6", "00110"},
	{"$7", "00111"},
	{"$8", "01000"},
	{"$9", "01001"},
	{"$10", "01010"},
	{"$11", "01011"},
	{"$12", "01100"},
	{"$13", "01101"},
	{"$14", "01110"},
	{"$15", "01111"},
	{"$16", "10000"},
	{"$17", "10001"},
	{"$18", "10010"},
	{"$19", "10011"},
	{"$20", "10100"},
	{"$21", "10101"},
	{"$22", "10110"},
	{"$23", "10111"},
	{"$24", "11000"},
	{"$25", "11001"},
	{"$26", "11010"},
	{"$27", "11011"},
	{"$28", "11100"},
	{"$29", "11101"},
	{"$30", "11110"},
	{"$31", "11111"},
};   

map<string, string> FunctionR{
	{"add", "100000"},
	{"and", "100100"},
	{"or", "100101"},
	{"nor", "100111"},
	{"slt", "101010"},
	{"sll", "000000"},
	{"srl", "000010"},
	{"sub", "100010"},
	{"addu", "100001"},
	{"jr", "001000"},
};

map<string, string> OpcodeI{
	{"addi", "001000"},
	{"addiu", "001001"},
	{"andi", "001100"},
	{"lw", "100011"},
	{"sw", "101011"},
	{"lbu", "100100"},
	{"lhu", "100101"},
	{"lui", "001111"},
	{"beq", "000100"},
	{"bne", "000101"},
};

map<string, int> Label;

string _trim(string s){
    int foundsharp = s.find('#');
    if (foundsharp != string::npos) s.erase(s.begin()+foundsharp,s.end());

    while (s[0]==' ') s.erase(s.begin());
    int i=0;
    while (i<s.size())
    {
        if (s[i]==' '&&s[i+1]==' ') s.erase(s.begin()+i), i--;
        i++;
    }
    if (s[s.size()-1]==' ') s.erase(s.begin()+s.size()-1);
    for (int i=0; i<s.size(); ++i)
        if (s[i]==' ' || s[i] == ':') s[i]=',';
    i=0;
    while (i<s.size())
    {
        if (s[i]==','&&s[i+1]==',') s.erase(s.begin()+i), i--;
        i++;
    }
    
    int l = s.length(); 
	int c = count(s.begin(), s.end(), '\t');
	remove(s.begin(), s.end(), '\t');
	s.resize(l - c);

    return s;
}

int base = 4194304;
int _lines_to_address(int index){
	return index * 4 + base;
}

void _read_file(vector<string> &lines){
    freopen ("input.txt", "r", stdin);
    string line;
	getline(cin,line);
    while (getline(cin,line)) lines.push_back(line);
}

void _print_data(const vector<string> &data){
    for (string line:data) cout<< line << endl;
}

void _remove_comment(vector<string> &data){
    vector<string> result;
    for (string line:data){
        if (_trim(line).size()!=0) result.push_back(_trim(line));
    }
	data = result;
	result.resize(0);
	string preS = "";
	int i = 0;
	for (string line:data){
		string fsyntax = "";
        for (char c:line){
            if (c == ',') break;
            fsyntax.push_back(c);
        }
        if (Command.count(fsyntax) == 0 && line.size()-1 == fsyntax.size() && i != data.size() - 1){
			preS += fsyntax + ',';
		}else{
			result.push_back(preS + line);
			preS = "";
		}
		i++;
	}

    data = result;
}

void _calculate_address_for_label(vector<string> &data){
	vector<string> result;
    int lineCount = 0;
    for (int i = 0; i < data.size(); i++){
        string fsyntax = "";
        for (char c:data[i]){
            if (c == ',') break;
            fsyntax.push_back(c);
        }

        if (Command.count(fsyntax) == 0){
            Label[fsyntax] = i;
            data[i].erase(0, fsyntax.size()+1);
        }
    }
}

void _pass1(vector<string> &data){
    _remove_comment(data);

    _calculate_address_for_label(data);
}

void R2Bin(string code, string &res, string temp){
	string Op = "000000";
	string rs = "00000";
	string rt = "00000";
	string rd = "00000";
	string shamt = "00000";
	string funct = "000000";
	funct = FunctionR[temp];
	
	vector<string> Reg;
	Reg.resize(3);
	int count = 0;
	for(int i=0;i<code.length();i++){
		if(code[i] == '$'){
			for(int j=i+1;j<=code.length();j++){
				if(code[j] == ',' || code[j] == ' '||j == code.length()){
					//cout << code.substr(i, j-i) << endl;
					//Reg.push_back(code.substr(i, j-i));
					Reg[count] = code.substr(i, j-i);
					count++;
					//cout << i << " " << j << endl;
					i = j+1;
				}
			}
		}
	}
    /*
	for(int i=0;i<3;i++){
		cout << Reg[i] << endl;
	}
    */
	rd = Register[(Reg[0])];
	rs = Register[(Reg[1])];
	rt = Register[(Reg[2])];
	if(temp == "jr"){
		rs = Register[(Reg[0])];
		rd = "00000";
		rt = "00000";
	}
	res = Op + rs + rt + rd + shamt + funct;
}

void I2Bin(string code, string &res, string temp, int index){
	string Op = "000000";
	string rs = "00000";
	string rt = "00000";
	string Imm = "0000000000000000";

	Op = OpcodeI[temp];
	
	vector<string> Reg;
	Reg.resize(3);
	int count = 0;
	//Lw, Sw
	if(temp == "sw"||temp == "lw"||temp == "lbu"||temp == "lhu"){
		//Register
		for(int i=0;i<code.length();i++){
			if(code[i] == '$'){
				for(int j=i+1;j<code.length();j++){
					if(code[j] == ',' || code[j] == ')'){
						//cout << code.substr(i, j-i) << endl;
						//Reg.push_back(code.substr(i, j-i));
						Reg[count] = code.substr(i, j-i);
						count++;
						//cout << i << " " << j << endl;
						i = j+1;
						break;
					}
				}
			}
		}
		//Imm
		for(int i=0;i<code.length();i++){
			if(code[i] == '('){
				for(int j=i;j>0;j--){
					if(code[j]==','){
					Reg[count] = code.substr(j+1, i-j-1);
					count++;	
					break;
					}
				}
			}
		}
		stringstream number(Reg[2]);
		int x = 0;
   		number >> x;
   		//dec2Bin(x, Imm, doDai);
   		Imm = bitset<16>(x).to_string();
        /*
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
        */
		rt = Register[(Reg[0])];
		rs = Register[(Reg[1])];
		//cout << Imm;
	}//LENH ADDI ANDI
	else if(temp == "addi"||temp == "andi"||temp == "addiu"){
		//Register
		for(int i=0;i<code.length();i++){
			if(code[i] == '$'){
				for(int j=i+1;j<code.length();j++){
					if(code[j] == ','){
						//cout << code.substr(i, j-i) << endl;
						//Reg.push_back(code.substr(i, j-i));
						Reg[count] = code.substr(i, j-i);
						count++;
						//cout << i << " " << j << endl;
						i = j+1;
					}
				}
			}
		}
		for(int i=code.length();i>=0;i--){
			if(code[i]==','){
				Reg[count] = code.substr(i+1, code.length());
				count++;
				break;	
			}
		}
		
		stringstream number(Reg[2]);
		int x = 0;
   		number >> x;
   		//dec2Bin(x, Imm, doDai);
   		Imm = bitset<16>(x).to_string();
        /*
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
        */
		rt = Register[(Reg[0])];
		rs = Register[(Reg[1])];
	}
	else if(temp == "lui"){
		//Register
		for(int i=0;i<code.length();i++){
			if(code[i] == '$'){
				for(int j=i+1;j<code.length();j++){
					if(code[j] == ','){
						//cout << code.substr(i, j-i) << endl;
						//Reg.push_back(code.substr(i, j-i));
						Reg[count] = code.substr(i, j-i);
						count++;
						//cout << i << " " << j << endl;
						i = j+1;
					}
				}
			}
		}
		for(int i=code.length();i>=0;i--){
			if(code[i]==','){
				Reg[count] = code.substr(i+1, code.length());
				count++;
				break;	
			}
		}
		stringstream number(Reg[1]);
		int x = 0;
   		number >> x;
   		//dec2Bin(x, Imm, doDai);
   		Imm = bitset<16>(x).to_string();
		/*
        for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
        */
		rt = Register[(Reg[0])];
		//cout << Imm << endl;
	}
	else if(temp == "beq"||temp == "bne"){
		//Register
		for(int i=0;i<code.length();i++){
			if(code[i] == '$'){
				for(int j=i+1;j<code.length();j++){
					if(code[j] == ','){
						//cout << code.substr(i, j-i) << endl;
						//Reg.push_back(code.substr(i, j-i));
						Reg[count] = code.substr(i, j-i);
						count++;
						//cout << i << " " << j << endl;
						i = j+1;
					}
				}
			}
		}
		for(int i=code.length();i>=0;i--){
			if(code[i]==','){
				Reg[count] = code.substr(i+1, code.length());
				count++;
				break;	
			}
		}
		//Xu li
		//GIA SU VI TRI HIEN TAI CUA LENH BNE LA 0x20(5), EXIT la 0x28
		string PASS_tru_PC_cong4;
		if(Label.count(Reg[2])!=0){
			int viTriLenh = index;//PC
			int viTriNhan = Label[(Reg[2])];//PASS
			int DoDoi = (viTriNhan - viTriLenh-1)*4;//extS = PASS-PC-4
			cout<< code <<": " <<  viTriNhan << " " << viTriLenh << " " << (viTriNhan - viTriLenh-1) << endl;
			PASS_tru_PC_cong4 = bitset<16>(DoDoi).to_string();
		}
		else{
			stringstream number(Reg[2]);
			int x = 0;
			number >> x;
			PASS_tru_PC_cong4 = bitset<16>(x).to_string();
		}
		
   		//dec2Bin(x, Imm, doDai);
   	
   		Imm = "00" + PASS_tru_PC_cong4.substr(0, 14);
		/*
        for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
        */
		rs = Register[(Reg[0])];
		rt = Register[(Reg[1])];
		 
	}
	res = Op + rs + rt + Imm;
}

void J2Bin(string code, string &res, string temp){
	string Op = "000000";
	string address = "00000000000000000000000000";
	string PC = "00000000000000000000000000000000";
	
	map<string, string> OpcodeJ;
	OpcodeJ["j"] = "000010";
	OpcodeJ["jal"] = "000011";
	
	Op = OpcodeJ[temp];

	string label;
	//Adress
	for(int i=code.length()-1;i>=0;i--){
		if(code[i]==',') break;
	    label.push_back(code[i]);
	}
	reverse(label.begin(), label.end());
	if(Label.count(label) != 0){
		int viTriNhan = Label[(label)];
		int DoDoi = _lines_to_address(viTriNhan);
		PC = bitset<32>(DoDoi).to_string();
	}else{
		stringstream number(label);
		long long x = 0;
   		number >> x;
		x += base;
   		PC = bitset<32>(x).to_string();
	}
	
   	address = PC.substr(4, 26);
	
	res = Op + address;
}

void _pass2(vector<string> &data, vector<string> &result){
    vector<string> res;
    int index = 0;
    for (string line: data) if (line.size() != 0){
        string binaryS = "";
        string fsyntax = "";
        for (char c:line){
            if (c == ',') break;
            fsyntax.push_back(c);
        }
        //cout<<"Converting " + line + " ..." << endl;
        switch(Command[fsyntax]){
            case (r):
                R2Bin(line, binaryS, fsyntax);
                break;
            case (i):
                I2Bin(line, binaryS, fsyntax, index);
                break;
            case (j):
                J2Bin(line, binaryS, fsyntax);
                break;
        }
        //cout<< binaryS << endl;
        res.push_back(binaryS);
        index ++;
    }
    result = res;
}

int main(){
    vector<string> mips_code;
    vector<string> binary_code;
    _read_file(mips_code);
    _pass1(mips_code);

    /*DEBUG*/{
        cout<< "mips code" << endl;
        _print_data(mips_code);
		cout<< endl;
        cout<< "label address" << endl;
        for (map<string, int> :: iterator it = Label.begin(); it != Label.end(); it++){
            cout<< it->first << " " << _lines_to_address(it->second) << endl;
        }
		cout<< endl;

		cout<< "line address" << endl;
		for (int i = 0; i < mips_code.size(); i++){
			cout<<_lines_to_address(i)<< '\t' << mips_code[i] << endl;
		}
		cout<< endl;
    }

    _pass2(mips_code, binary_code);

    /*DEBUG*/{
        cout<< "binary code" << endl;
        _print_data(binary_code);
    }
    
    freopen("binary.out", "w", stdout);
    _print_data(binary_code);
    return 0;
}
