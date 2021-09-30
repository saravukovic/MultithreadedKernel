#ifndef KERNELEVENT_CPP_
#define KERNELEVENT_CPP_
#include "kernelEv.h"
#include "SCHEDULE.H"


KernelEv::KernelEv(IVTNo ivtno){
	lock
	ivtNo=ivtno;
	myThread=((PCB*)PCB::running);
	if(IVTEntry::getIVTEntry(ivtNo)) IVTEntry::getIVTEntry(ivtNo)->myKernelEv=this;
	value=0;
	unlock
}

void KernelEv::block(){
	lock
	if(!value){
		PCB::running->blocked=1;
		value--;
		dispatch();
	}
	else if(value==1) value--;
	unlock
}

void KernelEv::deblock(){
	lock
	if(value==0) {
		value=1;
		unlock
	}
	else if(value<0){
		myThread->blocked=0;
		put(myThread);
		value++;
		dispatch();
		unlock
	}
}

KernelEv::~KernelEv(){
	lock
	if(IVTEntry::getIVTEntry(ivtNo)) IVTEntry::getIVTEntry(ivtNo)->myKernelEv = NULL;
	unlock
}

#endif /* KERNELEVENT_CPP_ */
