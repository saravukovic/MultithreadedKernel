#include "SCHEDULE.H"
#include "kSem.h"
#include "semaphor.h"
#include "novih.h"
#include "blocLis.h"

class ListBlockedSem;

Semaphore::Semaphore(int init){
	lock
	myImpl=new KernelSem(init);
	unlock
}
Semaphore::~Semaphore(){
	lock
	delete myImpl->blocked;
	delete myImpl;
	unlock
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val() const{
	return myImpl->getValue();
}

volatile ListBlockedSem *blockTimeSem=new ListBlockedSem();
volatile ListBlockedSem *blockZeroSem= new ListBlockedSem();

int Semaphore::wait(Time maxTimeToWait){
	lock
	myImpl->wait();
	if((myImpl->value)<0){
		Block *bl=new Block(maxTimeToWait,(PCB*)PCB::running);
		if(maxTimeToWait==0) {
			blockZeroSem->operator+=(bl);
		}
		else {
			blockTimeSem->operator+=(bl);
		}
		dispatch();
		if(!bl->unblock){
			myImpl->value++;
			for(int i=0;i<myImpl->blocked->brojpodataka();i++){
				if(myImpl->blocked->operator [](i)==bl->pcb) myImpl->blocked->operator ()(i);
			}
		}
		unlock;
		return bl->unblock;
	}
	else {
		unlock;
		return 1;
	}

}
