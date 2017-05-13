#pragma once
#include"ASN.1.h"
#include"integer.h"
#include<iostream>
class enumeratedASN : public integerASN//!
{
	std::string* content;
	int size;
public:

	enumeratedASN(int nv = 0) { tag = 10; value = nv; }
	~enumeratedASN() { delete[] content; }
	void display();
	void saveTofile(const char* filename);
	void fill(std::string* ntab, int nsize);


};