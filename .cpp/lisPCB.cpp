#include "listaPCB.h"
void ListPCB::brisi() {
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

void ListPCB::premesti(ListPCB& l) {
	lock
	duzina = l.duzina;
	prvi = l.prvi;
	posl = l.posl;
	l.prvi = NULL;
	l.posl = NULL;
	unlock
}

void ListPCB::kopiraj(const ListPCB& l) {
	lock
	duzina = 0;
	for (Elem* tek = prvi; tek; tek = tek->sl) {
		*this += tek->podatak;
	}
	unlock
}

ListPCB& ListPCB::operator+=(PCB* t) {
	lock
	duzina++;
	Elem* novi = new Elem(t);
	posl = (prvi ? posl->sl : prvi) = novi;
	unlock
	return *this;
}

PCB* ListPCB::operator[](int i) {
	lock
	Elem* tek = prvi;
	for (; i > 0; i--, tek = tek->sl);
	unlock
	return tek->podatak;
}

void ListPCB::operator()(int i) {
	lock
	Elem* tek = prvi;
	int j = 1;
	if (duzina == 1) {
		prvi = NULL;
		posl = NULL;
		tek->podatak=NULL;
		tek->sl=NULL;
		delete tek;
	}
	else if (i == 0) {
		Elem* stari = prvi;
		prvi = prvi->sl;
		stari->podatak=NULL;
		stari->sl=NULL;
		delete stari;
	}
	else {
		while (i > j) {
			j++;
			tek = tek->sl;
		}
		Elem* stari = tek->sl;
		tek->sl = tek->sl->sl;
		stari->podatak=NULL;
		stari->sl=NULL;
		delete stari;
		if (!tek->sl) posl = tek;
	}
	duzina--;
	unlock
}
