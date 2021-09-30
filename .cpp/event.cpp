#include "event.h"
#include "kernelEv.h"
Event::Event (IVTNo ivtNo){
	lock
	myImpl=new KernelEv(ivtNo);
	unlock
}
Event::~Event (){
	lock
	delete myImpl;
	unlock
}
void Event::wait (){
	if(myImpl->myThread==PCB::running) myImpl->block();
}
void Event::signal(){
	myImpl->deblock();
}

