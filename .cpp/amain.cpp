#include "Thread.h"
#include "event.h"
#include "PCB.h"
#include <IOSTREAM.H>
#include <DOS.H>
#include <STDLIB.H>
#include "novih.h"
#include "semaphor.h"
#include "idlet.h"


PCB* startContext=new PCB(defaultStackSize,defaultTimeSlice,NULL);
volatile PCB *PCB::running;

extern int userMain(int argc,char* argv[]);
int ret;

class UserThread:public Thread{
	int argc;
	char** argv;
public:
	UserThread(int c,char** v):Thread(),argc(c),argv(v){}
protected:
	void run(){
		ret=userMain(argc,argv);
	}
};


IdleThread* idle;

int main(int argc,char** argv){
	//lock
	if(argc <2){
		cout<<"Invalid input!"<<endl;
		//unlock
		return -1;
	}
	PCB::running=startContext;
	idle=new IdleThread();
	UserThread* user=new UserThread(argc,argv);
	init_intr();
	user->start();
	user->waitToComplete();
	startContext->finished=1;
	restore_intr();
	delete user;
	delete idle;
	//delete startContext;
	//unlock
	return ret;
}
