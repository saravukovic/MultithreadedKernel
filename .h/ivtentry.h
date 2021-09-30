#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include <dos.h>
#include <IOSTREAM.H>
#include "event.h"
#include "kernelEv.h"

#define PREPAREENTRY(intNo, oldIr)\
void interrupt intRoutine##intNo(...){\
	IVTEntry* entry = IVTEntry::getIVTEntry(intNo);\
	entry->getKernelEv()->deblock();\
	if(oldIr) entry->getOldISR()();\
}\
IVTEntry entry##intNo(intNo,intRoutine##intNo);


typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	IVTEntry(unsigned char ivtno, pInterrupt oi=NULL);
	pInterrupt getOldISR(){
		return oldISR;
	}
	static IVTEntry* getIVTEntry(IVTNo broj);
	IVTNo getIvtNo(){
		return ivtNo;
	}
	~IVTEntry();
	KernelEv* getKernelEv(){
		return myKernelEv;
	}
private:
	friend class Event;
	friend class KernelEv;
	KernelEv* myKernelEv;
	IVTNo ivtNo;
	static IVTEntry* IVT[256];
	pInterrupt oldISR, newISR;
};

#endif /* IVTENTRY_H_ */
