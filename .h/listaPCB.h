/*
 * listaPCB.h
 *
 *  Created on: Sep 19, 2020
 *      Author: OS1
 */

#ifndef LISTAPCB_H_
#define LISTAPCB_H_
#include <IOSTREAM.H>
#include <STDIO.H>
#include "PCB.h"

#define lock{\
	asm pushf;\
	asm cli;\
}

#define unlock asm popf

class ListPCB{
	struct Elem {
		PCB* podatak;
		Elem* sl;
		Elem(PCB* t) :sl(NULL){
			podatak=t;
		}
	};
	Elem* prvi, *posl;
	int duzina;
	void premesti(ListPCB& l);
	void kopiraj(const ListPCB& l);
	void brisi();
public:
	ListPCB():prvi(NULL), posl(NULL),duzina(0) {};
	ListPCB(const ListPCB& l) { kopiraj(l); }
	ListPCB& operator=(const ListPCB& l) {
		if (this != &l) {
			brisi();
			kopiraj(l);
		}
		return *this;
	}
	~ListPCB() { brisi(); }
	ListPCB& operator+=(PCB* t);
	int brojpodataka() const { return duzina; }
	PCB* operator[](int i);
	/*const PCB & operator[](int i) const {
		return (const PCB&)(*this)[i];
	}*/
	void operator()(int i);
};

#endif /* LISTAPCB_H_ */
