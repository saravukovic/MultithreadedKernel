#include "Thread.h"
#include "SCHEDULE.H"
#include <IOSTREAM.H>
#include <DOS.H>
#include "PCB.h"
#include "lista.h"
#include "novih.h"
#include "listaPCB.h"

static ID stid=0;
List* threads=new List();

Thread::Thread(StackSize s,Time t){
	lock
	myPCB=new PCB(s,t,this);
	threads->operator +=(this);
	unlock
}

void Thread::start(){
	put(myPCB);
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}
Thread * Thread::getThreadById(ID id){
	for(int i=0;i<threads->brojpodataka();i++){
		Thread* t=threads->operator [](i);
		if(t->getId()==id) return t;
	}
	return NULL;
}

void Thread::waitToComplete(){
	lock
	if(!myPCB->finished){
		(PCB*)PCB::running->blocked++;
		myPCB->wait->operator +=((PCB*)PCB::running);
		dispatch();
	}
	unlock
}

Thread::~Thread(){
	this->waitToComplete();
	lock
	int i;
	if(threads->operator [](0)){
		for(i=0;i<threads->brojpodataka();i++){
			if(threads->operator [](i)->getId()==this->getId()){
				threads->operator ()(i);
				break;
			}
		}
	}
	delete [] this->myPCB->stack;
	delete this->myPCB->wait;
	delete this->myPCB;
	unlock
}


PCB::PCB(StackSize s,Time t, Thread* thr){
	lock
	if(s>8192) s=defaultStackSize;
	stack=new unsigned[s];
	timeSlice=t;
	finished=0;
	stack[s-1]=0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[s-2]=FP_SEG(PCB::wrapper);
	stack[s-3]=FP_OFF(PCB::wrapper);
	ss=FP_SEG(stack+s-12);
	sp=FP_OFF(stack+s-12);
	bp=FP_OFF(stack+s-12);
#endif
	id=stid++;
	myThread=thr;
	blocked=0;
	PCB::brojNiti++;
	wait=new ListPCB();
	unlock
}

void PCB::wrapper(){
	running->myThread->run();
	exitThread();
}
