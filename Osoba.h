#pragma once
#include"ASN.1.h"
#include"Sequence.h"
#include"integer.h"
#include"enumerated.h"
#include"bitstring.h"
#include"utf8string.h"

class Osoba :public sequenceASN//!Klasa przechowujaca dane osoby 
{
	stringASN imie;
	stringASN nazwisko;
	integerASN wiek;
	enumeratedASN plec;
	bitstringASN zainteresowania;
public:
	Osoba();
	Osoba(std::string nimie, std::string nnazwisko, int nwiek, bool nplec, bool* nzainteresowania);
	void setImie(std::string nimie);
	std::string getImie();
	void setNazwisko(std::string nnazwisko);
	std::string getNazwisko();
	void setWiek(int nwiek);
	int getWiek();
	void setPlec(bool nplec);
	bool getPlec();
	void setZainteresowania(bool[], int nsize);
	bool* getZainteresowania();
};