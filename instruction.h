#include<iostream>
using namespace std;
class Register{
	char* name;
	unsigned __int32 value;
	char* code;
public:
	Register(char* theName){
		name=new char[strlen(theName)+1];
		name=strcpy(name,theName);
		value=0;
		code=new char[6];
		if(strcmp(theName,"r0")==0){
			code=strcpy(code,"00000");
		}else if(strcmp(theName,"eax")==0){
			code=strcpy(code,"00001");
		}else if(strcmp(theName,"edx")==0){
			code=strcpy(code,"00010");
		}else if(strcmp(theName,"ecx")==0){
			code=strcpy(code,"00011");
		}else if(strcmp(theName,"esi")==0){
			code=strcpy(code,"00100");
		}else if(strcmp(theName,"esp")==0){
			code=strcpy(code,"01101");
		}else if(strcmp(theName,"ebp")==0){
			code=strcpy(code,"01110");
		}else{
			cout<<"regiser fail to initialize"<<endl;
		}
	}
	~Register(){
		if(name!=NULL){
			delete []name;
		}
		if(code!=NULL){
			delete []code;
		}
	}
	void show(){
		if(name!=NULL){
			cout<<name;
		}
		else{
			cout<<"register not initialized"<<endl;
		}
	}
	char* searchcode(){
		if(code!=NULL){
			return code;
		}
		else{
			cout<<"the register code is NULL"<<endl;
			return NULL;
		}
	}
	char* searchname(){
		return name;
	}
	void setValue(unsigned __int32 val){
		value=val;
	}
	unsigned __int32 searchValue(){
		return value;
	}
};
Register eax("eax"),edx("edx"),ecx("ecx"),esi("esi"),esp("esp"),ebp("ebp"),r0("r0");
Register* selectreg(char* reg){//根据字符串选择对应的寄存器
	if(strcmp(reg,"eax")==0){
		return &eax;
	}else if(strcmp(reg,"edx")==0){
		return &edx;
	}else if(strcmp(reg,"ecx")==0){
		return &ecx;
	}else if(strcmp(reg,"ebp")==0){
		return &ebp;
	}else if(strcmp(reg,"esi")==0){
		return &esi;
	}else if(strcmp(reg,"esp")==0){
		return &esp;
	}else if(strcmp(reg,"r0")==0){
		return &r0;
	}else{
		cout<<"no such register"<<endl;
		return NULL;
	}
}
int PC;//程序计数器
unsigned __int32 DM[25];//数据内存，这里只包含本排序程序所涉及数据
unsigned __int32 IntegerOutput,AddOutput,Mult1Output,DivOutput,
	IntegerInput1,IntegerInput2,AddInput1,AddInput2,Mult1Input1,Mult1Input2,DivInput1,DivInput2;
