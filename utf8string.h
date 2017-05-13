#pragma once
#include "ASN.1.h"
#include<iostream>
class stringASN : public ASNobject//!
{
	std::string value;
public:
	stringASN(std::string n = "") { tag = 12;  value = n; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void setvalue(std::string n);
	std::string getstring();
	void display();
	void saveTofile(const char* filename);

};