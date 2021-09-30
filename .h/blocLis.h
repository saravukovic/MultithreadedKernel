#ifndef BLOCLIS_H_
#define BLOCLIS_H_
#include "Thread.h"
#include "PCB.h"
#include "semaphor.h"

struct Block{
	Time timeToWait;
	PCB* pcb;
	int unblock;
	Block(Time t, PCB* p):timeToWait(t),pcb(p),unblock(1){}
};

class ListBlockedSem{
	struct Elem {
		Block* podatak;
		Elem* sl;
		Elem(Block* t) :podatak(t),sl(NULL){}
	};
	Elem* prvi, *posl;
	int duzina;
	void premesti(ListBlockedSem& l);
	void kopiraj(const ListBlockedSem& l);
	void brisi();
public:
	ListBlockedSem():prvi(NULL), posl(NULL),duzina(0) {};
	ListBlockedSem(const ListBlockedSem& l) { kopiraj(l); }
	ListBlockedSem& operator=(const ListBlockedSem& l) {
		if (this != &l) {
			brisi();
			kopiraj(l);
		}
		return *this;
	}
	~ListBlockedSem() { brisi(); }
	ListBlockedSem& operator+=(Block* t);
	int brojpodataka() const { return duzina; }
	Block* operator[](int i) ;
	/*const Block & operator[](int i) const {
		return (const Block&)(*this)[i];
	}*/
	void operator()(int i);
};
#endif /* BLOCLIS_H_ */
