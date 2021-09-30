#include "ivtentry.h"

IVTEntry* IVTEntry::IVT[256]={NULL};

IVTEntry::IVTEntry(unsigned char ivtno, pInterrupt ni){
	lock
		if(getIVTEntry(ivtno)){
			unlock
			return;
		}
		ivtNo=ivtno;
		newISR=ni;
#ifndef BCC_BLOCK_IGNORE
		oldISR=getvect(ivtno);
		if(newISR) setvect(ivtNo,newISR);
#endif
		IVT[ivtNo]=this;
	unlock
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo broj){
	return IVT[broj];
}

IVTEntry::~IVTEntry(){
	lock
	(*oldISR)();
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo,oldISR);
#endif
	IVT[ivtNo]=NULL;
	unlock
}
