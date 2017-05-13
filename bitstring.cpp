#include"ASN.1.h"
#include"Exceptions.h"
#include"bitstring.h"
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
void bitstringASN::setvalue(bool* arg, int nsize)
{
	if (nsize != size)
	{
		delete[] bitstring;
		bitstring = new bool[nsize];
		size = nsize;
	}
	for (int i = 0; i < size; i++)
	{
		bitstring[i] = arg[i];
	}
}

void bitstringASN::decodeValue(std::istream& dataIn, int length)
{
	unsigned char comp;
	int k = 0;
	unsigned char buf = dataIn.get();
	if (size != (8 * length - buf - 8))
	{
		delete[] bitstring;
		bitstring = new bool[8 * length - buf - 8];
	}

	for (int i = 1; i < length; i++)
	{
		buf = dataIn.get();
		if (dataIn.eof())
			throw(EndofStream("Stream suddenly ended"));
		comp = 128;
		for (int j = 0; j < 8; j++)
		{
			if ((buf & comp) == 0)
				bitstring[k] = 0;
			else
				bitstring[k] = 1;
			comp = comp >> 1;
			k++;
		}
	}
}

bool* bitstringASN::getstring()
{
	return bitstring;
}

int bitstringASN::countLength()
{
	unsigned char length;

	length = size / 8;
	if ((size % 8) != 0)
		length++;
	length++;

	return length;
}
void bitstringASN::codeValue(std::ostream& dataOut)
{
	int length = countLength();
	unsigned char buf;

	if (size % 8 == 0)
		dataOut.put(0);
	else
		dataOut.put(8 - size % 8);
	int k = 0;

	for (int i = 1; i < length; i++)
	{
		buf = 0;
		int j = 0;
		while (j != 8)
		{
			buf = buf << 1;
			buf = buf + bitstring[k];
			j++;
			k++;
			if (k == size)
			{
				buf = buf << (8 - j);
				dataOut.put(buf);
				break;
			}
			if (i > size)
				throw (ArrayOut("Array of bits is out of bounds"));
		}
		dataOut.put(buf);
	}
}

void bitstringASN::display()
{
	std::cout << "bitstringASN: "<<std::dec;
	for (int i = 0; i < size; i++)
		std::cout << bitstring[i];
	std::cout << std::endl;
	for (int i = 0; i < size; i++)
	{
		if (bitstring[i])
			std::cout << content[i]<<" ";
	}
	std::cout <<std::endl<< "ASN.1 code(hex): " << std::endl;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase<<std::hex;
	for (unsigned int i = 0; i < y.size()-1; i++)
	{
		std::cout << (unsigned int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void bitstringASN::saveTofile(const char* filename)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (FileNotopen("File open failed"));
	file << "bitstringASN: " << std::dec;
	for (int i = 0; i < size; i++)
		file << bitstring[i];
	file << std::endl << "ASN.1 code(hex): " << std::endl;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (unsigned int)(y[i]) << " ";
	}
	file << std::endl;
}

void bitstringASN::fill(std::string* ntab, int nsize)
{
	size = nsize;
	content = new std::string[size];
	for (int i = 0; i < nsize; i++)
	{
		content[i] = ntab[i];
	}
}