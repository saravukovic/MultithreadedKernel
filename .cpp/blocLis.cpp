#include "blocLis.h"
#include "PCB.h"
//#ifndef BCC_BLOCK_IGNORE
void ListBlockedSem::brisi() {
	lock
	Elem* stari;
	while (prvi) {
		stari = prvi;
		prvi = prvi->sl;
		stari->podatak = NULL;
		delete stari;
	}
	unlock
}

void ListBlockedSem::premesti(ListBlockedSem& l) {
	lock
	duzina = l.duzina;
	prvi = l.prvi;
	posl = l.posl;
	l.prvi = NULL;
	l.posl = NULL;
	unlock
}

void ListBlockedSem::kopiraj(const ListBlockedSem& l) {
	lock
	duzina = 0;
	for (Elem* tek = prvi; tek; tek = tek->sl) {
		*this += tek->podatak;
	}
	unlock
}

ListBlockedSem& ListBlockedSem::operator+=(Block* t) {
	lock
	duzina++;
	Elem* novi = new Elem(t);
	posl = (prvi ? posl->sl : prvi) = novi;
	unlock
	return *this;
}

Block* ListBlockedSem::operator[](int i) {
	Elem* tek = prvi;
	for (; i > 0; i--, tek = tek->sl);
	return tek->podatak;
}

void ListBlockedSem::operator()(int i) {
	lock
	Elem* tek = prvi;
	int j = 1;
	if (duzina == 1) {
		prvi = NULL;
		posl = NULL;
		tek=NULL;
	}
	else if (i == 0) {
		Elem* stari = prvi;
		prvi = prvi->sl;
		stari=NULL;
	}
	else {
		while (i > j) {
			j++;
			tek = tek->sl;
		}
		Elem* stari = tek->sl;
		tek->sl = tek->sl->sl;
		stari=NULL;
		if (!tek->sl) posl = tek;
	}
	duzina--;
	unlock
}
//#endif
