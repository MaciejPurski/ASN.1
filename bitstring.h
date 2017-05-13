#pragma once
#include"ASN.1.h"
#include<iostream>
class bitstringASN : public ASNobject//!
{
	bool* bitstring;
	int size;
	std::string* content;
public:
	bitstringASN(int nsize = 0) { tag = 3;  size = nsize;  bitstring = new bool[size]; }
	void setvalue(bool* arg, int nsize);
	bool* getstring();
	~bitstringASN() { delete[]content; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void display();
	void saveTofile(const char* filename);
	void fill(std::string* ntab, int nsize); //funkcja wypelniajaca liste wielowybory
};
