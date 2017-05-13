#include<iostream>
#include<sstream>
#include<fstream>
#include"ASN.1.h"
#include"Exceptions.h"
#include"Sequence.h"

int sequenceASN::countLength()//!Funkcja liczaca dlugosc sequenceASN'a
{
	unsigned char length = 0;
	unsigned int i;
	for (i = 0; i < tab.size(); i++)
	{
		length = length + tab[i]->countLength();
	}
	length = length + 2 *((unsigned char) tab.size());

	return length;
}

void sequenceASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca wartosc sequenceASN'a
{
	for (auto i = tab.begin(); i != tab.end(); i++)
	{
		(*i)->serialize(dataOut);
	}
}

void sequenceASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca wartosc sequenceASN'a
{
	int k = 0;
	for (auto i = tab.begin(); i != tab.end(); i++)
	{
		(*i)->deserialize(dataIn);
		k = k + 1 + (*i)->countLength() + countLengthBytes((*i)->countLength());
		if (k == length) break;
	}
}

void sequenceASN::display()//!Funkcja wyswietlajaca sequenceASN'a
{
	std::cout << std::dec << "sequenceASN: " << std::endl;
	for (unsigned int i = 0; i < tab.size(); i++)
	{
		tab[i]->display();
	}
}

void sequenceASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku sequenceASN'a
{

	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	if (!file.good())
		throw (FileNotopen("File open failed"));
	file << std::dec << "sequenceASN: " << std::endl;
	for (unsigned int i = 0; i < tab.size(); i++)
	{
		tab[i]->serialize(file);
	}
	file.close();
}