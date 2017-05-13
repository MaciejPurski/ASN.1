#include"ASN.1.h"
#include"utf8string.h"
#include"Exceptions.h"
#include<string>
#include<sstream>
#include<fstream>
void stringASN::setvalue(std::string n)//!Funkcja wprowadzajaca nowa wartosc
{
	value = n;
}

int stringASN::countLength()//!Funkcja liczaca dlugosc stringa
{
	unsigned char length;
	length = static_cast<unsigned char>(value.length());
	return length;
}

void stringASN::codeValue(std::ostream& dataOut)//!Funkcja kodujaca stringa
{
	for (unsigned int i = 0; i < value.length(); i++)
	{
		dataOut.put(value[i]);
	}

}


void stringASN::decodeValue(std::istream& dataIn, int length)//!Funkcja dekodujaca stringa
{
	std::string nvalue;
	char buf;
	value = "";
	for (int i = 0; i < length; i++)
	{
		buf = dataIn.get();
		if (dataIn.eof())
			throw(EndofStream("Stream suddenly ended"));
		value = value + buf;
	}
}

std::string stringASN::getstring()//!Funkcja zwracajaca stringa
{
	return value;
}

void stringASN::display()//!Funkcja wyswietlajaca stringa
{
	std::cout << "stringASN " << value << std::endl;
	std::cout << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	std::cout << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		std::cout << (int)(y[i]) << " ";
	}
	std::cout << std::endl;
}

void stringASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku stringa
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (FileNotopen("File open failed"));
	file << "STRING: " << value << std::endl;
	file << "ASN.1 code(hex): " << std::endl << std::hex;
	std::stringstream ss;
	serialize(ss);
	std::string y = ss.str();
	file << std::showbase << std::hex;
	for (unsigned int i = 0; i < y.size(); i++)
	{
		file << (int)(y[i]) << " ";
	}
	file<< std::endl;
	file.close();
}