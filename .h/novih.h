/*
 * novih.h
 *
 *  Created on: Sep 12, 2020
 *      Author: OS1
 */

#ifndef NOVIH_H_
#define NOVIH_H_
#include <IOSTREAM.H>
#include <STDIO.H>
#include "Thread.h"
#include "PCB.h"

void init_intr();

void restore_intr();

void interrupt timer();

void dispatch();

void exitThread();

#endif /* NOVIH_H_ */
