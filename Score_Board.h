#include<iostream>
#include<list>
#include<algorithm>
#include"instruction.h"
struct inststatus{//指令的状态
	int instaddr;
	int cur_state;//指令所处状态,1表示Issue，2表示Read_oprands，3表示Execute，4表示Write_back
};
struct FU{//功能单元状态
	char *name;
	bool isBusy;
	char *Op;
	Register *Fi,*Fj,*Fk;
	FU *Qj,*Qk;
	bool Rj,Rk;
	void initFU(char *FUname){
		name=new char[strlen(FUname)+1];
		strcpy(name,FUname);
		isBusy=false;
		Rj=Rk=true;
		Op=NULL;
		Fi=Fj=Fk=NULL;
		Qj=Qk=NULL;
	}
	void destroy(){
		delete []name;
		if(Op!=NULL){
			delete []Op;
		}
	}
};
struct reg_status{
	FU *pFUeax,*pFUebp,*pFUecx,*pFUedx,*pFUesi,*pFUr0;
	void initRegStatus(){
		pFUeax=pFUebp=pFUecx=pFUedx=pFUesi=pFUr0=NULL;
	}
	void setFU(Register* preg,FU* pFU){
		if(strcmp(preg->searchname(),"eax")==0){
			pFUeax=pFU;
		}else if(strcmp(preg->searchname(),"ecx")==0){
			pFUecx=pFU;
		}else if(strcmp(preg->searchname(),"edx")==0){
			pFUedx=pFU;
		}else if(strcmp(preg->searchname(),"esi")==0){
			pFUesi=pFU;
		}
	}
	FU* getFU(Register* preg){
		if(strcmp(preg->searchname(),"eax")==0){
			return pFUeax;
		}else if(strcmp(preg->searchname(),"ecx")==0){
			 return pFUecx;
		}else if(strcmp(preg->searchname(),"edx")==0){
			return pFUedx;
		}else if(strcmp(preg->searchname(),"esi")==0){
			return pFUesi;
		}else{
			return NULL;
		}
	}
};
struct score_board{
	list<inststatus> instbuf;//当前正在计分板中的指令
	FU Integer,Mult1,Mult2,Add,Div;
	reg_status RegStatus;
	void initialize(){
		Integer.initFU("Integer");
		Mult1.initFU("Mult1");
		Mult2.initFU("Mult2");
		Add.initFU("Add");
		Div.initFU("Div");
		RegStatus.initRegStatus();
	}
};
list<inststatus>::iterator Iter;
score_board theScoreBoard;
void Issue(){//发射阶段
	inststatus newinst;
	newinst.cur_state=1;
	newinst.instaddr=PC;
	theScoreBoard.instbuf.push_back(newinst);
	if(strcmp(IM[PC/4].searchopcode(),"mul")==0){
		theScoreBoard.Mult1.isBusy=true;
		if(theScoreBoard.Mult1.Op==NULL){theScoreBoard.Mult1.Op=new char[4];}
		strcpy(theScoreBoard.Mult1.Op,IM[PC/4].searchopcode());
		theScoreBoard.Mult1.Fi=(Register*)IM[PC/4].searchoprand3();
		theScoreBoard.Mult1.Fj=(Register*)IM[PC/4].searchoprand1();
		theScoreBoard.Mult1.Fk=(Register*)IM[PC/4].searchoprand2();
		theScoreBoard.Mult1.Qj=theScoreBoard.RegStatus.getFU(theScoreBoard.Mult1.Fj);
		theScoreBoard.Mult1.Qk=theScoreBoard.RegStatus.getFU(theScoreBoard.Mult1.Fk);
		theScoreBoard.Mult1.Rj=(theScoreBoard.Mult1.Qj==NULL);
		theScoreBoard.Mult1.Rk=(theScoreBoard.Mult1.Qk==NULL);
		theScoreBoard.RegStatus.setFU(theScoreBoard.Mult1.Fi,&theScoreBoard.Mult1);
	}else if(strcmp(IM[PC/4].searchopcode(),"add")==0||strcmp(IM[PC/4].searchopcode(),"sub")==0||strcmp(IM[PC/4].searchopcode(),"adi")==0){
		theScoreBoard.Add.isBusy=true;
		if(theScoreBoard.Add.Op==NULL){theScoreBoard.Add.Op=new char[4];}
		strcpy(theScoreBoard.Add.Op,IM[PC/4].searchopcode());
		if(strcmp(IM[PC/4].searchopcode(),"adi")==0){
			theScoreBoard.Add.Fi=(Register*)IM[PC/4].searchoprand2();
			theScoreBoard.Add.Fj=(Register*)IM[PC/4].searchoprand1();
			theScoreBoard.Add.Qj=theScoreBoard.RegStatus.getFU(theScoreBoard.Add.Fj);
			theScoreBoard.Add.Rj=(theScoreBoard.Add.Qj==NULL);
		}else{
			theScoreBoard.Add.Fi=(Register*)IM[PC/4].searchoprand3();
			theScoreBoard.Add.Fj=(Register*)IM[PC/4].searchoprand1();
			theScoreBoard.Add.Fk=(Register*)IM[PC/4].searchoprand2();
			theScoreBoard.Add.Qj=theScoreBoard.RegStatus.getFU(theScoreBoard.Add.Fj);
			theScoreBoard.Add.Qk=theScoreBoard.RegStatus.getFU(theScoreBoard.Add.Fk);
			theScoreBoard.Add.Rj=(theScoreBoard.Add.Qj==NULL);
			theScoreBoard.Add.Rk=(theScoreBoard.Add.Qk==NULL);
		}
		theScoreBoard.RegStatus.setFU(theScoreBoard.Add.Fi,&theScoreBoard.Add);
	}else if(strcmp(IM[PC/4].searchopcode(),"div")==0){
		theScoreBoard.Div.isBusy=true;
		if(theScoreBoard.Div.Op==NULL){theScoreBoard.Div.Op=new char[4];}
		strcpy(theScoreBoard.Div.Op,IM[PC/4].searchopcode());
		theScoreBoard.Div.Fi=(Register*)IM[PC/4].searchoprand3();
		theScoreBoard.Div.Fj=(Register*)IM[PC/4].searchoprand1();
		theScoreBoard.Div.Fk=(Register*)IM[PC/4].searchoprand2();
		theScoreBoard.Div.Qj=theScoreBoard.RegStatus.getFU(theScoreBoard.Div.Fj);
		theScoreBoard.Div.Qk=theScoreBoard.RegStatus.getFU(theScoreBoard.Div.Fk);
		theScoreBoard.Add.Rj=(theScoreBoard.Div.Qj==NULL);
		theScoreBoard.Add.Rk=(theScoreBoard.Div.Qk==NULL);
		theScoreBoard.RegStatus.setFU(theScoreBoard.Div.Fi,&theScoreBoard.Div);
	}else{
		theScoreBoard.Integer.isBusy=true;
		if(theScoreBoard.Integer.Op==NULL){theScoreBoard.Integer.Op=new char[4];}
		strcpy(theScoreBoard.Integer.Op,IM[PC/4].searchopcode());
		theScoreBoard.Integer.Fj=(Register*)IM[PC/4].searchoprand1();
		theScoreBoard.Integer.Qj=theScoreBoard.RegStatus.getFU(theScoreBoard.Integer.Fj);
		theScoreBoard.Integer.Rj=(theScoreBoard.Integer.Qj==NULL);
		if(strcmp(IM[PC/4].searchopcode(),"lw")==0||strcmp(IM[PC/4].searchopcode(),"sll")==0){
			theScoreBoard.Integer.Fi=(Register*)IM[PC/4].searchoprand2();
			theScoreBoard.RegStatus.setFU(theScoreBoard.Integer.Fi,&theScoreBoard.Integer);
		}
		if(strcmp(IM[PC/4].searchopcode(),"slt")==0){
			theScoreBoard.Integer.Fi=(Register*)IM[PC/4].searchoprand3();
			theScoreBoard.Integer.Fk=(Register*)IM[PC/4].searchoprand2();
			theScoreBoard.Integer.Qk=theScoreBoard.RegStatus.getFU(theScoreBoard.Integer.Fk);
			theScoreBoard.Integer.Rk=(theScoreBoard.Integer.Qk==NULL);
			theScoreBoard.RegStatus.setFU(theScoreBoard.Integer.Fi,&theScoreBoard.Integer);
		}
		if(strcmp(IM[PC/4].searchopcode(),"beq")==0||strcmp(IM[PC/4].searchopcode(),"sw")==0){
			theScoreBoard.Integer.Fk=(Register*)IM[PC/4].searchoprand2();
			theScoreBoard.Integer.Qk=theScoreBoard.RegStatus.getFU(theScoreBoard.Integer.Fk);
			theScoreBoard.Integer.Rk=(theScoreBoard.Integer.Qk==NULL);
		}
	}
	PC+=4;
}
void Read_oprands(int instaddr){//读取操作数阶段
	Iter->cur_state=2;
	if(strcmp(IM[instaddr/4].searchopcode(),"mul")==0){
		Mult1Input1=theScoreBoard.Mult1.Fj->searchValue();
		Mult1Input2=theScoreBoard.Mult1.Fk->searchValue();
			
	}else if(strcmp(IM[instaddr/4].searchopcode(),"add")==0||strcmp(IM[instaddr/4].searchopcode(),"adi")==0||strcmp(IM[instaddr/4].searchopcode(),"sub")==0){
		if(strcmp(IM[instaddr/4].searchopcode(),"adi")==0){
			AddInput1=theScoreBoard.Add.Fj->searchValue();
		}else{
			AddInput1=theScoreBoard.Add.Fj->searchValue();
			AddInput2=theScoreBoard.Add.Fk->searchValue();
		}
	}else if(strcmp(IM[instaddr/4].searchopcode(),"div")==0){
		DivInput1=theScoreBoard.Div.Fj->searchValue();
		DivInput2=theScoreBoard.Div.Fk->searchValue();
	}else{
		if(strcmp(IM[instaddr/4].searchopcode(),"sll")==0||strcmp(IM[instaddr/4].searchopcode(),"jr")==0||strcmp(IM[instaddr/4].searchopcode(),"lw")==0){
			IntegerInput1=theScoreBoard.Integer.Fj->searchValue();
		}else{
			IntegerInput1=theScoreBoard.Integer.Fj->searchValue();
			IntegerInput2=theScoreBoard.Integer.Fk->searchValue();
		}
	}
}
void Execute(int instaddr){
	Iter->cur_state=3;
	if(strcmp(IM[instaddr/4].searchopcode(),"add")==0){
		AddOutput=AddInput1+AddInput2;
	}else if(strcmp(IM[instaddr/4].searchopcode(),"sub")==0){
		AddOutput=AddInput1-AddInput2;
	}else if(strcmp(IM[instaddr/4].searchopcode(),"adi")==0){
		AddOutput=AddInput1+(*((int*)IM[instaddr/4].searchoprand3()));
	}else if(strcmp(IM[instaddr/4].searchopcode(),"mul")==0){
		Mult1Output=Mult1Input1*Mult1Input2;
	}else if(strcmp(IM[instaddr/4].searchopcode(),"div")==0){
		DivOutput=DivInput1/DivInput2;
	}else if(strcmp(IM[instaddr/4].searchopcode(),"sll")==0){
		IntegerOutput=IntegerInput1<<(*((int*)IM[instaddr/4].searchoprand3()));
	}
}
void Write_back(list<inststatus>::iterator theIter){
	if(strcmp(IM[theIter->instaddr/4].searchopcode(),"jr")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"beq")==0){
		if(strcmp(IM[theIter->instaddr/4].searchopcode(),"jr")==0){
			PC=IntegerInput1;
		}else{
			if(IntegerInput1==IntegerInput2){
				PC=theIter->instaddr+8;
			}
		}
		list<inststatus>::iterator temp=theIter;
		while(temp!=theScoreBoard.instbuf.end()){
			theIter++;
			if(strcmp(IM[temp->instaddr/4].searchopcode(),"mul")==0){
				theScoreBoard.Mult1.isBusy=false;
				theScoreBoard.Mult1.Rj=theScoreBoard.Mult1.Rk=true;
				delete []theScoreBoard.Mult1.Op;
				theScoreBoard.Mult1.Op=NULL;
				theScoreBoard.RegStatus.setFU(theScoreBoard.Mult1.Fi,NULL);
				theScoreBoard.Mult1.Fi=theScoreBoard.Mult1.Fj=theScoreBoard.Mult1.Fk=NULL;
				theScoreBoard.Mult1.Qj=theScoreBoard.Mult1.Qk=NULL;
			}else if(strcmp(IM[temp->instaddr/4].searchopcode(),"add")==0||strcmp(IM[temp->instaddr/4].searchopcode(),"adi")==0||strcmp(IM[temp->instaddr/4].searchopcode(),"sub")==0){
				theScoreBoard.Add.isBusy=false;
				theScoreBoard.Add.Rj=theScoreBoard.Add.Rk=true;
				delete []theScoreBoard.Add.Op;
				theScoreBoard.Add.Op=NULL;
				theScoreBoard.RegStatus.setFU(theScoreBoard.Add.Fi,NULL);
				theScoreBoard.Add.Fi=theScoreBoard.Add.Fj=theScoreBoard.Add.Fk=NULL;
				theScoreBoard.Add.Qj=theScoreBoard.Add.Qk=NULL;
			}else if(strcmp(IM[temp->instaddr/4].searchopcode(),"div")==0){
				theScoreBoard.Div.isBusy=false;
				theScoreBoard.Div.Rj=theScoreBoard.Div.Rk=true;
				delete []theScoreBoard.Div.Op;
				theScoreBoard.Div.Op=NULL;
				theScoreBoard.RegStatus.setFU(theScoreBoard.Div.Fi,NULL);
				theScoreBoard.Div.Fi=theScoreBoard.Div.Fj=theScoreBoard.Div.Fk=NULL;
				theScoreBoard.Div.Qj=theScoreBoard.Div.Qk=NULL;
			}else{
				theScoreBoard.Integer.isBusy=false;
				theScoreBoard.Integer.Rj=theScoreBoard.Integer.Rk=true;
				delete []theScoreBoard.Integer.Op;
				theScoreBoard.Integer.Op=NULL;
				if(strcmp(IM[temp->instaddr/4].searchopcode(),"slt")==0||strcmp(IM[temp->instaddr/4].searchopcode(),"sll")==0||strcmp(IM[temp->instaddr/4].searchopcode(),"lw")==0){
					theScoreBoard.RegStatus.setFU(theScoreBoard.Integer.Fi,NULL);
				}
				theScoreBoard.Integer.Fi=theScoreBoard.Integer.Fj=theScoreBoard.Integer.Fk=NULL;
				theScoreBoard.Integer.Qj=theScoreBoard.Integer.Qk=NULL;
			}
			theScoreBoard.instbuf.erase(temp);
			temp=theIter;
		}
		return;
	}
	if(strcmp(IM[theIter->instaddr/4].searchopcode(),"sw")==0){
		DM[(ebp.searchValue()-IntegerInput2)/4]=IntegerInput1;
	}else{
		if(strcmp(IM[theIter->instaddr/4].searchopcode(),"lw")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand2())->setValue(DM[(ebp.searchValue()-IntegerInput1)/4]);
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"sll")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand2())->setValue(IntegerOutput);
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"slt")==0){
			if(IntegerInput1<IntegerInput2){
				((Register*)IM[theIter->instaddr/4].searchoprand3())->setValue(1);
			}else{
				((Register*)IM[theIter->instaddr/4].searchoprand3())->setValue(0);
			}
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"adi")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand2())->setValue(AddOutput);
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"add")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"sub")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand3())->setValue(AddOutput);
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"mul")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand3())->setValue(Mult1Output);
		}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"div")==0){
			((Register*)IM[theIter->instaddr/4].searchoprand3())->setValue(DivOutput);
		}
	}
	if(strcmp(IM[theIter->instaddr/4].searchopcode(),"mul")==0){
		theScoreBoard.Mult1.isBusy=false;
		theScoreBoard.Mult1.Rj=theScoreBoard.Mult1.Rk=true;
		delete []theScoreBoard.Mult1.Op;
		theScoreBoard.Mult1.Op=NULL;
		theScoreBoard.RegStatus.setFU(theScoreBoard.Mult1.Fi,NULL);
		theScoreBoard.Mult1.Fi=theScoreBoard.Mult1.Fj=theScoreBoard.Mult1.Fk=NULL;
		theScoreBoard.Mult1.Qj=theScoreBoard.Mult1.Qk=NULL;
	}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"add")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"adi")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"sub")==0){
		theScoreBoard.Add.isBusy=false;
		theScoreBoard.Add.Rj=theScoreBoard.Add.Rk=true;
		delete []theScoreBoard.Add.Op;
		theScoreBoard.Add.Op=NULL;
		theScoreBoard.RegStatus.setFU(theScoreBoard.Add.Fi,NULL);
		theScoreBoard.Add.Fi=theScoreBoard.Add.Fj=theScoreBoard.Add.Fk=NULL;
		theScoreBoard.Add.Qj=theScoreBoard.Add.Qk=NULL;
	}else if(strcmp(IM[theIter->instaddr/4].searchopcode(),"div")==0){
		theScoreBoard.Div.isBusy=false;
		theScoreBoard.Div.Rj=theScoreBoard.Div.Rk=true;
		delete []theScoreBoard.Div.Op;
		theScoreBoard.Div.Op=NULL;
		theScoreBoard.RegStatus.setFU(theScoreBoard.Div.Fi,NULL);
		theScoreBoard.Div.Fi=theScoreBoard.Div.Fj=theScoreBoard.Div.Fk=NULL;
		theScoreBoard.Div.Qj=theScoreBoard.Div.Qk=NULL;
	}else{
		theScoreBoard.Integer.isBusy=false;
		theScoreBoard.Integer.Rj=theScoreBoard.Integer.Rk=true;
		delete []theScoreBoard.Integer.Op;
		theScoreBoard.Integer.Op=NULL;
		if(strcmp(IM[theIter->instaddr/4].searchopcode(),"slt")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"sll")==0||strcmp(IM[theIter->instaddr/4].searchopcode(),"lw")==0){
			theScoreBoard.RegStatus.setFU(theScoreBoard.Integer.Fi,NULL);
		}
		theScoreBoard.Integer.Fi=theScoreBoard.Integer.Fj=theScoreBoard.Integer.Fk=NULL;
		theScoreBoard.Integer.Qj=theScoreBoard.Integer.Qk=NULL;
	}
	theScoreBoard.instbuf.erase(theIter);
}