class instruction{
	char* opcode;
	void* oprand1;//r型指令与i型指令中oprand1是源操作数寄存器
	void* oprand2;//r型指令中oprand2是源操作数寄存器，i型指令中oprand2是目标寄存器
	void* oprand3;//r型指令中oprand2是目标寄存器，i型指令中oprand2是立即数
	int type1,type2,type3;
	char* machinecode;
public:
	instruction(){
		opcode=machinecode=NULL;
		oprand1=oprand2=oprand3=NULL;
		type1=type2=type3=0;//操作数类型，0表示未赋值，1表示register，2表示立即数
	}
	~instruction(){
		if(opcode!=NULL){
			delete []opcode;
		}
		if(machinecode!=NULL){
			delete []machinecode;
		}
	}
	bool setOpcode(char* theOpcode){
		if(opcode!=NULL){
			cout<<"opcode can't be modified"<<endl;
			return false;
		}
		opcode=new char[strlen(theOpcode)+1];
		bool checkopcode;
		if(strcmp(theOpcode,"add")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"adi")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"lw")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"sw")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"sub")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"mul")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"div")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"beq")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"slt")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"jr")==0){
			checkopcode=true;
		}else if(strcmp(theOpcode,"sll")==0){
			checkopcode=true;
		}else{
			checkopcode=false;
		}
		if(checkopcode){
			opcode=strcpy(opcode,theOpcode);
		}
		else{
			cout<<"the opcode is wrong."<<endl;
		}
		return checkopcode;
	}
	bool setOprand1(void* theOprand1,int type){
		if(oprand1!=NULL){
			cout<<"oprand1 can't be modified"<<endl;
			return false;
		}
		if(type==1){
			oprand1=(Register*)theOprand1;
			type1=1;
			return true;
		}else{
			cout<<"the oprand1 is wrong."<<endl;
			return false;
		}
	}
	bool setOprand2(void* theOprand2,int type){
		if(oprand2!=NULL){
			cout<<"the oprand2 can't be modified."<<endl;
			return false;
		}
		if(type==1){
			oprand2=(Register*)theOprand2;
			type2=1;
			return true;
		}else{
			cout<<"the oprand2 is wrong."<<endl;
			return false;
		}
	}
	bool setOprand3(void* theOprand3,int type){
		if(oprand3!=NULL){
			cout<<"the oprand3 can't be modified."<<endl;
			return false;
		}
		if(type==1){
			oprand3=(Register*)theOprand3;
			type3=1;
			return true;
		}else if(type==2){              //只有第3个操作数才有可能是立即数
			oprand3=(int*)theOprand3;
			type3=2;
			return true;
		}else{
			cout<<"the oprand3 is wrong."<<endl;
			return false;
		}
	}
	void print(){
		if(opcode==NULL||type1==0||type2==0){
			cout<<"instruction not completed"<<endl;
			return;
		}
		cout<<opcode<<"   ";
		((Register*)oprand1)->show();
		if(strcmp(opcode,"jr")==0){
			cout<<endl;
			return;
		}
		cout<<",";
		((Register*)oprand2)->show();
		cout<<",";
		if(type3==1){
			((Register*)oprand3)->show();
		}
		else{
			char s[6];
			s[5]='\0';
			s[4]='h';
			int i=*(int*)oprand3,index=3;
			while(i!=0){
				if((i%16)<10){
					s[index]=(char)((i%16)+'0');
				}
				else{
					s[index]=(char)((i%16-10)+'a');
				}
				i/=16;
				index--;
			}
			while(index>=0){
				s[index]='0';
				index--;
			}
			cout<<s;
		}
		cout<<endl;
	}
	void convert2machinecode(){//将汇编指令转换成机器码
		if(opcode==NULL||type1==0||type2==0){
			cout<<"the instruction cannot be converted to machinecode."<<endl;
			return;
		}
		machinecode=new char[33];
		if(type3==1){                       //寄存器指令编码
			if(strcmp(opcode,"add")==0){
				machinecode=strcpy(machinecode,"00110");
			}else if(strcmp(opcode,"sub")==0){
				machinecode=strcpy(machinecode,"01000");
			}else if(strcmp(opcode,"mul")==0){
				machinecode=strcpy(machinecode,"01001");
			}else if(strcmp(opcode,"div")==0){
				machinecode=strcpy(machinecode,"01010");
			}else if(strcmp(opcode,"slt")==0){
				machinecode=strcpy(machinecode,"10111");
			}else if(strcmp(opcode,"jr")==0){
				machinecode=strcpy(machinecode,"11011");
			}else{
				cout<<"instruction type is wrong"<<endl;
				return;
			}
			machinecode=strcat(machinecode,((Register*)oprand1)->searchcode());
			machinecode=strcat(machinecode,((Register*)oprand2)->searchcode());
			machinecode=strcat(machinecode,((Register*)oprand3)->searchcode());
			machinecode=strcat(machinecode,"000000000000");
		}
		else{                       //立即数指令编码
			if(strcmp(opcode,"adi")==0){
				machinecode=strcpy(machinecode,"00111");
			}else if(strcmp(opcode,"lw")==0){
				machinecode=strcpy(machinecode,"00000");
			}else if(strcmp(opcode,"sw")==0){
				machinecode=strcpy(machinecode,"00011");
			}else if(strcmp(opcode,"beq")==0){
				machinecode=strcpy(machinecode,"10101");
			}else if(strcmp(opcode,"sll")==0){
				machinecode=strcpy(machinecode,"10010");
			}else{
				cout<<"instruction type is wrong"<<endl;
				return;
			}
			machinecode=strcat(machinecode,((Register*)oprand1)->searchcode());
			machinecode=strcat(machinecode,((Register*)oprand2)->searchcode());
			if(strcmp(opcode,"sll")==0){//sll指令第3个操作数虽然是立即数，但按照寄存器指令进行编码
				machinecode=strcat(machinecode,r0.searchcode());
				char s[6];
				s[5]='\0';
				int i=*((int*)oprand3),index=4;
				while(i!=0){
					if(i%2==0){
						s[index]='0';
					}
					else{
						s[index]='1';
					}
					i/=2;
					index--;
				}
				while(index>=0){
					s[index]='0';
					index--;
				}
				machinecode=strcat(machinecode,s);
				machinecode=strcat(machinecode,"0000000");
				return;
			}
			char s[18];
			s[17]='\0';
			int i=*((int*)oprand3),index=16;
			while(i!=0){
				if(i%2==0){
					s[index]='0';
				}
				else{
					s[index]='1';
				}
				i/=2;
				index--;
			}
			while(index>=0){
				s[index]='0';
				index--;
			}
			machinecode=strcat(machinecode,s);
		}
	}
	char* searchmachinecode(){
		if(machinecode==NULL){
			cout<<"the machinecode is NULL"<<endl;
			return NULL;
		}
		else{
			return machinecode;
		}
	}
	int searchtype3(){
		return type3;
	}
	void* searchoprand3(){
		return oprand3;
	}
	void* searchoprand2(){
		return oprand2;
	}
	void* searchoprand1(){
		return oprand1;
	}
	char* searchopcode(){
		return opcode;
	}
};
instruction IM[200];//指令内存，只包含所有排序程序的指令