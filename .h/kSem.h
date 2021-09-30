/*
 * kernelSem.h
 *
 *  Created on: Sep 16, 2020
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_
#include "listaPCB.h"
#include "PCB.h"
#include "semaphor.h"
class KernelSem{
public:
	KernelSem(int v);
	int getValue() const {
		return value;
	}
	void wait();
	void signal();
	ListPCB* getBlocked(){
		return blocked;
	}
protected:
	friend class Semaphore;
	void block();
	void deblock();
private:
	ListPCB *blocked;
	int value;
	int lck;
};

#endif /* KSEM_H_ */
