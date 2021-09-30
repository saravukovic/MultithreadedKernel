/*
 * IdleThread.h
 *
 *  Created on: Sep 28, 2020
 *      Author: OS1
 */

#ifndef IDLET_H_
#define IDLET_H_
#include "Thread.h"
#include "PCB.h"
class IdleThread:public Thread{
public:
	IdleThread():Thread(){}
	void run();
#ifndef BCC_BLOCK_IGNORE
	PCB* getMyPCB(){
		return PCB::getPCB((Thread*)this);
#endif
	}
};

#endif /* IDLETHREAD_H_ */
