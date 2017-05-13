#pragma once
#include"ASN.1.h"
#include<iostream>
class integerASN : public ASNobject//!
{
protected:
	int value;
public:
	integerASN(int x = 0) { tag = 2;  value = x; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void setvalue(int b);
	int getvalue();
	void display();
	void saveTofile(const char* filename);

};
