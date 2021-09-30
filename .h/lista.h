#ifndef LISTA_H
#define LISTA_H
#include "Thread.h"
#include <IOSTREAM.H>
#include <STDIO.H>

class List{
	struct Elem {
		Thread* podatak;
		Elem* sl;
		Elem(Thread* t) :podatak(t),sl(NULL){}
	};
	Elem* prvi, *posl;
	int duzina;
	void premesti(List& l);
	void kopiraj(const List& l);
	void brisi();
public:
	List():prvi(NULL), posl(NULL),duzina(0) {};
	List(const List& l) { kopiraj(l); }
	List& operator=(const List& l) {
		if (this != &l) {
			brisi();
			kopiraj(l);
		}
		return *this;
	}
	~List() { brisi(); }
	List& operator+=(Thread* t);
	int brojpodataka() const { return duzina; }
	Thread* operator[](int i) ;
	const Thread & operator[](int i) const {
		return (const Thread&)(*this)[i];
	}
	void operator()(int i);
};


#endif // !LISTA_H
