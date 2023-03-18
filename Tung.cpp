#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <fstream>
#include <map>

using namespace std;

//THANH GHI
string Register(string temp){
	map<string, string> Register;
	Register["$0"] = "00000";
	Register["$zero"] = "00000";
	Register["$at", "$1"] = "00001";
	Register["$v0", "$2"] = "00010";
	Register["$v1", "$3"] = "00011";
	Register["$a0", "$4"] = "00100";
	Register["$a1", "$5"] = "00101";
	Register["$a2", "$6"] = "00110";
	Register["$a3", "$7"] = "00111";
	Register["$t0", "$8"] = "01000";
	Register["$t1", "$9"] = "01001";
	Register["$t2", "$10"] = "01010";
	Register["$t3", "$11"] = "01011";
	Register["$t4", "$12"] = "01100";
	Register["$t5", "$13"] = "01101";
	Register["$t6", "$14"] = "01110";
	Register["$t7", "$15"] = "01111";
	Register["$s0", "$16"] = "10000";
	Register["$s1", "$17"] = "10001";
	Register["$s2", "$18"] = "10010";
	Register["$s3", "$19"] = "10011";
	Register["$s4", "$20"] = "10100";
	Register["$s5", "$21"] = "10101";
	Register["$s6", "$22"] = "10110";
	Register["$s7", "$23"] = "10111";
	Register["$t8", "$24"] = "11000";
	Register["$t9", "$25"] = "11001";
	Register["$k0", "$26"] = "11010";
	Register["$k1", "$27"] = "11011";
	Register["$gp", "$28"] = "11100";
	Register["$sp", "$29"] = "11101";
	Register["$fp", "$30"] = "11110";
	Register["$ra", "$31"] = "11111";
	return Register[temp];
}
int Label(string temp){
	map<string, int> Label;
	Label["Loop"] = 2;
	Label["Exit"] = 7;
	return Label[temp];
}
//CHECK LENH
void checkType(string temp, int &typeR, int &typeI, int &typeJ){
	if(temp == "add"||temp == "addu"||temp == "jr"||temp == "and"||temp == "or"||temp == "nor"||temp == "slt"||temp == "sll"||temp == "srl"||temp == "sub"){
		typeR = 1;
		return;
	}
	else if(temp == "addi"||temp == "andi"||temp == "subi"||temp == "beq"||temp == "lui"||temp == "bne"||temp == "lw"||temp == "sw"||temp == "lbu"||temp == "lhu"){
		typeI = 1;
		return;
	}
	else if(temp == "j"||temp == "jal"){
		typeJ = 1;
		return;
	}
}
//LENH DINH DANG R
void R2Bin(string code, string &res, string temp){
	string Op = "000000";
	string rs = "00000";
	string rt = "00000";
	string rd = "00000";
	string shamt = "00000";
	string funct = "000000";
	//FUNCTION
	map<string, string> FunctionR;
	FunctionR["add"] = "100000";
	FunctionR["and"] = "100100";
	FunctionR["or"] = "100101";
	FunctionR["nor"] = "100111";
	FunctionR["slt"] = "101010";
	FunctionR["sll"] = "000000";
	FunctionR["srl"] = "000010";
	FunctionR["sub"] = "100010";
	FunctionR["addu"] = "100001";
	FunctionR["jr"] = "001000";
	funct = FunctionR[temp];
	//THANH GHI
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
	for(int i=0;i<3;i++){
		cout << Reg[i] << endl;
	}
	rd = Register(Reg[0]);
	rs = Register(Reg[1]);
	rt = Register(Reg[2]);
	if(temp == "jr"){
		rs = rd;
		rd = "00000";
	}
	res = Op + rs + rt + rd + shamt + funct;
}
// DEC TO BIN
//void dec2Bin(int n, string &Imm, int &count)
//{
//    int binaryNum[32];
//    int i = 0;
//    while (n > 0) {
//        binaryNum[i] = n % 2;
//        n = n / 2;
//        i++;
//    }
////    for(int j = i - 1; j >= 0; j--){
////    	cout << binaryNum[j];
////	}
//	for(int j=0;j<i;j++){
//    	cout << binaryNum[j];
//    	Imm[count] = binaryNum[j] + '0';
//    	count--;
//	}
//}
//LENH DINH DANG I
void I2Bin(string code, string &res, string temp){
	string Op = "000000";
	string rs = "00000";
	string rt = "00000";
	string Imm = "0000000000000000";
	//FUNCTION
	map<string, string> OpcodeI;
	OpcodeI["addi"] = "001000";
	OpcodeI["addiu"] = "001001";
	OpcodeI["andi"] = "001100";
	OpcodeI["lw"] = "100011";
	OpcodeI["sw"] = "101011";
	OpcodeI["lbu"] = "100100";
	OpcodeI["lhu"] = "100101";
	OpcodeI["lui"] = "001111";
	OpcodeI["beq"] = "000100";
	OpcodeI["bne"] = "000101";
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
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
		rt = Register(Reg[0]);
		rs = Register(Reg[1]);
		//cout << Imm;
	}//LENH ADDI ANDI
	else if(temp == "addi"||temp == "andi"){
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
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
		rt = Register(Reg[0]);
		rs = Register(Reg[1]);
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
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
		rt = Register(Reg[0]);
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
		if(Label(Reg[2])!=0){
			int viTriLenh = 4;//PC
			int viTriNhan = Label(Reg[2]);//PASS
			int DoDoi = (viTriNhan - viTriLenh-1)*4;//extS = PASS-PC-4
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
		for(int i=0;i<3;i++){
			cout << Reg[i] << endl;
		}
		rs = Register(Reg[0]);
		rt = Register(Reg[1]);
		 
	}
	res = Op + rs + rt + Imm;
}
//LENH DINH DANG J
void J2Bin(string code, string &res, string temp){
	string Op = "000000";
	string address = "00000000000000000000000000";
	string PC = "00000000000000000000000000000000";
	
	map<string, string> OpcodeJ;
	OpcodeJ["j"] = "000010";
	OpcodeJ["jal"] = "000011";
	
	Op = OpcodeJ[temp];
	
	vector<string> Reg;
	Reg.resize(1);
	int count = 0;
	//Adress
	for(int i=code.length();i>=0;i--){
			if(code[i]==','){
				Reg[count] = code.substr(i+1, code.length());
				count++;
				break;	
			}
	}
	//GIA SU lENH J LOOP DANG O VI TRI 0x24(6), NHAN LOOP O VI TRI 2
	cout <<Label(Reg[0]) <<endl;
	if(Label(Reg[0])!=0){
		int viTriNhan = Label(Reg[0]);
		int DoDoi = viTriNhan*4;
		PC = bitset<32>(DoDoi).to_string();
	}else{
		stringstream number(Reg[0]);
		long long x = 0;
   		number >> x;
   		PC = bitset<32>(x).to_string();
	}
	
   	//dec2Bin(x, Imm, doDai);
   	
   	address = PC.substr(4, 26);
	
	//cout << Reg[0];
	res = Op + address;
}
//CAT LENH DE CHECK XEM LENH DO THUOC DINH DANG GI
void Mips2Bin(string code, string &res){
	int flag = 0;
	string temp;
	int typeR = 0;
	int typeI = 0;
	int typeJ = 0;
	for(int i=0;i<=code.length();i++){
		if(code[i] == ','){
			temp = code.substr(flag, i-flag);
			cout << temp <<endl;
			break;
		}
	}
	checkType(temp, typeR, typeI, typeJ);
	//cout << typeR << " " << typeI << " " << typeJ;
	if(typeR == 1){
		R2Bin(code, res, temp);
	}
	else if(typeI == 1){
		I2Bin(code, res, temp);
	}
	else if(typeJ == 1){
		J2Bin(code, res, temp);
	}
}
int main(){
	string code = "j,8";
	//string code = "bne,$t0,$s5,Exit";
	//cout << code;
	string res;
	//cout << code.substr(4, 7);
	Mips2Bin(code, res);
	cout << res;
	return 0;
}
