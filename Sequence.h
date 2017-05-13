#pragma once
#include"ASN.1.h"
#include<vector>
#include<iostream>
class sequenceASN : public ASNobject//!
{
protected:
	std::vector<ASNobject*> tab;
public:
	sequenceASN() { tag = 16; }
	~sequenceASN() {}
	sequenceASN(std::vector<ASNobject*> ntab) { tag = 16;  tab = ntab; }
	int countLength();
	void codeValue(std::ostream& dataOut);
	void decodeValue(std::istream& dataIn, int length);
	void display();
	void saveTofile(const char* filename);


};
