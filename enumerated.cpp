#include<iostream>
#include"ASN.1.h"
#include "enumerated.h"
#include<sstream>
#include<fstream>
#include"Exceptions.h"
#include<string>

void enumeratedASN::display()//!Funkcja Wyswietlajaca na ekran enuma
{
		std::cout << "enumeratedASN -state(decimal): " << value << std::endl;
		std::cout << "- " << content[value] << std::endl;
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

void enumeratedASN::saveTofile(const char* filename)//!Funkcja zapisujaca do pliku 
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (!file.good())
		throw (FileNotopen("File open failed"));
	file << "enumeratedASN -state(decimal): " << value << std::endl;
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

void enumeratedASN::fill(std::string* ntab, int nsize)//!Funkcja przypisujaca nazwy poszczegolnym wartosciom
{
	size = nsize;
	content = new std::string[size + 1];
	for (int i = 0; i < nsize+1; i++)
	{
		content[i] = ntab[i];
	}
}