#include "SCHEDULE.H"
#include <IOSTREAM.H>
#include <DOS.H>
#include "novih.h"
#include "PCB.h"
#include "blocLis.h"
#include "listaPCB.h"
#include "idlet.h"


volatile int csod=0;
unsigned tss,tsp,tbp;
volatile int cntr=((PCB*)PCB::running)->timeSlice;
volatile int PCB::brojNiti;
volatile int prvi=0;

extern volatile ListBlockedSem *blockTimeSem;
extern volatile ListBlockedSem *blockZeroSem;

extern void tick();

volatile ListPCB *blockList= new ListPCB();

extern IdleThread *idle;
extern PCB* startContext;
volatile int idl=0;

void IdleThread::run(){
	while(!idl){
		if(PCB::brojNiti==2) {
			break;
		}
	}
}


void put(PCB* pcb){
	lock
	if(pcb!=NULL && pcb->id!=1){
		Scheduler::put(pcb);
	}
	unlock
}


PCB* get(){
	lock
	PCB* pcb=Scheduler::get();
	if(PCB::brojNiti==1){
		unlock
		return startContext;
	}
	else if(pcb){
		unlock;
		return pcb;
	}
	else{
		unlock;
		return idle->getMyPCB();
	}
}

unsigned oldTimerOffs,oldTimerSeg;

void semBlockUpdate();

void interrupt timer(){
	if(!csod) cntr--;
	if(csod || !cntr || (csod&&!((PCB*)PCB::running)->timeSlice)){
		asm{
			mov tsp,sp
			mov tss,ss
			mov tbp,bp
		}
		((PCB*)PCB::running)->sp=tsp;
		((PCB*)PCB::running)->ss=tss;
		((PCB*)PCB::running)->bp=tbp;
		if(!((PCB*)PCB::running)->finished && !((PCB*)PCB::running)->blocked) put(((PCB*)PCB::running));
		((PCB*)PCB::running)=get();
		tsp=((PCB*)PCB::running)->sp;
		tss=((PCB*)PCB::running)->ss;
		tbp=((PCB*)PCB::running)->bp;
		cntr=((PCB*)PCB::running)->timeSlice;
		asm{
			mov sp,tsp
			mov ss,tss
			mov bp,tbp
		}
	}
	if(!csod) {
		asm int 60h;
		semBlockUpdate();
	}
	csod=0;
	tick(); //kada se ubaci test okruzenje!!!!
}

#ifndef BCC_BLOCK_IGNORE
void semBlockUpdate(){
	if(blockTimeSem->brojpodataka()>0)
		for(int i=0;i<blockTimeSem->brojpodataka();i++){
			Block* cur = blockTimeSem->operator[](i);
			if((--cur->timeToWait)==0){
				if(cur->pcb->blocked){
					cur->pcb->blocked=0;
					cur->unblock=0;
					put(cur->pcb);
				}
				blockTimeSem->operator ()(i);
			}
		}
}
#endif

void init_intr(){
	asm{
		cli

		push ax
		push es

		mov ax,0
		mov es,ax

		mov ax,word ptr es:0022h
		mov word ptr oldTimerSeg,ax
		mov ax,word ptr es:0020h
		mov word ptr oldTimerOffs,ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSeg
		mov word ptr es:0182h,ax
		mov ax, oldTimerOffs
		mov word ptr es:0180h,ax

		pop es
		pop ax

		sti
	}
}

void restore_intr(){
	asm{
		cli
		push ax
		push es

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSeg
		mov word ptr es:0022h,ax
		mov ax, word ptr oldTimerOffs
		mov word ptr es:0020h,ax

		pop es
		pop ax
		sti
	}
}

void dispatch(){
	lock
	csod=1;
	timer();
	//asm int 8h;
	unlock
}

extern ListPCB *blockedByW;

void exitThread(){
	lock
	PCB* curr = ((PCB*)PCB::running);
	curr->finished=1;
#ifndef BCC_BLOCK_IGNORE
	if(curr->wait->brojpodataka()){
		for(int i=0;i<PCB::running->wait->brojpodataka();i++){
			PCB* w = PCB::running->wait->operator [](i);
			w->blocked--;
			if(w->blocked==0) {
				put(w);
			}
			curr->wait->operator ()(i);
#endif
		}
	}
	PCB::brojNiti--;
	//unlock
	dispatch();
	unlock
}
