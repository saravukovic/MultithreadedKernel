#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "event.h"
#include "ivtentry.h"
#include "PCB.h"

class KernelEv{
	public:
	KernelEv(IVTNo ivtno);
	int getIVTNo() const {
		return ivtNo;
	}
	~KernelEv();
	void block();
	void deblock();
protected:
	friend class Event;
private:
	int value;
	IVTNo ivtNo;
	PCB *myThread;
};

#endif /* KERNELEV_H_ */
