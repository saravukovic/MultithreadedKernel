#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"
#include <IOSTREAM.H>
#include "lista.h"
#include <DOS.H>
void exitThread();

#define lock{\
	asm pushf;\
	asm cli;\
}

#define unlock asm popf

class ListPCB;

class PCB{
public:
	PCB(){}
	PCB(StackSize s,Time t, Thread* thr);
	unsigned bp;
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned finished;
	int timeSlice;
	int blocked;
	Thread *myThread;
	static volatile int brojNiti;
	static volatile PCB *running;
	static void wrapper();
	ID id;
	ListPCB* wait;
	static PCB* getPCB(Thread* t){
		return t->myPCB;
	}
};
void put(PCB* pcb);
PCB* get();
#endif /* PCB_H_ */
