#include"ASN.1.h"
#include "integer.h"
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include"Exceptions.h"

int integerASN::countLength()//!Funkcja liczaca dlugosc integerASNa
{
	unsigned char length = 1;
	int maxl = 128;
	if (value >= 0)
	{
		while ((maxl - 1) < value)
		{
			length++;
			maxl = maxl << 8;
		}
	}
	else
	{
		while (maxl < (-1)*value)
		{
			length++;
			maxl = maxl << 8;
		}
	}
	return length;
}

void integerASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca wartosc inta
{
	int length = countLength();
	for (int i = 0; i <length; i++)
	{
		dataOut << (unsigned char)(value >> ((length - 1 - i) * 8) & 0xFF);
	}
}

void integerASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca wartosc inta
{
	int temp;
	unsigned char byte;
	unsigned char oldestbyte;
	byte = dataIn.get();
	oldestbyte = byte;
	value = 0;
	for (int i = 0; i < length; i++)
	{
		temp = byte;
		temp = temp << (8 * (length - 1 - i)); //ustawiamy bajt w odpowiednie miejsce
		value = value | temp; //przypisujemy byte do value
		if (i != (length - 1)) //w ostatnim przebiegu petli nie wyciagamy bajtu
			byte = dataIn.get();
		if (dataIn.eof())
			throw(EndofStream("Stream suddenly ended"));
	}

	if ((0x80 & oldestbyte) == 128) //jesli pierwszy bit jest rowny 1 to znaczy ze liczba ujemna i trzeba dopelnic jedynkami
	{

		temp = 0xFF;
		temp = temp << 8 * length;
		for (int i = 0; i < (4 - length); i++)
		{
			value = value | temp;
			temp = temp << 8;
		}

	}


}


void integerASN::setvalue(int arg)//!Funkcja przypisujaca to inta nowa wartosc
{
	value = arg;
}


int integerASN::getvalue()//!Funkcja zwracajaca wartosc inta 
{
	return value;
}

void integerASN::display() //!Funkcja wyswietlajaca inta
{
	std::cout << "integerASN(decimal): " << std::dec<<value << std::endl;
	std::cout << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (int) (y[i]) << " ";
	}
	std::cout<<std::endl;
}

void integerASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (FileNotopen("File open failed"));
	file << "integerASN(decimal): " << value << std::endl;
	file << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (int)(y[i]) << " ";
	}
	file << std::endl;
	file.close();

}