#include "Thread.h"
#include "SCHEDULE.H"
#include <IOSTREAM.h>
#include "kSem.h"
#include "blocLis.h"

extern volatile ListBlockedSem *blockTimeSem;
extern volatile ListBlockedSem *blockZeroSem;

KernelSem::KernelSem(int v):value(v), lck(0){
	lock
	blocked=new ListPCB();
	unlock
}

void KernelSem::block(){
	lock
	((PCB*)PCB::running)->blocked=1;
	blocked->operator +=((PCB*)PCB::running);
	unlock
}

void KernelSem::deblock(){
	lock
	if(blocked->operator [](0)){
		PCB* b = blocked->operator [](0);
		for(int i=0;i<blockTimeSem->brojpodataka();i++){
			if(b->id==blockTimeSem->operator [](i)->pcb->id) blockTimeSem->operator ()(i);
		}
		for(i=0;i<blockZeroSem->brojpodataka();i++){
			if(b->id==blockZeroSem->operator [](i)->pcb->id) blockZeroSem->operator ()(i);
		}

		b->blocked=0;
		put(b);
		blocked->operator ()(0);
		dispatch();
	}
	unlock
}

void KernelSem::wait(){
	lock
	if(--value<0) block();
	unlock
}

void KernelSem::signal(){
	lock
	if(value++<0) deblock();
	unlock
}
