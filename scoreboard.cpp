#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include"Score_Board.h"
using namespace std;
void showsb(){
	cout<<"Instruction State:"<<endl;
	Iter=theScoreBoard.instbuf.begin();
	while(Iter!=theScoreBoard.instbuf.end()){
		switch(Iter->cur_state){
		case(1):
			cout<<"Issue        ";
			break;
		case(2):
			cout<<"Read Oprands ";
			break;
		case(3):
			cout<<"Execute      ";
			break;
		default:
			cout<<"Write Back   ";
			break;
		}
		IM[Iter->instaddr/4].print();
		Iter++;
	}
	cout<<endl<<"Function Unit:"<<endl<<"FU Name"<<setw(10)<<"Is_busy?"<<setw(10)<<"Op"<<setw(6)<<"Fi"<<setw(6)<<"Fj"<<setw(6)<<"Fk"<<setw(6)<<"Qj"<<setw(10)<<"Qk"<<setw(10)<<"Rj"<<setw(6)<<"Rk"<<endl;
	cout<<"Integer"<<setw(6)<<theScoreBoard.Integer.isBusy<<setw(14)<<(theScoreBoard.Integer.Op==NULL?"":theScoreBoard.Integer.Op)<<setw(6)<<(theScoreBoard.Integer.Fi==NULL?"":theScoreBoard.Integer.Fi->searchname())<<setw(6)<<
		(theScoreBoard.Integer.Fj==NULL?"":theScoreBoard.Integer.Fj->searchname())<<setw(6)<<(theScoreBoard.Integer.Fk==NULL?"":theScoreBoard.Integer.Fk->searchname())<<setw(6)<<
		(theScoreBoard.Integer.Qj==NULL?"":theScoreBoard.Integer.Qj->name)<<setw(10)<<(theScoreBoard.Integer.Qk==NULL?"":theScoreBoard.Integer.Qk->name)<<setw(10)<<
		theScoreBoard.Integer.Rj<<setw(6)<<theScoreBoard.Integer.Rk<<endl;
	cout<<"Mult1  "<<setw(6)<<theScoreBoard.Mult1.isBusy<<setw(14)<<(theScoreBoard.Mult1.Op==NULL?"":theScoreBoard.Mult1.Op)<<setw(6)<<(theScoreBoard.Mult1.Fi==NULL?"":theScoreBoard.Mult1.Fi->searchname())<<setw(6)<<
		(theScoreBoard.Mult1.Fj==NULL?"":theScoreBoard.Mult1.Fj->searchname())<<setw(6)<<(theScoreBoard.Mult1.Fk==NULL?"":theScoreBoard.Mult1.Fk->searchname())<<setw(6)<<
		(theScoreBoard.Mult1.Qj==NULL?"":theScoreBoard.Mult1.Qj->name)<<setw(10)<<(theScoreBoard.Mult1.Qk==NULL?"":theScoreBoard.Mult1.Qk->name)<<setw(10)<<
		theScoreBoard.Mult1.Rj<<setw(6)<<theScoreBoard.Mult1.Rk<<endl;
	cout<<"Add    "<<setw(6)<<theScoreBoard.Add.isBusy<<setw(14)<<(theScoreBoard.Add.Op==NULL?"":theScoreBoard.Add.Op)<<setw(6)<<(theScoreBoard.Add.Fi==NULL?"":theScoreBoard.Add.Fi->searchname())<<setw(6)<<
		(theScoreBoard.Add.Fj==NULL?"":theScoreBoard.Add.Fj->searchname())<<setw(6)<<(theScoreBoard.Add.Fk==NULL?"":theScoreBoard.Add.Fk->searchname())<<setw(6)<<
		(theScoreBoard.Add.Qj==NULL?"":theScoreBoard.Add.Qj->name)<<setw(10)<<(theScoreBoard.Add.Qk==NULL?"":theScoreBoard.Add.Qk->name)<<setw(10)<<
		theScoreBoard.Add.Rj<<setw(6)<<theScoreBoard.Add.Rk<<endl;
	cout<<"Div    "<<setw(6)<<theScoreBoard.Div.isBusy<<setw(14)<<(theScoreBoard.Div.Op==NULL?"":theScoreBoard.Div.Op)<<setw(6)<<(theScoreBoard.Div.Fi==NULL?"":theScoreBoard.Div.Fi->searchname())<<setw(6)<<
		(theScoreBoard.Div.Fj==NULL?"":theScoreBoard.Div.Fj->searchname())<<setw(6)<<(theScoreBoard.Div.Fk==NULL?"":theScoreBoard.Div.Fk->searchname())<<setw(6)<<
		(theScoreBoard.Div.Qj==NULL?"":theScoreBoard.Div.Qj->name)<<setw(10)<<(theScoreBoard.Div.Qk==NULL?"":theScoreBoard.Div.Qk->name)<<setw(10)<<
		theScoreBoard.Div.Rj<<setw(6)<<theScoreBoard.Div.Rk<<endl<<endl;
	cout<<"      eax"<<setw(6)<<"ecx"<<setw(6)<<"edx"<<setw(6)<<"esi"<<setw(6)<<"r0 "<<setw(6)<<"ebp"<<endl;
	cout<<"FU    "<<(theScoreBoard.RegStatus.pFUeax==NULL?"":theScoreBoard.RegStatus.pFUeax->name)<<setw(6)<<
		(theScoreBoard.RegStatus.pFUecx==NULL?"":theScoreBoard.RegStatus.pFUecx->name)<<setw(6)<<
		(theScoreBoard.RegStatus.pFUedx==NULL?"":theScoreBoard.RegStatus.pFUedx->name)<<setw(6)<<
		(theScoreBoard.RegStatus.pFUesi==NULL?"":theScoreBoard.RegStatus.pFUesi->name)<<setw(6)<<
		(theScoreBoard.RegStatus.pFUr0==NULL?"":theScoreBoard.RegStatus.pFUr0->name)<<setw(6)<<
		(theScoreBoard.RegStatus.pFUebp==NULL?"":theScoreBoard.RegStatus.pFUebp->name)<<endl;
	cout<<"value "<<eax.searchValue()<<setw(6)<<ecx.searchValue()<<setw(6)<<edx.searchValue()<<setw(6)<<esi.searchValue()<<setw(6)<<r0.searchValue()<<setw(9)<<ebp.searchValue()<<endl<<endl;
	cout<<"data  ";
	for(int i=1;i<=20;i++){
		cout<<DM[i]<<" ";
	}
	cout<<endl;
}
bool checkWB(Register* preg){
	list<inststatus>::iterator itr=theScoreBoard.instbuf.begin();
	while(itr!=Iter){
		if(itr->cur_state==1){
			if(strcmp(IM[itr->instaddr/4].searchopcode(),"lw")==0||strcmp(IM[itr->instaddr/4].searchopcode(),"sll")==0
				||strcmp(IM[itr->instaddr/4].searchopcode(),"adi")==0||strcmp(IM[itr->instaddr/4].searchopcode(),"jr")==0){
					if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand1())->searchname(),preg->searchname())==0){
						return false;
					}
			}else{
				if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand1())->searchname(),preg->searchname())==0
					||strcmp(((Register*)IM[itr->instaddr/4].searchoprand2())->searchname(),preg->searchname())==0){
						return false;
				}
			}
		}
		itr++;
	}
	return true;
}
bool checkRO(Register *preg1,Register *preg2){
	list<inststatus>::iterator itr=theScoreBoard.instbuf.begin();
	while(itr!=Iter){
		if(strcmp(IM[itr->instaddr/4].searchopcode(),"lw")==0||strcmp(IM[itr->instaddr/4].searchopcode(),"sll")==0
			||strcmp(IM[itr->instaddr/4].searchopcode(),"adi")==0){
				if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand2())->searchname(),preg1->searchname())==0){
					return false;
				}
				if(preg2!=NULL){
					if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand2())->searchname(),preg2->searchname())==0){
						return false;
					}
				}
		}else if(strcmp(IM[itr->instaddr/4].searchopcode(),"sw")!=0&&strcmp(IM[itr->instaddr/4].searchopcode(),"beq")!=0
			&&strcmp(IM[itr->instaddr/4].searchopcode(),"jr")!=0){
				if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand3())->searchname(),preg1->searchname())==0){
					return false;
				}
				if(preg2!=NULL){
					if(strcmp(((Register*)IM[itr->instaddr/4].searchoprand3())->searchname(),preg2->searchname())==0){
						return false;
					}
				}
		}
		itr++;
	}
	return true;
}
bool checkIS(){
	if(strcmp(IM[PC/4].searchopcode(),"adi")==0||strcmp(IM[PC/4].searchopcode(),"add")==0||
		strcmp(IM[PC/4].searchopcode(),"sub")==0){
			if(theScoreBoard.Add.isBusy){
				return false;
			}
	}else if(strcmp(IM[PC/4].searchopcode(),"mul")==0){
		if(theScoreBoard.Mult1.isBusy){
			return false;
		}
	}else if(strcmp(IM[PC/4].searchopcode(),"div")==0){
		if(theScoreBoard.Div.isBusy){
			return false;
		}
	}else{
		if(theScoreBoard.Integer.isBusy){
			return false;
		}
	}
	if(strcmp(IM[PC/4].searchopcode(),"lw")==0||strcmp(IM[PC/4].searchopcode(),"sll")==0||
		strcmp(IM[PC/4].searchopcode(),"adi")==0){
			if(theScoreBoard.Integer.Fi==(Register*)IM[PC/4].searchoprand2()
				||theScoreBoard.Mult1.Fi==(Register*)IM[PC/4].searchoprand2()
				||theScoreBoard.Add.Fi==(Register*)IM[PC/4].searchoprand2()
				||theScoreBoard.Div.Fi==(Register*)IM[PC/4].searchoprand2()){
					return false;
			}
	}else if(strcmp(IM[PC/4].searchopcode(),"sw")!=0&&strcmp(IM[PC/4].searchopcode(),"beq")!=0&&
		strcmp(IM[PC/4].searchopcode(),"jr")!=0){
			if(theScoreBoard.Integer.Fi==(Register*)IM[PC/4].searchoprand3()
				||theScoreBoard.Mult1.Fi==(Register*)IM[PC/4].searchoprand3()
				||theScoreBoard.Add.Fi==(Register*)IM[PC/4].searchoprand3()
				||theScoreBoard.Div.Fi==(Register*)IM[PC/4].searchoprand3()){
					return false;
			}
	}
	return true;
}
int main(){
	char opc[4],opr1[4],opr2[4],opr3[5];
	int imm[199],i,j,indexofinst=0;
	ifstream in("assemblycode.txt");//汇编指令保存在该txt文件中
	string s;
	PC=0;
	ebp.setValue(1000);
	while(getline(in,s)){//一行一行读取汇编指令，写入指令内存中
		for(i=0;s[i]!=' ';i++){
			opc[i]=s[i];
		}
		opc[i]='\0';
		for(i=4;s[i]!=',';i++){
			opr1[i-4]=s[i];
		}
		opr1[i-4]='\0';
		for(i=i+1,j=0;s[i]!=',';i++,j++){
			opr2[j]=s[i];
		}
		opr2[j]='\0';
		for(i=i+1,j=0;s[i]!='h'&&s[i]!='\0';i++,j++){
			opr3[j]=s[i];
		}
		opr3[j]='\0';
		if(s[i]=='h'){
			imm[indexofinst]=0;
			for(int k=0;opr3[k]!='\0';k++){
				if((int)opr3[k]<'a'){
					imm[indexofinst]=imm[indexofinst]*16+((int)opr3[k]-'0');
				}
				else{
					imm[indexofinst]=imm[indexofinst]*16+((int)opr3[k]-'a'+10);
				}
			}
			if(IM[indexofinst].setOpcode(opc)&&IM[indexofinst].setOprand1(selectreg(opr1),1)&&IM[indexofinst].setOprand2(selectreg(opr2),1)&&IM[indexofinst].setOprand3(&imm[indexofinst],2)){
				IM[indexofinst].convert2machinecode();//转换成机器码
				indexofinst++;
			}
		}
		else{
			if(IM[indexofinst].setOpcode(opc)&&IM[indexofinst].setOprand1(selectreg(opr1),1)&&IM[indexofinst].setOprand2(selectreg(opr2),1)&&IM[indexofinst].setOprand3(selectreg(opr3),1)){
				IM[indexofinst].convert2machinecode();//转换成机器码
				indexofinst++;
			}
		}
	}
	in.close();
	theScoreBoard.initialize();
	Issue();
	showsb();
	cout<<"下一步调试";
	system("pause");
	cout<<endl;
	while(!theScoreBoard.instbuf.empty()){
		Iter=theScoreBoard.instbuf.begin();
		while(Iter!=theScoreBoard.instbuf.end()){
			if(Iter->cur_state==3){
				if(strcmp(IM[Iter->instaddr/4].searchopcode(),"beq")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"jr")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"sw")==0){
					if(strcmp(IM[Iter->instaddr/4].searchopcode(),"sw")==0){
						list<inststatus>::iterator it=Iter;
						Iter++;
						Write_back(it);
					}else{
						Write_back(Iter);
						Iter=theScoreBoard.instbuf.begin();
					}
					continue;
				}else{
					if(strcmp(IM[Iter->instaddr/4].searchopcode(),"lw")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"sll")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"adi")==0){
						if(checkWB(((Register*)IM[Iter->instaddr/4].searchoprand2()))){
							list<inststatus>::iterator it=Iter;
							Iter++;
							Write_back(it);
							continue;
						}
					}else{
						if(checkWB(((Register*)IM[Iter->instaddr/4].searchoprand3()))){
							list<inststatus>::iterator it=Iter;
							Iter++;
							Write_back(it);
							continue;
						}
					}
				}
			}else if(Iter->cur_state==2){
				Execute(Iter->instaddr);
			}else{
				if(strcmp(IM[Iter->instaddr/4].searchopcode(),"lw")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"sll")==0
					||strcmp(IM[Iter->instaddr/4].searchopcode(),"adi")==0||strcmp(IM[Iter->instaddr/4].searchopcode(),"jr")==0){
						if(checkRO((Register*)IM[Iter->instaddr/4].searchoprand1(),NULL)){
							Read_oprands(Iter->instaddr);
						}
				}else{
					if(checkRO((Register*)IM[Iter->instaddr/4].searchoprand1(),(Register*)IM[Iter->instaddr/4].searchoprand2())){
							Read_oprands(Iter->instaddr);
						}
				}
			}
			Iter++;
		}
		if(PC!=796&&(checkIS())){
			Issue();
		}
		showsb();
			cout<<"下一步调试";
		    system("pause");
			cout<<endl;
	}
	theScoreBoard.Integer.destroy();
	theScoreBoard.Mult1.destroy();
	theScoreBoard.Mult2.destroy();
	theScoreBoard.Add.destroy();
	theScoreBoard.Div.destroy();
	system("pause");
	return 0;
}